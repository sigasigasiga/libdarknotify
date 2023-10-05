#pragma once

#include <memory>

#include <siga/dark_notify/util/export.hpp>

namespace siga::dark_notify {

class SIGA_DARK_NOTIFY_EXPORT dark_notify_t
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
    // You must unregister the callback if it was ever registered,
    // otherwise it is undefined behaviour
    //
    // Also note that the callback may be triggered
    // more than once with the same value
    //
    // It is unspecified on which thread the callback would be invoked
    virtual void register_callback(callback_t callback) = 0;
    virtual void unregister_callback() = 0;

    virtual appearance_t query() = 0;

    // If the callback was not registered,
    // the behaviour of these methods is undefined
    virtual void tick() = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
};

SIGA_DARK_NOTIFY_EXPORT std::unique_ptr<dark_notify_t> make_default_notifier();

} // namespace siga::dark_notify
