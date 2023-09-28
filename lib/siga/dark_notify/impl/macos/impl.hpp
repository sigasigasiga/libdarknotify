#pragma once

#include <siga/dark_notify/appearance.hpp>

namespace siga::dark_notify::impl::macos {

appearance_t query();
void register_callback(callback_t cb);
void tick();
void run();

} // namespace siga::dark_notify::impl::macos
