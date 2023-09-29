#pragma once

#include <siga/dark_notify/dark_notify.hpp>

#import <Foundation/Foundation.h>

namespace siga::dark_notify::impl::macos {

class nsapp_t : public dark_notify_t
{
public:
    nsapp_t() = default;

public: // dark_notify_t
    void register_callback(callback_t cb) final;
    void unregister_callback() final;

    appearance_t query() final;

    void tick() final;
    void run() final;

private:
    id<NSObject> observer_;
};

} // namespace siga::dark_notify::impl::macos
