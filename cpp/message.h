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
    Message(std::vector<uint8_t> sourceBuffer);
    ~Message() = default;

    // Write data
    void clear();
    void add(uint8_t byte);
    void add(uint16_t word);
    void add(uint32_t dword);
    void add(uint64_t qword);
    void add(int8_t byte);
    void add(int16_t word);
    void add(int32_t dword);
    void add(int64_t qword);
    void add(std::string str);
    void add(std::vector<uint8_t> bytes);
    void add(MessagePtr message);

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

    std::vector<uint8_t> getBuffer();
    size_t size() const;
    std::string toString() const;

private:
    std::vector<uint8_t> buffer;
    size_t index;
};
