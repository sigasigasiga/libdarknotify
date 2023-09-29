#include <iostream>

#include <siga/dark_notify/dark_notify.hpp>

int main() {
    namespace sdn    = siga::dark_notify;
    using notifier_t = sdn::dark_notify_t;

    auto notifier    = sdn::make_default_notifier();

    notifier->register_callback([](notifier_t::appearance_t app) {
        std::cout << (int)app << std::endl;
    });

    // You can also use `siga::dark_notify::run()` instead
    while(true) {
        notifier->tick();
    }
}
