#include "message.h"
#include <iostream>

Message::Message() : index(0) {
    // buffer.reserve(1024);
}

Message::Message(std::vector<uint8_t> sourceBuffer) : Message() {
    buffer = sourceBuffer;
}

void Message::clear() {
    buffer.clear();
    index = 0;
}

void Message::add(uint8_t byte) {
    buffer.push_back(byte);
}

void Message::add(uint16_t word) {
    buffer.push_back((word >> 0) & 0xFF);
    buffer.push_back((word >> 8) & 0xFF);
}

void Message::add(uint32_t dword) {
    buffer.push_back((dword >> 0) & 0xFF);
    buffer.push_back((dword >> 8) & 0xFF);
    buffer.push_back((dword >> 16) & 0xFF);
    buffer.push_back((dword >> 24) & 0xFF);
}

void Message::add(uint64_t qword) {
    buffer.push_back((qword >> 0) & 0xFF);
    buffer.push_back((qword >> 8) & 0xFF);
    buffer.push_back((qword >> 16) & 0xFF);
    buffer.push_back((qword >> 24) & 0xFF);
    buffer.push_back((qword >> 32) & 0xFF);
    buffer.push_back((qword >> 40) & 0xFF);
    buffer.push_back((qword >> 48) & 0xFF);
    buffer.push_back((qword >> 56) & 0xFF);
}

void Message::add(int8_t byte) {
    add(static_cast<uint8_t>(byte));
}

void Message::add(int16_t word) {
    add(static_cast<uint16_t>(word));
}

void Message::add(int32_t dword) {
    add(static_cast<uint32_t>(dword));
}

void Message::add(int64_t qword) {
    add(static_cast<uint64_t>(qword));
}

void Message::add(std::string str) {
    add((uint32_t)str.length());
    buffer.insert(buffer.end(), str.begin(), str.end());
}

void Message::add(std::vector<uint8_t> bytes) {
    add((uint32_t)bytes.size());
    buffer.insert(buffer.end(), bytes.begin(), bytes.end());
}

void Message::add(MessagePtr message) {
    add(message->getBuffer());
}

void Message::reset() {
    index = 0;
}

uint8_t Message::readUInt8() {
    if (index >= buffer.size()) {
        return 0;
    }
    return buffer[index++];
}

uint16_t Message::readUInt16() {
    if (index + 2 > buffer.size()) {
        return 0;
    }
    uint16_t word = 0;
    word |= buffer[index++];
    word |= buffer[index++] << 8;
    return word;
}

uint32_t Message::readUInt32() {
    if (index + 4 > buffer.size()) {
        return 0;
    }
    uint32_t dword = 0;
    dword |= buffer[index++];
    dword |= buffer[index++] << 8;
    dword |= buffer[index++] << 16;
    dword |= buffer[index++] << 24;
    return dword;
}

uint64_t Message::readUInt64() {
    if (index + 8 > buffer.size()) {
        return 0;
    }
    uint64_t qword = 0;
    qword |= (uint64_t)buffer[index++];
    qword |= (uint64_t)buffer[index++] << 8;
    qword |= (uint64_t)buffer[index++] << 16;
    qword |= (uint64_t)buffer[index++] << 24;
    qword |= (uint64_t)buffer[index++] << 32;
    qword |= (uint64_t)buffer[index++] << 40;
    qword |= (uint64_t)buffer[index++] << 48;
    qword |= (uint64_t)buffer[index++] << 56;
    return qword;
}

int8_t Message::readInt8() {
    return static_cast<int8_t>(readUInt8());
}

int16_t Message::readInt16() {
    return static_cast<int16_t>(readUInt16());
}

int32_t Message::readInt32() {
    return static_cast<int32_t>(readUInt32());
}

int64_t Message::readInt64() {
    return static_cast<int64_t>(readUInt64());
}

std::vector<uint8_t> Message::readBytes() {
    if (index + 4 > buffer.size()) {
        return {};
    }
    uint32_t length = readUInt32();
    if (index + length > buffer.size()) {
        return {};
    }
    std::vector<uint8_t> bytes(buffer.begin() + index, buffer.begin() + index + length);
    index += length;
    return bytes;
}

std::string Message::readString() {
    std::vector<uint8_t> bytes = readBytes();
    std::string str(bytes.begin(), bytes.end());
    return str;
}

MessagePtr Message::readMessage() {
    std::vector<uint8_t> bytes = readBytes();
    return std::make_shared<Message>(bytes);
}

std::vector<uint8_t> Message::getBuffer() {
    return buffer;
}

size_t Message::size() const {
    return buffer.size();
}

std::string Message::toString() const {
    // Print as comma-separated list of hex values (e. g. "00 0F AA 48") without wrapping
    std::string str;
    str += "[" + std::to_string(buffer.size()) + " bytes: ";
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i > 0) {
            str += " ";
        }
        str += "0123456789ABCDEF"[buffer[i] >> 4];
        str += "0123456789ABCDEF"[buffer[i] & 0x0F];
    }
    str += "]";
    return str;
}
