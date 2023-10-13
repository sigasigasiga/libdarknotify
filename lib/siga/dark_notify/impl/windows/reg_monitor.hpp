#pragma once

#include <siga/dark_notify/dark_notify.hpp>
#include <siga/dark_notify/impl/windows/util/cancellable_event.hpp>
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
    void stop() final;

private:
    using exit_reason_t = util::cancellable_event_t::exit_reason_t;

private:
    exit_reason_t start_event_wait(DWORD ms);

private:
    const util::scoped_hkey_t theme_hkey_;

    callback_t callback_;
    util::cancellable_event_t event_;
};

} // namespace siga::dark_notify::impl::windows
