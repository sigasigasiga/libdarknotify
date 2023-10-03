#pragma once

#include <system_error>

#include <windows.h>

namespace siga::dark_notify::impl::windows::util {

inline std::error_code make_sys_ec(int ec) {
    return std::error_code{ec, std::system_category()};
}

inline std::error_code make_sys_ec(LSTATUS ec) {
    return make_sys_ec(static_cast<int>(ec));
}

inline std::error_code make_sys_ec(DWORD ec) {
    return make_sys_ec(static_cast<int>(ec));
}

inline std::error_code make_sys_ec() {
    return make_sys_ec(::GetLastError());
}

// clang-format off

// This concept is needed in order to allow `last_error_exception_t` to be
// copy-construtible from non-const `last_error_exception_t&`.
//
// We need this type to be copy-constructible because its copies are made
// in the depths of C++ runtime library when an exception is thrown
template<typename... Args>
concept sys_ec_makable_from = requires(Args &&...args) {
    make_sys_ec(std::forward<Args>(args)...);
};

// clang-format on

class last_error_exception_t : public std::system_error
{
public:
    template<typename... Args>
    requires sys_ec_makable_from<Args...>
    last_error_exception_t(Args &&...args)
        : std::system_error{make_sys_ec(std::forward<Args>(args)...)} {}
};

} // namespace siga::dark_notify::impl::windows::util
