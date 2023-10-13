#include "siga/dark_notify/impl/windows/util/cancellable_event.hpp"

#include <array>
#include <format>

#include "siga/dark_notify/impl/windows/util/error.hpp"

namespace siga::dark_notify::impl::windows::util {

namespace {

util::scoped_event_t make_terminate_event() {
    const auto raw_event = ::CreateEventExA(
        nullptr,         // lpEventAttributes
        nullptr,         // lpName
        0,               // dwFlags
        EVENT_ALL_ACCESS // dwDesiredAccess
    );

    if(raw_event == nullptr) {
        throw util::last_error_exception_t{};
    } else {
        return util::scoped_event_t{raw_event};
    }
}

} // anonymous namespace

cancellable_event_t::cancellable_event_t()
    : terminate_event_{make_terminate_event()} //
{
}

auto cancellable_event_t::wait(DWORD ms) -> exit_reason_t {
    std::array raw_events{main_event_.get(), terminate_event_.get()};
    enum idx_t
    {
        main,
        terminate,
        _size
    };

    static_assert(std::ranges::size(raw_events) == _size);

    const auto result = ::WaitForMultipleObjects(
        std::ranges::size(raw_events), // nCount
        raw_events.data(),             // lpHandles
        false,                         // bWaitAll
        ms                             // dwMilliseconds
    );

    switch(result) {
        case WAIT_OBJECT_0 + idx_t::main: {
            return exit_reason_t::signaled;
        }

        case WAIT_OBJECT_0 + idx_t::terminate: {
            return exit_reason_t::terminated;
        }

        case WAIT_TIMEOUT: {
            return exit_reason_t::timeout;
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

void cancellable_event_t::terminate() {
    if(!::SetEvent(terminate_event_.get())) {
        throw last_error_exception_t{};
    }
}

} // namespace siga::dark_notify::impl::windows::util
