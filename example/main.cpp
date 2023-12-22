#include <cassert>
#include <cstdint>
#include <iostream>

#include <siga/dark_notify/dark_notify.hpp>

namespace {

namespace sdn = siga::dark_notify;

std::string_view name(sdn::dark_notify_t::appearance_t a) {
    using appearance_t = sdn::dark_notify_t::appearance_t;

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
        "Normally this shouldn't happen, but if it does please open an issue"
    };
}

std::ostream &operator<<(std::ostream &os, sdn::dark_notify_t::appearance_t a) {
    return os << name(a);
}

class notifier_callback_t
{
public:
    notifier_callback_t(sdn::dark_notify_t &notifier, std::uintmax_t count)
        : notifier_{&notifier}
        , count_{count} {}

public:
    void operator()(sdn::dark_notify_t::appearance_t appearance) {
        std::cout << "The system theme has changed to " << appearance
                  << std::endl;

        if(--count_ == 0) {
            std::cerr << "The notification count was withdrawn. Exiting...";
            // The main loop of the notifier can be stopped like so
            notifier_->stop();
        }
    }

private:
    sdn::dark_notify_t *notifier_;
    std::uintmax_t count_;
};

} // anonymous namespace

int main() try {
    auto notifier = sdn::make_default_notifier();
    assert(notifier);

    // If you want to check the system theme only once, you can use this method.
    std::cout << "Current system theme is " << notifier->query() << std::endl;

    // If you want to receive updates about the system theme change,
    // you should set the callback and run the event loop
    notifier->register_callback(notifier_callback_t{*notifier, 3});

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
