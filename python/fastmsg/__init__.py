from typing import Optional
import struct


class Message:
    def __init__(self, buffer: Optional[bytes] = None):
        if buffer is None:
            buffer = b""
        self._buffer = buffer
        self.index = 0

    def clear(self):
        self._buffer = b""
        return self

    def _packer(fmt: str, doc: str):
        def pack(self, value):
            self._buffer += struct.pack(fmt, value)
            return self

        pack.__doc__ = doc
        return pack

    add_uint8 = _packer("B", "Add an unsigned 8-bit integer")
    add_uint16 = _packer("H", "Add an unsigned 16-bit integer")
    add_uint32 = _packer("I", "Add an unsigned 32-bit integer")
    add_uint64 = _packer("Q", "Add an unsigned 64-bit integer")
    add_int8 = _packer("b", "Add a signed 8-bit integer")
    add_int16 = _packer("h", "Add a signed 16-bit integer")
    add_int32 = _packer("i", "Add a signed 32-bit integer")
    add_int64 = _packer("q", "Add a signed 64-bit integer")

    def add_string(self, value: str):
        self.add_uint32(len(value))
        self._buffer += value.encode("utf-8")
        return self

    def add_bytes(self, value: bytes):
        self.add_uint32(len(value))
        self._buffer += value
        return self

    def add_message(self, message: "Message"):
        buf = message.seal()
        self.add_uint32(len(buf))
        self._buffer += buf
        return self

    def reset(self):
        self.index = 0

    def _has_remaining(self, size: int):
        return self.index + size <= len(self._buffer)

    def _unpacker(fmt: str, doc: str):
        def unpack(self):
            size = struct.calcsize(fmt)
            if not self._has_remaining(size):
                return 0
            value = struct.unpack_from(fmt, self._buffer, self.index)[0]
            self.index += size
            return value

        unpack.__doc__ = doc
        return unpack

    read_uint8 = _unpacker("B", "Read an unsigned 8-bit integer")
    read_uint16 = _unpacker("H", "Read an unsigned 16-bit integer")
    read_uint32 = _unpacker("I", "Read an unsigned 32-bit integer")
    read_uint64 = _unpacker("Q", "Read an unsigned 64-bit integer")
    read_int8 = _unpacker("b", "Read a signed 8-bit integer")
    read_int16 = _unpacker("h", "Read a signed 16-bit integer")
    read_int32 = _unpacker("i", "Read a signed 32-bit integer")
    read_int64 = _unpacker("q", "Read a signed 64-bit integer")

    def read_bytes(self):
        size = self.read_uint32()
        if not self._has_remaining(size):
            return b""
        value = self._buffer[self.index : self.index + size]
        self.index += size
        return value

    def read_string(self):
        return self.read_bytes().decode("utf-8")

    def read_message(self):
        return Message(self.read_bytes())

    def __bytes__(self):
        return self._buffer

    def __repr__(self):
        return (
            f'[{len(self._buffer)} bytes: {" ".join(f"{b:02X}" for b in self._buffer)}]'
        )

    def seal(self):
        return self._buffer
