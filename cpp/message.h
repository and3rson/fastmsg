#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <memory>

class Message;
using MessagePtr = std::shared_ptr<Message>;

class Message {
public:
    Message();
    explicit Message(const std::vector<uint8_t>& sourceBuffer);
    ~Message() = default;

    // Write data
    Message* clear();
    Message* add(uint8_t byte);
    Message* add(uint16_t word);
    Message* add(uint32_t dword);
    Message* add(uint64_t qword);
    Message* add(int8_t byte);
    Message* add(int16_t word);
    Message* add(int32_t dword);
    Message* add(int64_t qword);
    Message* add(std::string str);
    Message* add(std::vector<uint8_t> bytes);
    Message* add(MessagePtr message);

    // Read data
    void reset();
    uint8_t readUInt8();
    uint16_t readUInt16();
    uint32_t readUInt32();
    uint64_t readUInt64();
    int8_t readInt8();
    int16_t readInt16();
    int32_t readInt32();
    int64_t readInt64();
    std::string readString();
    std::vector<uint8_t> readBytes();
    MessagePtr readMessage();

    const std::vector<uint8_t>& seal();
    size_t size() const;
    std::string toString() const;

private:
    std::vector<uint8_t> buffer;
    size_t index;
};
