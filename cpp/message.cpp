#include "message.h"
#include <vector>

Message::Message() : index(0) {
}

Message::Message(const std::vector<uint8_t>& sourceBuffer) : Message() {
    buffer = sourceBuffer;
}

Message* Message::clear() {
    buffer.clear();
    index = 0;
    return this;
}

Message* Message::add(uint8_t byte) {
    buffer.push_back(byte);
    return this;
}

Message* Message::add(uint16_t word) {
    buffer.push_back((word >> 0) & 0xFF);
    buffer.push_back((word >> 8) & 0xFF);
    return this;
}

Message* Message::add(uint32_t dword) {
    buffer.push_back((dword >> 0) & 0xFF);
    buffer.push_back((dword >> 8) & 0xFF);
    buffer.push_back((dword >> 16) & 0xFF);
    buffer.push_back((dword >> 24) & 0xFF);
    return this;
}

Message* Message::add(uint64_t qword) {
    buffer.push_back((qword >> 0) & 0xFF);
    buffer.push_back((qword >> 8) & 0xFF);
    buffer.push_back((qword >> 16) & 0xFF);
    buffer.push_back((qword >> 24) & 0xFF);
    buffer.push_back((qword >> 32) & 0xFF);
    buffer.push_back((qword >> 40) & 0xFF);
    buffer.push_back((qword >> 48) & 0xFF);
    buffer.push_back((qword >> 56) & 0xFF);
    return this;
}

Message* Message::add(int8_t byte) {
    return add(static_cast<uint8_t>(byte));
}

Message* Message::add(int16_t word) {
    return add(static_cast<uint16_t>(word));
}

Message* Message::add(int32_t dword) {
    return add(static_cast<uint32_t>(dword));
}

Message* Message::add(int64_t qword) {
    return add(static_cast<uint64_t>(qword));
}

Message* Message::add(const std::string& str) {
    add((uint32_t)str.length());
    buffer.insert(buffer.end(), str.begin(), str.end());
    return this;
}

// Helper function to encode a single wide character to UTF-8
void encodeUtf8Char(std::vector<uint8_t>& byteArray, wchar_t wc16) {
    uint32_t wc = static_cast<uint32_t>(wc16);
    if (wc <= 0x7F) {
        // 1-byte character (ASCII)
        byteArray.push_back(static_cast<uint8_t>(wc));
    } else if (wc <= 0x7FF) {
        // 2-byte character
        byteArray.push_back(static_cast<uint8_t>(0xC0 | (wc >> 6)));
        byteArray.push_back(static_cast<uint8_t>(0x80 | (wc & 0x3F)));
    } else if (wc <= 0xFFFF) {
        // 3-byte character
        byteArray.push_back(static_cast<uint8_t>(0xE0 | (wc >> 12)));
        byteArray.push_back(static_cast<uint8_t>(0x80 | ((wc >> 6) & 0x3F)));
        byteArray.push_back(static_cast<uint8_t>(0x80 | (wc & 0x3F)));
    } else if (wc <= 0x10FFFF) {
        // 4-byte character
        byteArray.push_back(static_cast<uint8_t>(0xF0 | (wc >> 18)));
        byteArray.push_back(static_cast<uint8_t>(0x80 | ((wc >> 12) & 0x3F)));
        byteArray.push_back(static_cast<uint8_t>(0x80 | ((wc >> 6) & 0x3F)));
        byteArray.push_back(static_cast<uint8_t>(0x80 | (wc & 0x3F)));
    }
}

Message* Message::add(const std::wstring& str) {
    std::vector<uint8_t> bytes;
    for (wchar_t c : str) {
        encodeUtf8Char(bytes, c);
    }
    add(bytes);
    return this;
}

Message* Message::add(std::vector<uint8_t> bytes) {
    add((uint32_t)bytes.size());
    buffer.insert(buffer.end(), bytes.begin(), bytes.end());
    return this;
}

Message* Message::add(MessagePtr message) {
    add(message->seal());
    return this;
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

wchar_t decodeUtf8Char(const std::vector<uint8_t>& utf8Bytes, size_t& index) {
    uint8_t byte = utf8Bytes[index];
    wchar_t wc = 0;

    if (byte <= 0x7F) {
        // 1-byte character (ASCII)
        wc = byte;
    } else if ((byte & 0xE0) == 0xC0) {
        // 2-byte character
        if (index + 1 >= utf8Bytes.size()) {
            return 0;
        };
        wc = ((byte & 0x1F) << 6) | (utf8Bytes[++index] & 0x3F);
    } else if ((byte & 0xF0) == 0xE0) {
        // 3-byte character
        if (index + 2 >= utf8Bytes.size()) {
            return 0;
        }
        wc = ((byte & 0x0F) << 12) | ((utf8Bytes[index + 1] & 0x3F) << 6) | (utf8Bytes[index + 2] & 0x3F);
        index += 2;
    } else if ((byte & 0xF8) == 0xF0) {
        // 4-byte character
        if (index + 3 >= utf8Bytes.size()) {
            return 0;
        }
        wc = ((byte & 0x07) << 18) | ((utf8Bytes[index + 1] & 0x3F) << 12) | ((utf8Bytes[index + 2] & 0x3F) << 6) |
             (utf8Bytes[index + 3] & 0x3F);
        index += 3;
    } else {
        return 0;
    }

    return wc;
}

std::wstring Message::readWString() {
    std::wstring str;
    std::vector<uint8_t> bytes = readBytes();
    size_t i = 0;
    while (i < bytes.size()) {
        wchar_t c = decodeUtf8Char(bytes, index);
        if (c) {
            str.push_back(c);
        }
        i++;
    }
    return str;
}

MessagePtr Message::readMessage() {
    std::vector<uint8_t> bytes = readBytes();
    return std::make_shared<Message>(bytes);
}

const std::vector<uint8_t>& Message::seal() {
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
