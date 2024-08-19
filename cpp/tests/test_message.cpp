#include <iostream>

#include "message.h"

#define ASSERT_EQ(a, b)                                                          \
    {                                                                            \
        auto a_ = a;                                                             \
        if (a_ != b) {                                                           \
            std::cerr << "Assertion failed: " << a_ << " != " << b << std::endl; \
            exit(1);                                                             \
        } else {                                                                 \
            std::cout << "Assertion passed: " << a_ << " == " << b << std::endl; \
        }                                                                        \
    }

void test_normal() {
    // Create a message
    MessagePtr msg = std::make_shared<Message>();
    msg->add("Hello")->add(static_cast<uint16_t>(42))->add(std::vector<uint8_t>{34, 13, 37});

    MessagePtr msgNested = std::make_shared<Message>();
    msgNested->add("Nested");
    msgNested->add(static_cast<int64_t>(-42069));
    msg->add(msgNested);

    std::cout << msg->toString() << std::endl;

    // Serialize the message
    std::vector<uint8_t> buffer = msg->seal();
    ASSERT_EQ(buffer.size(), 40);

    // Deserialize the message
    MessagePtr deserialized = std::make_shared<Message>(buffer);
    ASSERT_EQ(deserialized->readString(), "Hello");
    ASSERT_EQ(deserialized->readUInt16(), 42);
    std::vector<uint8_t> vec = deserialized->readBytes();
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(vec[0], 34);
    MessagePtr deserializedNested = deserialized->readMessage();
    ASSERT_EQ(deserializedNested->readString(), "Nested");
    ASSERT_EQ(deserializedNested->readInt64(), -42069);
}

void test_broken_length() {
    MessagePtr msg = std::make_shared<Message>(std::vector<uint8_t>{0xFF, 0xFF, 0xFF, 0xFF, 0x01});
    ASSERT_EQ(msg->readUInt32(), 0xFFFFFFFF);
    msg->reset();
    ASSERT_EQ(msg->readBytes().size(), 0);
    msg->reset();
    ASSERT_EQ(msg->readString(), "");
    msg->reset();
    ASSERT_EQ(msg->readMessage()->size(), 0);
}

void test_out_of_bounds() {
    MessagePtr msg = std::make_shared<Message>(std::vector<uint8_t>{0x01});
    ASSERT_EQ(msg->readUInt32(), 0);
    msg->reset();
    ASSERT_EQ(msg->readBytes().size(), 0);
    msg->reset();
    ASSERT_EQ(msg->readString(), "");
    msg->reset();
    ASSERT_EQ(msg->readMessage()->size(), 0);
}

int main(int argc, char** argv) {
    test_normal();
    test_broken_length();
    test_out_of_bounds();
}
