#include <iostream>
#include <net/netcpp.hpp>

using namespace net;

int main() {
    Socket sock(Protocol::Tcp);
    Context ctx;
    ctx.completed = [](Context*, bool) {
        std::cout << "Connected";
    };
    sock.bind(Endpoint(IpAddress::Loopback, 1225));
    sock.listen();

    while (true)
    {
        char buf[100] = "";
        auto client = sock.accept();
        client.receive(buf);
        std::cout << buf << '\n';
    }
    return 0;
}