#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#else
#include <netdb.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif // _WIN32

#include "Socket.hpp"

Socket::Socket() : m_socketDescriptor(-1) {
    auto result = 0;
#ifdef _WIN32
    // Need to use WSA on Windows
    WSADATA wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw WSAStartupException();
    }
#endif // _WIN32

    addrinfo hints = {};
    addrinfo *serverInfo;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    result = getaddrinfo("127.0.0.1", "1337", &hints, &serverInfo);
    if (result != 0) {
        throw SocketException("Unable to get address info");
    }

    m_socketDescriptor = socket(
        serverInfo->ai_family,
        serverInfo->ai_socktype,
        serverInfo->ai_protocol);

    result = connect(m_socketDescriptor, serverInfo->ai_addr, serverInfo->ai_addrlen);
    if (result != 0) {
        throw SocketException("Unable to connect to server");
    }

    freeaddrinfo(serverInfo);
}

Socket::~Socket() {
#ifdef _WIN32
    closesocket(m_socketDescriptor);
    WSACleanup();
#else
    close(m_socketDescriptor);
#endif
}

void Socket::sendMessage(const Message& msg) {
    size_t bytesSent = 0;
    auto buffer = msg.getSerializedMessage();
    while (bytesSent < buffer.size()) {
        // Winsock requires const char* to be sent
        const char* bytes = reinterpret_cast<const char*>(&buffer[bytesSent]);
        auto result = send(m_socketDescriptor, bytes, buffer.size() - bytesSent, 0);
        if (result == -1) {
            throw SocketException("Unable to send message");
        } else {
            bytesSent += result;
        }
    }
}

std::queue<Message> Socket::pollMessages() {
    pollfd events;
    events.fd = m_socketDescriptor;
#ifdef _WIN32
    events.events = POLLRDNORM;
#else
    events.events = POLLIN;
#endif // _WIN32

    std::queue<Message> messages;

    auto result = -1;
    while (result != 0) {
#ifdef _WIN32
        result = WSAPoll(&events, 1, 0);
#else
        result = poll(&events, 1, 0);
#endif // _WIN32
        if (result == -1) {
            throw SocketException("Unable to poll for messages");
        } else if (result > 0) {
            // TODO:
            // - Loop over recv until received correct number of bytes
            // - Add message send queue, and use send in here
            if (events.revents & POLLIN) {
                std::vector<uint8_t> buffer(4);

                // reinterpret_cast for Windows
                auto received = recv(m_socketDescriptor, reinterpret_cast<char*>(&buffer[0]), 4, 0);
                if (received == 0) {
                    throw SocketException("Connection to server has closed");
                }

                uint16_t size = ntohs((buffer[3] << 8) | buffer[2]);
                if (size > 0) {
                    buffer.resize(size + 4);
                    received = recv(m_socketDescriptor, reinterpret_cast<char*>(&buffer[4]), size, 0);
                    if (received == 0) {
                        throw SocketException("Connection to server has closed");
                    }
                }

                auto message = Message(buffer);
                std::cout << std::string(message) << std::endl;
                messages.push(message);
            }
        }
    }

    return messages;
}
