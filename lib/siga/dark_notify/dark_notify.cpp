#include "siga/dark_notify/dark_notify.hpp"

#include <siga/dark_notify/impl/windows/reg_monitor.hpp>

namespace siga::dark_notify {

std::unique_ptr<dark_notify_t> make_default_notifier() {
    return std::make_unique<impl::windows::reg_monitor_t>();
}

} // namespace siga::dark_notify
