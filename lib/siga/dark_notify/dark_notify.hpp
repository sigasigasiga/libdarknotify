#pragma once

#include <memory>

namespace siga::dark_notify {

class dark_notify_t
{
public:
    enum class appearance_t
    {
        unknown,
        light,
        dark
    };

    using callback_t = void (*)(appearance_t);

public:
    virtual ~dark_notify_t() = default;

public:
    virtual void register_callback(callback_t callback) = 0;
    virtual void unregister_callback()                  = 0;

    virtual appearance_t query()                        = 0;

    virtual void tick()                                 = 0;
    virtual void run()                                  = 0;
};

std::unique_ptr<dark_notify_t> make_default_notifier();

} // namespace siga::dark_notify
