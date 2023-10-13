#pragma once

#include <siga/dark_notify/impl/windows/util/winapi_scoped_handle.hpp>
#include <siga/dark_notify/util/export.hpp>

namespace siga::dark_notify::impl::windows::util {

class SIGA_DARK_NOTIFY_EXPORT cancellable_event_t
{
public:
    enum class exit_reason_t
    {
        signaled,
        terminated,
        timeout
    };

public:
    cancellable_event_t();

public:
    HANDLE get() const { return main_event_.get(); }
    void set(scoped_event_t event) { main_event_ = std::move(event); }
    void reset() { set(scoped_event_t{}); }

    exit_reason_t wait(DWORD ms = INFINITE);
    void terminate();

private:
    scoped_event_t main_event_;
    scoped_event_t terminate_event_;
};

} // namespace siga::dark_notify::impl::windows::util
