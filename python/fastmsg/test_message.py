import unittest
from copy import copy

from . import Message


class TestMessage(unittest.TestCase):

    def test_normal(self):
        msg = Message()
        msg.add_string("Hello")
        msg.add_uint16(42)
        msg.add_bytes(bytes([34, 13, 37]))

        msg_nested = Message()
        msg_nested.add_string("Nested")
        msg_nested.add_int64(-42069)
        msg.add_message(msg_nested)

        if len(msg.buffer) != 40:
            self.fail(f"Expected length 40, got {len(msg.buffer)}")

        buffer = copy(msg.buffer)

        deserialized = Message(buffer)

        s = deserialized.read_string()
        if s != "Hello":
            self.fail(f"Expected Hello, got {s}")

        u = deserialized.read_uint16()
        if u != 42:
            self.fail(f"Expected 42, got {u}")

        b = deserialized.read_bytes()
        if len(b) != 3:
            self.fail(f"Expected length 3, got {len(b)}")

        if b != bytes([34, 13, 37]):
            self.fail(f"Expected [34, 13, 37], got {b}")

        nested = deserialized.read_message()

        s = nested.read_string()
        if s != "Nested":
            self.fail(f"Expected Nested, got {s}")

        i = nested.read_int64()
        if i != -42069:
            self.fail(f"Expected -42069, got {i}")

    def test_broken_length(self):
        msg = Message(bytes([0xFF, 0xFF, 0xFF, 0xFF, 0x01]))

        u = msg.read_uint32()
        if u != 0xFFFFFFFF:
            self.fail(f"Expected 0xFFFFFFFF, got 0x{u:08X}")

        msg.reset()

        b = msg.read_bytes()
        if b != b'':
            self.fail(f"Expected None, got {b}")

        msg.reset()

        s = msg.read_string()
        if s != "":
            self.fail(f"Expected empty string, got {s}")

        msg.reset()

        msg = msg.read_message()
        if len(msg.buffer) != 0:
            self.fail(f"Expected length 0, got {len(msg.buffer)}")

    def test_out_of_bounds(self):
        msg = Message(bytes([0x01]))

        u = msg.read_uint32()
        if u != 0:
            self.fail(f"Expected 0, got {u}")

        msg.reset()

        b = msg.read_bytes()
        if b != b'':
            self.fail(f"Expected None, got {b}")

        msg.reset()

        s = msg.read_string()
        if s != "":
            self.fail(f"Expected empty string, got {s}")

        msg.reset()

        msg = msg.read_message()
        if len(msg.buffer) != 0:
            self.fail(f"Expected length 0, got {len(msg.buffer)}")


if __name__ == "__main__":
    unittest.main()
