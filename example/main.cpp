#include <cassert>
#include <iostream>

#include <siga/dark_notify/dark_notify.hpp>

namespace {

namespace sdn = siga::dark_notify;

using appearance_t = sdn::dark_notify_t::appearance_t;

std::string_view name(appearance_t a) {
    switch(a) {
        case appearance_t::unknown: {
            return "unknown";
        }

        case appearance_t::light: {
            return "light";
        }

        case appearance_t::dark: {
            return "dark";
        }
    }

    throw std::runtime_error{
        "Undefined behaviour in the libdarknotify was triggered? "
        "Normally this shouldn't happen, but if it does, open an issue"
    };
}

std::ostream &operator<<(std::ostream &os, appearance_t a) {
    return os << name(a);
}

} // anonymous namespace

int main() try {
    auto notifier = sdn::make_default_notifier();
    assert(notifier);

    // If you want to check the system theme only once, you can use this method.
    std::cout << "Current system theme is " << notifier->query() << std::endl;

    // If you want to receive updates about the system theme change,
    // you should set the callback and run the event loop
    notifier->register_callback([](sdn::dark_notify_t::appearance_t app) {
        std::cout << "The system theme has changed to " << app << std::endl;
    });

    // If you want to integrate `notifier`'s event loop into your existing one,
    // use `notifier->tick()`
    notifier->run();
} catch(const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
} catch(...) {
    std::cerr << "Unexpected error";
    return 2;
}
