#include "Message.hpp"

#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

#include <iomanip>
#include <sstream>

Message::Message(const MessageType& type, const google::protobuf::Message& msg) : m_type(type) {
    auto size = msg.ByteSizeLong();
    if (size > UINT16_MAX) {
        throw MessageSizeException();
    }

    m_serializedMessage.resize(size + 4);
    msg.SerializeToArray(&m_serializedMessage[4], size);

    uint16_t messageType = htons(static_cast<uint16_t>(type));
    uint16_t messageSize = htons(static_cast<uint16_t>(size));

    m_serializedMessage[0] = messageType & 0xff;
    m_serializedMessage[1] = (messageType >> 8) & 0xff;

    m_serializedMessage[2] = messageSize & 0xff;
    m_serializedMessage[3] = (messageSize >> 8) & 0xff;
}

Message::Message(const MessageType& type) : m_type(type) {
    m_serializedMessage.resize(4);

    uint16_t messageType = htons(static_cast<uint16_t>(type));

    m_serializedMessage[0] = messageType & 0xff;
    m_serializedMessage[1] = (messageType >> 8) & 0xff;

    m_serializedMessage[2] = 0;
    m_serializedMessage[3] = 0;
}

Message::Message(const std::vector<uint8_t>& serializedMessage) :
    m_serializedMessage(serializedMessage) {
    uint16_t messageType = (m_serializedMessage[1] << 8) | m_serializedMessage[0];
    m_type = static_cast<MessageType>(ntohs(messageType));
}

Message::Message(const Message& other) {
    m_serializedMessage = other.m_serializedMessage;
    m_type = other.m_type;
}

std::vector<uint8_t> Message::getSerializedMessage() const noexcept {
    return m_serializedMessage;
}

void Message::getMessage(google::protobuf::Message& msg) const noexcept {
    msg.ParseFromArray(&m_serializedMessage[4], m_serializedMessage.size() - 4);
}

MessageType Message::getType() const noexcept {
    return m_type;
}

size_t Message::getSize() const noexcept {
    return m_serializedMessage.size();
}

Message::operator std::string() const noexcept {
    std::stringstream buffer;
    buffer << std::hex << std::setfill('0') << std::setw(2);
    for (auto& b : m_serializedMessage) {
        buffer << static_cast<int>(b) << " ";
    }
    return buffer.str();
}
