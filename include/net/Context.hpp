#pragma once

#include <tbb/concurrent_vector.h>

#include <memory>
#include <atomic>
#include <functional>

#include "Socket.hpp"

namespace net
{
	enum class ContextType
	{
        None,
		Accept,
		Connect,
		Disconnect,
		Send,
		Receive
	};

    class Context : private OVERLAPPED
	{
        friend class Socket;
        friend class IoSystem;

        using Callback = std::function<void(Context*, bool)>;
    public:
		Context();
        ~Context();
    public:
        Callback completed = [](Context*, bool) {};
    public:
        std::unique_ptr<Socket> acceptSocket;
        std::optional<Endpoint> endpoint;
        std::span<char> buffer {};
        tbb::concurrent_vector<std::span<char>> sendBuffer;
        std::atomic<u_long> length = 0;
        void* token;
    private:
        void init();
    private:
        ContextType _contextType;
    };
}

