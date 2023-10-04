#pragma once

#include <tuple>
#include <utility>

namespace siga::dark_notify::impl::windows::util {

template<typename Handle, auto DefaultValue, typename Close>
class basic_scoped_handle_t
{
public:
    // `DefaultValue` cannot always be the same type as the `Handle`, because in
    // WinAPI the handle type is `void*`, and the default value for it is `-1`,
    // and we cannot convert `-1` to a pointer in the constexpr context
    static Handle default_value() noexcept {
        return reinterpret_cast<Handle>(DefaultValue);
    }

public:
    explicit basic_scoped_handle_t(
        Handle handle = default_value(),
        Close close = {}
    ) noexcept
        // Note that the usage `std::forward<Close>` allows us to specify
        // `Close` as a reference type, and everything would work correctly
        : state_{handle, std::forward<Close>(close)} {}

    basic_scoped_handle_t(basic_scoped_handle_t &&rhs) noexcept { swap(rhs); }

    ~basic_scoped_handle_t() {
        if(valid()) {
            Close &close = std::get<Close>(state_);
            close(std::get<Handle>(state_));
        }
    }

    basic_scoped_handle_t &operator=(basic_scoped_handle_t &&rhs) noexcept {
        swap(rhs);
        return *this;
    }

public:
    Handle get() const noexcept { return handle_ref(); }
    bool valid() const noexcept { return get() != default_value(); }
    operator bool() const noexcept { return valid(); }

    void reset() noexcept {
        basic_scoped_handle_t that{default_value(), get_close()};
        swap(that);
    }

    void reset(Handle handle) noexcept {
        static_assert(
            std::is_empty_v<Close>,
            "It is not safe to reset the `Handle` if the `Close` is stateful"
        );

        basic_scoped_handle_t that{handle, get_close()};
        swap(that);
    }

    Handle release() noexcept {
        return std::exchange(handle_ref(), default_value());
    }

    Close &get_close() noexcept { return std::get<Close>(state_); }
    const Close &get_close() const noexcept { return std::get<Close>(state_); }

    void swap(basic_scoped_handle_t &that) noexcept {
        std::swap(state_, that.state_);
    }

private:
    Handle &handle_ref() noexcept { return std::get<Handle>(state_); }
    const Handle &handle_ref() const noexcept {
        return std::get<Handle>(state_);
    }

private:
    std::tuple<Handle, Close> state_;
};

template<typename Handle, auto DefaultValue, typename Close>
auto operator<=>(
    const basic_scoped_handle_t<Handle, DefaultValue, Close> &lhs,
    const auto &rhs
) noexcept {
    return lhs.get() <=> rhs;
}

template<typename Handle, auto DefaultValue, typename Close>
auto operator<=>(
    const auto &lhs,
    const basic_scoped_handle_t<Handle, DefaultValue, Close> &rhs
) noexcept {
    return lhs <=> rhs.get();
}

template<typename Handle, auto DefaultValue, typename Close>
auto operator==(
    const basic_scoped_handle_t<Handle, DefaultValue, Close> &lhs,
    std::nullptr_t
) noexcept {
    return lhs.get() == nullptr;
}

template<typename Handle, auto DefaultValue, typename Close>
auto operator==(
    std::nullptr_t,
    const basic_scoped_handle_t<Handle, DefaultValue, Close> &rhs
) noexcept {
    return nullptr == rhs.get();
}

template<typename Handle, auto DefaultValue, typename Close>
auto operator<=>(
    const basic_scoped_handle_t<Handle, DefaultValue, Close> &lhs,
    const basic_scoped_handle_t<Handle, DefaultValue, Close> &rhs
) noexcept {
    return lhs.get() <=> rhs.get();
}

} // namespace siga::dark_notify::impl::windows::util
