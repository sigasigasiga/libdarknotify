#pragma once

#include <siga/dark_notify/dark_notify.hpp>
#include <siga/dark_notify/impl/windows/util/winapi_scoped_handle.hpp>
#include <siga/dark_notify/util/export.hpp>

namespace siga::dark_notify::impl::windows {

class SIGA_DARK_NOTIFY_EXPORT reg_monitor_t : public dark_notify_t
{
public:
    reg_monitor_t();

public: // dark_notify_t
    void register_callback(callback_t callback) final;
    void unregister_callback() final;

    appearance_t query() final;

    void tick() final;
    void run() final;

private:
    void start_event_wait(DWORD ms);

private:
    const util::scoped_hkey_t theme_hkey_;

    callback_t callback_;
    util::scoped_event_t event_;
};

} // namespace siga::dark_notify::impl::windows
