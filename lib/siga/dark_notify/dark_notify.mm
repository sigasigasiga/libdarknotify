#include "siga/dark_notify/dark_notify.hpp"

#ifdef __APPLE__
#    include "siga/dark_notify/impl/macos/nsapp.h"
#endif // __APPLE__

namespace siga::dark_notify {

std::unique_ptr<dark_notify_t> make_default_notifier() {
#ifdef __APPLE__
    return std::make_unique<impl::macos::nsapp_t>();
#endif // __APPLE__
}

} // namespace siga::dark_notify
