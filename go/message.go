// Go implementation of the fastmsg Message class
package main

import "encoding/binary"

// Message represents a packed data buffer.
// It is used for both serializing and deserializing data.
// All numbers are stored in little-endian byte order.
type Message struct {
	buffer []byte
	index  int
}

// NewMessage creates a new Message instance for serializing data.
func NewMessage() *Message {
	return &Message{
		buffer: make([]byte, 0),
		index:  0,
	}
}

// NewMessageFromBuffer creates a new Message instance from a buffer for deserializing data.
func NewMessageFromBuffer(buffer []byte) *Message {
	return &Message{
		buffer: buffer,
		index:  0,
	}
}

// Clear clears the message buffer.
func (m *Message) Clear() *Message {
	m.buffer = make([]byte, 0)

	return m
}

// AddUInt8 adds a uint8 to the message buffer.
func (m *Message) AddUInt8(b uint8) *Message {
	m.buffer = append(m.buffer, b)

	return m
}

// AddUInt16 adds a uint16 to the message buffer.
func (m *Message) AddUInt16(value uint16) *Message {
	buf := make([]byte, 2)
	binary.LittleEndian.PutUint16(buf, value)
	m.buffer = append(m.buffer, buf...)

	return m
}

// AddUInt32 adds a uint32 to the message buffer.
func (m *Message) AddUInt32(value uint32) *Message {
	buf := make([]byte, 4)
	binary.LittleEndian.PutUint32(buf, value)
	m.buffer = append(m.buffer, buf...)

	return m
}

// AddUInt64 adds a uint64 to the message buffer.
func (m *Message) AddUInt64(value uint64) *Message {
	buf := make([]byte, 8)
	binary.LittleEndian.PutUint64(buf, value)
	m.buffer = append(m.buffer, buf...)

	return m
}

// AddInt8 adds an int8 to the message buffer.
func (m *Message) AddInt8(b int8) *Message {
	m.buffer = append(m.buffer, byte(b))

	return m
}

// AddInt16 adds an int16 to the message buffer.
func (m *Message) AddInt16(value int16) *Message {
	buf := make([]byte, 2)
	binary.LittleEndian.PutUint16(buf, uint16(value))
	m.buffer = append(m.buffer, buf...)

	return m
}

// AddInt32 adds an int32 to the message buffer.
func (m *Message) AddInt32(value int32) *Message {
	buf := make([]byte, 4)
	binary.LittleEndian.PutUint32(buf, uint32(value))
	m.buffer = append(m.buffer, buf...)

	return m
}

// AddInt64 adds an int64 to the message buffer.
func (m *Message) AddInt64(value int64) *Message {
	buf := make([]byte, 8)
	binary.LittleEndian.PutUint64(buf, uint64(value))
	m.buffer = append(m.buffer, buf...)

	return m
}

// AddString adds a string to the message buffer.
// The string is prefixed with its length as a uint32.
func (m *Message) AddString(str string) *Message {
	m.AddUInt32(uint32(len(str)))
	m.buffer = append(m.buffer, []byte(str)...)

	return m
}

// AddBytes adds a byte slice to the message buffer.
// The byte slice is prefixed with its length as a uint32.
func (m *Message) AddBytes(bytes []byte) *Message {
	m.AddUInt32(uint32(len(bytes)))
	m.buffer = append(m.buffer, bytes...)

	return m
}

// AddMessage adds a Message to the message buffer.
// The Message is prefixed with its length as a uint32.
func (m *Message) AddMessage(message *Message) *Message {
	m.AddBytes(message.Seal())

	return m
}

// Reset sets deserialization cursor to the beginning of the message buffer.
// All Read* methods advance the cursor, so Reset can be used to read the message again.
func (m *Message) Reset() {
	m.index = 0
}

// ReadUInt8 reads a uint8 from the message buffer and advances the cursor.
// If there's not enough data to read, it returns 0.
func (m *Message) ReadUInt8() uint8 {
	if m.index >= len(m.buffer) {
		return 0
	}

	b := m.buffer[m.index]

	m.index++

	return b
}

// ReadUInt16 reads a uint16 from the message buffer and advances the cursor.
// If there's not enough data to read, it returns 0.
func (m *Message) ReadUInt16() uint16 {
	if m.index+2 > len(m.buffer) {
		return 0
	}

	value := binary.LittleEndian.Uint16(m.buffer[m.index : m.index+2])

	m.index += 2

	return value
}

// ReadUInt32 reads a uint32 from the message buffer and advances the cursor.
// If there's not enough data to read, it returns 0.
func (m *Message) ReadUInt32() uint32 {
	if m.index+4 > len(m.buffer) {
		return 0
	}

	value := binary.LittleEndian.Uint32(m.buffer[m.index : m.index+4])

	m.index += 4

	return value
}

// ReadUInt64 reads a uint64 from the message buffer and advances the cursor.
// If there's not enough data to read, it returns 0.
func (m *Message) ReadUInt64() uint64 {
	if m.index+8 > len(m.buffer) {
		return 0
	}

	value := binary.LittleEndian.Uint64(m.buffer[m.index : m.index+8])

	m.index += 8

	return value
}

// ReadInt8 reads an int8 from the message buffer and advances the cursor.
// If there's not enough data to read, it returns 0.
func (m *Message) ReadInt8() int8 {
	return int8(m.ReadUInt8())
}

// ReadInt16 reads an int16 from the message buffer and advances the cursor.
// If there's not enough data to read, it returns 0.
func (m *Message) ReadInt16() int16 {
	return int16(m.ReadUInt16())
}

// ReadInt32 reads an int32 from the message buffer and advances the cursor.
// If there's not enough data to read, it returns 0.
func (m *Message) ReadInt32() int32 {
	return int32(m.ReadUInt32())
}

// ReadInt64 reads an int64 from the message buffer and advances the cursor.
// If there's not enough data to read, it returns 0.
func (m *Message) ReadInt64() int64 {
	return int64(m.ReadUInt64())
}

// ReadBytes reads a byte slice from the message buffer and advances the cursor.
// If there's not enough data to read, it returns nil.
func (m *Message) ReadBytes() []byte {
	var bytes []byte

	if m.index+4 > len(m.buffer) {
		return nil
	}

	length := m.ReadUInt32()

	if m.index+int(length) > len(m.buffer) {
		return nil
	}

	bytes = m.buffer[m.index : m.index+int(length)]
	m.index += int(length)

	return bytes
}

// ReadString reads a string from the message buffer and advances the cursor.
// If there's not enough data to read, it returns an empty string.
func (m *Message) ReadString() string {
	return string(m.ReadBytes())
}

// ReadMessage reads a Message from the message buffer and advances the cursor.
// If there's not enough data to read, it returns nil.
func (m *Message) ReadMessage() *Message {
	return NewMessageFromBuffer(m.ReadBytes())
}

// Seal returns the serialized message buffer.
func (m *Message) Seal() []byte {
	return m.buffer
}
