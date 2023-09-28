#include <iostream>

#include <siga/dark_notify/dark_notify.hpp>

int main() {
    namespace sdn = siga::dark_notify;
    sdn::register_callback([](sdn::appearance_t app) {
        std::cout << (int)app << std::endl;
    });

    // You can also use `siga::dark_notify::run()` instead
    while(true) {
        sdn::tick();
    }
}
