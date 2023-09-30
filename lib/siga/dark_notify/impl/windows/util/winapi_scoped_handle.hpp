#pragma once

#include <cassert>

#include <windows.h>

#include <siga/dark_notify/util/basic_scoped_handle.hpp>
#include <siga/dark_notify/util/export.hpp>

namespace siga::dark_notify::impl::windows::util {

// HANDLE
class SIGA_DARK_NOTIFY_EXPORT close_handle_t
{
public:
    void operator()(HANDLE h) const noexcept;
};

using scoped_handle_t = basic_scoped_handle_t<HANDLE, -1, close_handle_t>;

inline void close_handle_t::operator()(HANDLE h) const noexcept {
    assert(scoped_handle_t::default_value() == INVALID_HANDLE_VALUE);
    (void)::CloseHandle(h);
}

// event
using scoped_event_t = basic_scoped_handle_t<HANDLE, 0, close_handle_t>;

// HKEY
class SIGA_DARK_NOTIFY_EXPORT reg_close_key_t
{
public:
    void operator()(HKEY h) const noexcept { (void)::RegCloseKey(h); }
};

using scoped_hkey_t = basic_scoped_handle_t<HKEY, HKEY{}, reg_close_key_t>;

} // namespace siga::dark_notify::impl::windows::util
