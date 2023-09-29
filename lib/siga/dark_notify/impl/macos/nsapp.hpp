#pragma once

#include <siga/dark_notify/dark_notify.hpp>

namespace siga::dark_notify::impl::macos {

class nsapp_t : public dark_notify_t
{
public:
    nsapp_t() = default;

public: // dark_notify_t
    appearance_t query() final;
    void register_callback(callback_t cb) final;
    void tick() final;
    void run() final;
};

} // namespace siga::dark_notify::impl::macos
