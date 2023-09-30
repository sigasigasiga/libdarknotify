#include "siga/dark_notify/impl/windows/reg_monitor.hpp"

#include <format>
#include <string_view>

#include "siga/dark_notify/impl/windows/util/error.hpp"

namespace siga::dark_notify::impl::windows {

namespace {

constexpr std::string_view reg_key_path = "Software\\"
                                          "Microsoft\\"
                                          "Windows\\"
                                          "CurrentVersion\\"
                                          "Themes\\"
                                          "Personalize\\";

constexpr std::string_view reg_key_name = "AppsUseLightTheme";

util::scoped_hkey_t make_theme_hkey() {
    HKEY ret;

    const auto status = ::RegOpenKeyEx(
        HKEY_CURRENT_USER,   // hKey
        reg_key_path.data(), // lpSubKey
        0,                   // ulOptions
        KEY_READ,            // samDesired
        &ret                 // phkResult
    );

    if(status == ERROR_SUCCESS) {
        return util::scoped_hkey_t{ret};
    } else {
        throw util::last_error_exception_t{};
    }
}

util::scoped_event_t make_event() {
    const auto raw_event = ::CreateEventA(
        nullptr, // lpEventAttributes
        false,   // bManualReset
        false,   // bInitialState
        nullptr  // lpName
    );

    if(raw_event == nullptr) {
        throw util::last_error_exception_t{};
    } else {
        return util::scoped_event_t{raw_event};
    }
}

void register_event(HKEY theme_hkey, HANDLE event) {
    const auto register_result = ::RegNotifyChangeKeyValue(
        theme_hkey,                 // hKey
        false,                      // bWatchSubtree
        REG_NOTIFY_CHANGE_LAST_SET, // dwNotifyFilter
        event,                      // hEvent
        true                        // fAsynchronous
    );

    if(register_result != ERROR_SUCCESS) {
        throw util::last_error_exception_t{};
    }
}

bool wait_event(HANDLE event, DWORD ms) {
    const auto result = ::WaitForSingleObject(event, ms);
    switch(result) {
        case WAIT_OBJECT_0: {
            return true;
        }

        case WAIT_TIMEOUT: {
            return false;
        }

        case WAIT_FAILED: {
            throw util::last_error_exception_t{};
        }
    }

    const auto error_string = std::format(
        "Got an unexpected result from `WaitForSingleObject`: {}",
        result
    );
    throw std::runtime_error{error_string};
}

} // anonymous namespace

reg_monitor_t::reg_monitor_t() : theme_hkey_{make_theme_hkey()} {
}

// dark_notify_t
void reg_monitor_t::register_callback(callback_t callback) {
    callback_ = callback;
}

void reg_monitor_t::unregister_callback() {
    callback_ = nullptr;
}

auto reg_monitor_t::query() -> appearance_t {
    DWORD value_type;
    DWORD value;
    DWORD value_size = sizeof(value);

    const auto query_status = ::RegQueryValueExA(
        theme_hkey_.get(),                // hKey
        reg_key_name.data(),              // lpValueName
        nullptr,                          // lpReserved
        &value_type,                      // lpType
        reinterpret_cast<LPBYTE>(&value), // lpData
        &value_size                       // lpcbData
    );

    if(query_status == ERROR_FILE_NOT_FOUND) {
        return appearance_t::unknown;
    } else if(query_status != ERROR_SUCCESS) {
        throw util::last_error_exception_t{query_status};
    }

    if(value_type != REG_DWORD) {
        throw std::runtime_error{"Unexpected registry value type"};
    }

    switch(value) {
        case 0: {
            return appearance_t::dark;
        }

        case 1: {
            return appearance_t::light;
        }

        default: {
            return appearance_t::unknown;
        }
    }
}

void reg_monitor_t::tick() {
    start_event_wait(0);
}

void reg_monitor_t::run() {
    while(true) {
        start_event_wait(INFINITE);
    }
}

// private
void reg_monitor_t::start_event_wait(DWORD ms) {
    if(!event_) {
        event_ = make_event();
        register_event(theme_hkey_.get(), event_.get());
    }

    if(wait_event(event_.get(), ms)) {
        event_.reset();
        callback_(query());
    }
}

} // namespace siga::dark_notify::impl::windows
