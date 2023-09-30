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

inline std::error_code make_sys_ec(DWORD ec = ::GetLastError()) {
    return make_sys_ec(static_cast<int>(ec));
}

class last_error_exception_t : public std::system_error
{
public:
    template<typename... Args>
    last_error_exception_t(Args &&...args)
        : std::system_error{make_sys_ec(std::forward<Args>(args)...)} {}
};

} // namespace siga::dark_notify::impl::windows::util
