#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <exception>
#include <queue>
#include <string>

#include "Message.hpp"

#ifdef _WIN32
class WSAStartupException : public std::exception {
    public:
        const char* what() const noexcept override {
            return "WSA failed to initialize";
        }
};
#endif

class SocketException : public std::exception {
    public:
        SocketException(const std::string& message) : msg(message) {}
        const char* what() const noexcept override {
            return msg.c_str();
        }

    private:
        std::string msg;
};

class Socket {
    public:
        Socket();
        Socket(const Socket&) = delete;
        ~Socket();

        void sendMessage(const Message&);
        std::queue<Message> pollMessages();

    private:
        int m_socketDescriptor;
};

#endif
