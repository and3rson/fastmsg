# fastmsg

fastmsg is a set of cross-language serialization libraries.

Currently, the following languages are supported:

- [C++](./cpp)
- [Go](./go)
- [Python](./python)

## Contents

 * [Motivation](#motivation)
 * [Payload format](#payload-format)
 * [Example usage](#example-usage)
    * [C++](#c)
    * [Go](#go)
    * [Python](#python)
 * [Running tests](#running-tests)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->

## Motivation

Why another serialization library? Let me explain why not protobuf, flatbuffers, msgpack, etc.

- 🟢 **Zero dependencies.**<br />
  fastmsg has no dependencies other than the standard library.
- 🟢 **No schema.**<br />
  fastmsg is schema-less. You don't have to define a schema before serializing data.
  This is useful when you want to serialize simple data structures without the overhead of defining a schema.
- 🟢 **Smart-pointer-friendly.**<br />
  `MessagePtr` is a smart pointer to Message that manages the underlying buffer's lifetime.
- 🟢 **Message owns the buffer.**<br />
  `MessagePtr` keeps its own buffer, so you don't have to worry about the buffer's lifetime.
  For instance, returning a `MessagePtr` object from a function is perfectly fine.
  This is not the case with other libraries, where you have to manage the buffer's lifetime yourself.
  Don't get me wrong, I like protobuf and flatbuffers, but they are too verbose when it comes to serializing simple data structures.
- 🟢 **Tested with GCC & MinGW.**<br />
  fastmsg is tested with GCC and MinGW, so you can use it on Windows without any issues.
- 🟢 **Memory-safe.**
  Common mistakes like buffer overflows are checked at runtime. This is especially useful when deserializing malformed messages or data from untrusted sources.

Now, here are some important points to consider before using fastmsg:

- 🔴 **No schema.**<br />
  Again, if you need a schema, fastmsg is not for you.
- 🔴 **No custom types.**<br />
  fastmsg only supports a limited set of native types: integers, strings, byte vectors, and nested messages.
  For instance, even dictionaries are not supported! Why? Because you can easily serialize a dictionary by serializing its keys and values separately. It's all up to you!
- 🔴 **No error handling.**<br />
  fastmsg does not provide error handling. If an error occurs, a default value is returned (e.g., 0 for integers, empty string for strings, etc.).
- 🔴 **No type safety.**<br />
  fastmsg is not type-safe. You have to know the type of the data you are reading.
- 🔴 **No backward compatibility.**<br />
  fastmsg is not designed for backward compatibility. If you need backward compatibility, you should consider using other libraries.

Why not X?

- ❔ **protobuf**: depends on absl, which is a pain to link statically.
- ❔ **flatbuffers**: does not manage the buffer's lifetime, requiring you to manage it yourself by writing more boilerplate code.
- ❔ **msgpack**: depends on boost, and the code looks too verbose for simple data structures.

## Payload format

Here's an example of a collection that contains a string, an integer, a byte vector, and a nested message.

Sample data:

```
05 00 00 00 48 65 6C 6C 6F 2A 00 03 00 00 00 22 0D 25 12 00 00 00 06 00 00 00 4E 65 73 74 65 64 AB 5B FF FF FF FF FF FF
```

```python
[
  0x05, 0x00, 0x00, 0x00,                           # String length (5)
  0x48, 0x65, 0x6C, 0x6C, 0x6F,                     # String ("Hello")
  0x2A,                                             # 16-bit unsigned int (42)
  0x03, 0x00, 0x00, 0x00,                           # Byte vector length (3)
  0x22, 0x0D, 0x25,                                 # Byte vector ([34, 13, 37])
  0x12, 0x00, 0x00, 0x00,                           # Nested message length (18)
  0x06, 0x00, 0x00, 0x00,                           # String length (6)
  0x4E, 0x65, 0x73, 0x74, 0x65, 0x64,               # String ("Nested")
  0xAB, 0x5B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF    # 64-bit int (-42069)
]
```

## Example usage

### C++

Installation: simply copy [message.h](./cpp/message.h) and [message.cpp](./cpp/message.cpp) to your project.

Serialization:

```cpp
#include <fastmsg/message.h>

// ...

MessagePtr msg = std::make_shared<Message>();

msg->add("Hello");
msg->add(static_cast<uint16_t>(42));
msg->add(std::vector<uint8_t>{34, 13, 37});

MessagePtr msgNested = std::make_shared<Message>();
msgNested->add("Nested");
msgNested->add(static_cast<int64_t>(-42069));
msg->add(msgNested);

std::vector<uint8_t> buffer = msg->getBuffer();

// Send buffer over network, write to file, etc.
}
```

Deserialization:

```cpp
#include <fastmsg/message.h>

// ...

MessagePtr msg = std::make_shared<Message>(buffer);
std::string str = msg->readString();
uint16_t num = msg->readUInt16();
std::vector<uint8_t> vec = msg->readBytes();

MessagePtr msgNested = msg->readMessage();
std::string strNested = msgNested->readString();
int64_t numNested = msgNested->readInt64();
```

### Go

Installation:

```sh
go get github.com/and3rson/fastmsg/go
```

Serialization:

```go
import "github.com/and3rson/fastmsg/go"

msg := NewMessage()
msg.AddString("Hello")
msg.AddUInt16(42)
msg.AddBytes([]byte{34, 13, 37})

msgNested := NewMessage()
msgNested.AddString("Nested")
msgNested.AddInt64(-42069)
msg.AddMessage(msgNested)

buffer := msg.Buffer
```

Deserialization:

```go
import "github.com/and3rson/fastmsg/go"

msg := NewMessageFromBuffer(buffer)
s := msg.ReadString()
u := msg.ReadUInt16()
b := msg.ReadBytes()

msgNested := msg.ReadMessage()
sNested := msgNested.ReadString()
iNested := msgNested.ReadInt64()
```

### Python

Installation:

```sh
pip install fastmsg
```

Serialization:

```python
from fastmsg import Message

msg = Message()
msg.add_string("Hello")
msg.add_uint16(42)
msg.add_bytes(bytes([34, 13, 37]))

msg_nested = Message()
msg_nested.add_string("Nested")
msg_nested.add_int64(-42069)
msg.add_message(msg_nested)

buffer = msg.buffer
```

Deserialization:

```python
from fastmsg import Message

msg = Message(buffer)
s = msg.read_string()
u = msg.read_uint16()
b = msg.read_bytes()

msg_nested = msg.read_message()
s_nested = msg_nested.read_string()
i_nested = msg_nested.read_int64()
```

## Running tests

```sh
make test
```
