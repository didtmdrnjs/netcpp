#pragma once

#include <span>
#include <memory>

#include "IpAddress.hpp"
#include "Endpoint.hpp"

#include <mutex>
#include <optional>

namespace net
{
	enum class AddressFamily
	{
		IPv4 = AF_INET,
		IPv6 = AF_INET6
	};

	enum class SocketType
	{
		Stream = SOCK_STREAM,
		Dgram = SOCK_DGRAM
	};

	enum class Protocol
	{
        Ip = IPPROTO_IP,
		Tcp = IPPROTO_TCP,
		Udp = IPPROTO_UDP
	};

	enum class OptionLevel
	{
		IP = SOL_IP,
		IPv6 = SOL_IPV6,
		Socket = SOL_SOCKET,
	};

	enum class OptionName
	{
		// Socket Level
		Linger = SO_LINGER,
		ReuseAddress = SO_REUSEADDR,
		SendBuffer = SO_SNDBUF,
		RecvBuffer = SO_RCVBUF,
		Broadcast = SO_BROADCAST,

        UpdateAcceptContext = SO_UPDATE_ACCEPT_CONTEXT,
        UpdateConnectContext = SO_UPDATE_CONNECT_CONTEXT,

        // IP Level
		TTL = 4,

		// Tcp Level
		NoDelay = TCP_NODELAY
    };

	struct Linger
	{
		bool enabled;
		int time;
	};

    class Context;

	class Socket
    {
	public:
		Socket();
		Socket(Protocol pt);
		Socket(const Socket& sock);
		Socket(Socket&& sock) noexcept;
		~Socket();
	public:
		void close();
        void create(Protocol pt = Protocol::Ip);

		void setHandle(SOCKET s);

		bool bind(Endpoint ep);
		bool listen(int backlog = SOMAXCONN) const;
	public:
		SOCKET getHandle() const;
        std::optional<Endpoint> getRemoteEndpoint() const;
        std::optional<Endpoint> getLocalEndpoint() const;
	public:
		void setRemoteEndpoint(Endpoint ep);
		void setLocalEndpoint(Endpoint ep);
	public:
		void disconnect();
		Socket accept() const;
		bool connect(Endpoint ep);

		bool send(std::span<char> s) const;
		bool send(std::span<char> s, Endpoint target) const;

		int receive(std::span<char> s) const;
		int receive(std::span<char> s, Endpoint target) const;
    public:
        bool disconnect(Context* context) const;
        bool accept(Context *context) const;
        bool connect(Context* context);
        bool send(Context* context) const;
        bool receive(Context* context) const;
    public:
		template<class T>
		bool setSocketOption(OptionLevel level, OptionName name, T value) const
		{
			if (_sock == INVALID_SOCKET)
				return false;
            return setsockopt(_sock,
				static_cast<int>(level),
				static_cast<int>(name),
				reinterpret_cast<const char*>(&value),
				sizeof(T)
			) == 0;
		}

		void setBlocking(bool isBlocking) const;
		void setLinger(Linger linger) const;
		void setBroadcast(bool isBroadcast) const;
		void setReuseAddress(bool isReuseAddr) const;
		void setNoDelay(bool isNoDelay) const;
		void setTTL(int ttl) const;
		void setSendBuffer(int size) const;
		void setReceiveBuffer(int size) const;
		bool isOpen() const;

        void BindEndpoint();
	public:
		Socket& operator=(const Socket& sock);
		Socket& operator=(Socket&& sock) noexcept;
    private:
		std::optional<Endpoint> _remoteEndpoint;
        std::optional<Endpoint> _localEndpoint;
		SOCKET _sock;
	};
}