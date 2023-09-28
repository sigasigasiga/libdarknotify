#pragma once

namespace siga::dark_notify {

enum class appearance_t
{
    unknown,
    light,
    dark
};

using callback_t = void (*)(appearance_t);

} // namespace siga::dark_notify
