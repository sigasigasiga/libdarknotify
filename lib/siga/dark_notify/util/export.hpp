#pragma once

#ifdef _WIN32
#    define SIGA_DARK_NOTIFY_EXPORT __declspec(dllexport)
#else // _WIN32
#    define SIGA_DARK_NOTIFY_EXPORT
#endif // _WIN32
