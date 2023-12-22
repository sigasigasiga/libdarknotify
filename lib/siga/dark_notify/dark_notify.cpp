#include "siga/dark_notify/dark_notify.hpp"

#ifdef _WIN32
#    include <siga/dark_notify/impl/windows/reg_monitor.hpp>
#else // _WIN32
#    include <siga/dark_notify/impl/xdg/glib.hpp>
#endif // _WIN32

namespace siga::dark_notify {

std::unique_ptr<dark_notify_t> make_default_notifier() {
#ifdef _WIN32
    return std::make_unique<impl::windows::reg_monitor_t>();
#else  // _WIN32
    return std::make_unique<impl::xdg::glib_t>();
#endif // _WIN32
}

} // namespace siga::dark_notify
