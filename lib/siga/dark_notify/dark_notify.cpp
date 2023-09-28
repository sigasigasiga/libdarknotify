#include "siga/dark_notify/dark_notify.hpp"

#include "siga/dark_notify/impl/macos/impl.hpp"

namespace siga::dark_notify {

appearance_t query() {
    return impl::macos::query();
}

void register_callback(callback_t callback) {
    return impl::macos::register_callback(callback);
}

void tick() {
    return impl::macos::tick();
}

void run() {
    return impl::macos::run();
}

} // namespace siga::dark_notify
