#include "siga/dark_notify/impl/xdg/glib.hpp"

#include <cassert>
#include <stdexcept>

namespace siga::dark_notify::impl::xdg {

namespace {

using namespace std::literals;

enum class color_scheme_t : guint32
{
    no_preference,
    prefers_dark,
    prefers_light
};

using appearance_t = glib_t::appearance_t;

appearance_t to_appearance(color_scheme_t color_scheme) {
    switch(color_scheme) {
        case color_scheme_t::no_preference: {
            return appearance_t::unknown; // FIXME: or is it?
        }

        case color_scheme_t::prefers_dark: {
            return appearance_t::dark;
        }

        case color_scheme_t::prefers_light: {
            return appearance_t::light;
        }
    }

    return appearance_t::unknown;
}

void on_setting_changed(
    GDBusProxy * /* proxy */,
    char * /* sender_name */,
    char *signal_name,
    GVariant *parameters,
    gpointer user_data
) {
    if(signal_name != "SettingChanged"sv) {
        return;
    }

    const char *ns;
    const char *key;
    g_autoptr(GVariant) var = NULL;
    ::g_variant_get(parameters, "(&s&sv)", &ns, &key, &var);

    if(ns == "org.freedesktop.appearance"sv && key == "color-scheme"sv) {
        color_scheme_t cs;
        ::g_variant_get(var, "u", &cs);
        auto cb_ptr = static_cast<glib_t::callback_t *>(user_data);
        (*cb_ptr)(to_appearance(cs));
    }
}

} // anonymous namespace

// dark_notify_t
glib_t::glib_t() {
    g_autoptr(GMainLoop) loop = ::g_main_loop_new(nullptr, false);
    if(!loop) {
        throw std::bad_alloc{};
    }

    g_autoptr(GDBusProxy) portal{::g_dbus_proxy_new_for_bus_sync(
        G_BUS_TYPE_SESSION,
        G_DBUS_PROXY_FLAGS_NONE,
        nullptr,
        "org.freedesktop.portal.Desktop",
        "/org/freedesktop/portal/desktop",
        "org.freedesktop.portal.Settings",
        nullptr,
        nullptr // FIXME: pass a `GError` object here
    )};

    if(!portal) {
        // FIXME: add an error description here
        throw std::runtime_error{"Cannot connect to the portal: "s};
    }

    loop_.reset(loop);
    portal_.reset(portal);
}

void glib_t::register_callback(callback_t callback) {
    constexpr std::string_view signal =
#if GLIB_CHECK_VERSION(2, 72, 0)
        "g-signal::SettingChanged"
#else
        "g-signal"
#endif
        ;

    assert(portal_);
    cb_ = std::move(callback);
    signal_id_ = g_signal_connect(
        portal_.get(),
        signal.data(),
        G_CALLBACK(on_setting_changed),
        &cb_
    );
}

void glib_t::unregister_callback() {
    cb_ = nullptr;
    ::g_signal_handler_disconnect(portal_.get(), signal_id_);
}

appearance_t glib_t::query() {
    constexpr gint dbus_timeout_ms = 1000;

    // FIXME: use `g_autoptr`?
    GVariant *const call =
        ::g_variant_new("(ss)", "org.freedesktop.appearance", "color-scheme");
    if(!call) {
        throw std::bad_alloc{};
    }

    g_autoptr(GVariant) result = ::g_dbus_proxy_call_sync(
        portal_.get(),
        "Read",
        call,
        G_DBUS_CALL_FLAGS_NONE,
        dbus_timeout_ms,
        nullptr,
        nullptr
    );

    if(!result) {
        return appearance_t::unknown; // FIXME: or is it?
    }

    g_autoptr(GVariant) outer = nullptr;
    ::g_variant_get(result, "(v)", &outer);

    g_autoptr(GVariant) inner = nullptr;
    ::g_variant_get(outer, "v", &inner);
    const auto scheme =
        static_cast<color_scheme_t>(::g_variant_get_uint32(inner));

    return to_appearance(scheme);
}

void glib_t::tick() {
    auto *const ctx = ::g_main_loop_get_context(loop_.get());
    ::g_main_context_iteration(ctx, false);
}

void glib_t::run() {
    ::g_main_loop_run(loop_.get());
}

void glib_t::stop() {
    ::g_main_loop_quit(loop_.get());
}

} // namespace siga::dark_notify::impl::xdg
