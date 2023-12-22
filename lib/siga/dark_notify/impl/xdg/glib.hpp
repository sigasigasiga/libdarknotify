#pragma once

#include "siga/dark_notify/dark_notify.hpp"

#include <gio/gio.h>
#include <glib-object.h>

namespace siga::dark_notify::impl::xdg {

class glib_t : public dark_notify_t
{
public:
    glib_t();

private: // dark_notify_t
    void register_callback(callback_t callback) final;
    void unregister_callback() final;

    appearance_t query() final;

    void tick() final;
    void run() final;
    void stop() final;

private:
    class g_unref_t
    {
    public:
        void operator()(gpointer ptr) { ::g_object_unref(ptr); }
    };

    template<typename T>
    using ptr_t = std::unique_ptr<T, g_unref_t>;

    class g_main_loop_unref_t
    {
    public:
        void operator()(GMainLoop *ptr) { ::g_main_loop_unref(ptr); }
    };

    using main_loop_t = std::unique_ptr<GMainLoop, g_main_loop_unref_t>;

private:
    main_loop_t loop_;
    ptr_t<GDBusProxy> portal_;

    callback_t cb_;
    gulong signal_id_;
};

} // namespace siga::dark_notify::impl::xdg
