from typing import Optional
import struct


class Message:
    def __init__(self, buffer: Optional[bytes] = None):
        if buffer is None:
            buffer = b""
        self.buffer = buffer
        self.index = 0

    def clear(self):
        self.buffer = b""

    @staticmethod
    def packer(fmt: str, doc: str):
        def pack(self, value):
            self.buffer += struct.pack(fmt, value)

        pack.__doc__ = doc
        return pack

    add_uint8 = packer("B", "Add an unsigned 8-bit integer")
    add_uint16 = packer("H", "Add an unsigned 16-bit integer")
    add_uint32 = packer("I", "Add an unsigned 32-bit integer")
    add_uint64 = packer("Q", "Add an unsigned 64-bit integer")
    add_int8 = packer("b", "Add a signed 8-bit integer")
    add_int16 = packer("h", "Add a signed 16-bit integer")
    add_int32 = packer("i", "Add a signed 32-bit integer")
    add_int64 = packer("q", "Add a signed 64-bit integer")

    def add_string(self, value: str):
        self.add_uint32(len(value))
        self.buffer += value.encode("utf-8")

    def add_bytes(self, value: bytes):
        self.add_uint32(len(value))
        self.buffer += value

    def add_message(self, message: 'Message'):
        self.add_uint32(len(message.buffer))
        self.buffer += message.buffer

    def reset(self):
        self.index = 0

    def _has_remaining(self, size: int):
        return self.index + size <= len(self.buffer)

    @staticmethod
    def unpacker(fmt: str, doc: str):
        def unpack(self):
            size = struct.calcsize(fmt)
            if not self._has_remaining(size):
                return 0
            value = struct.unpack_from(fmt, self.buffer, self.index)[0]
            self.index += size
            return value

        unpack.__doc__ = doc
        return unpack

    read_uint8 = unpacker("B", "Read an unsigned 8-bit integer")
    read_uint16 = unpacker("H", "Read an unsigned 16-bit integer")
    read_uint32 = unpacker("I", "Read an unsigned 32-bit integer")
    read_uint64 = unpacker("Q", "Read an unsigned 64-bit integer")
    read_int8 = unpacker("b", "Read a signed 8-bit integer")
    read_int16 = unpacker("h", "Read a signed 16-bit integer")
    read_int32 = unpacker("i", "Read a signed 32-bit integer")
    read_int64 = unpacker("q", "Read a signed 64-bit integer")

    def read_bytes(self):
        size = self.read_uint32()
        if not self._has_remaining(size):
            return b""
        value = self.buffer[self.index : self.index + size]
        self.index += size
        return value

    def read_string(self):
        return self.read_bytes().decode("utf-8")

    def read_message(self):
        return Message(self.read_bytes())

    def __bytes__(self):
        return self.buffer

    def __repr__(self):
        return (
            f'[{len(self.buffer)} bytes: {" ".join(f"{b:02X}" for b in self.buffer)}]'
        )
