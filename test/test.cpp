#include <iostream>
#include <net/netcpp.hpp>

using namespace net;

int main() {
    Socket sock(Protocol::Tcp);
    Context ctx;
    ctx.completed = [](Context*, bool) {
        std::cout << "Connected";
    };
    ctx.endpoint = Endpoint(IpAddress::Loopback, 1225);
    sock.connect(&ctx);

    while (true) { Sleep(1); }
    return 0;
}