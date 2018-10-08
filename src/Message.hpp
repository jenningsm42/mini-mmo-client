#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <cstdint>
#include <google/protobuf/message.h>
#include "MessageTypes.hpp"

class MessageSizeException : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Message is too long";
        }
};

class Message {
    public:
        Message(const MessageType&, const google::protobuf::Message&);
        Message(const MessageType&);
        Message(const std::vector<uint8_t>&);
        Message(const Message&);

        std::vector<uint8_t> getSerializedMessage() const noexcept;
        void getMessage(google::protobuf::Message&) const noexcept;

        MessageType getType() const noexcept;
        size_t getSize() const noexcept;

        // For logging purposes only - prints human-readable hexadecimal string
        operator std::string() const noexcept;

    private:
        std::vector<uint8_t> m_serializedMessage;
        MessageType m_type;
};

#endif
