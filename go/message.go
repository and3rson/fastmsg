package main

import "encoding/binary"

type Message struct {
	Buffer []byte
	Index  int
}

func NewMessage() *Message {
	return &Message{
		Buffer: make([]byte, 0),
		Index:  0,
	}
}

func NewMessageFromBuffer(buffer []byte) *Message {
	return &Message{
		Buffer: buffer,
		Index:  0,
	}
}

func (m *Message) AddUInt8(b uint8) *Message {
	m.Buffer = append(m.Buffer, b)

	return m
}

func (m *Message) AddUInt16(value uint16) *Message {
	buf := make([]byte, 2)
	binary.LittleEndian.PutUint16(buf, value)
	m.Buffer = append(m.Buffer, buf...)

	return m
}

func (m *Message) AddUInt32(value uint32) *Message {
	buf := make([]byte, 4)
	binary.LittleEndian.PutUint32(buf, value)
	m.Buffer = append(m.Buffer, buf...)

	return m
}

func (m *Message) AddUInt64(value uint64) *Message {
	buf := make([]byte, 8)
	binary.LittleEndian.PutUint64(buf, value)
	m.Buffer = append(m.Buffer, buf...)

	return m
}

func (m *Message) AddInt8(b int8) *Message {
	m.Buffer = append(m.Buffer, byte(b))

	return m
}

func (m *Message) AddInt16(value int16) *Message {
	buf := make([]byte, 2)
	binary.LittleEndian.PutUint16(buf, uint16(value))
	m.Buffer = append(m.Buffer, buf...)

	return m
}

func (m *Message) AddInt32(value int32) *Message {
	buf := make([]byte, 4)
	binary.LittleEndian.PutUint32(buf, uint32(value))
	m.Buffer = append(m.Buffer, buf...)

	return m
}

func (m *Message) AddInt64(value int64) *Message {
	buf := make([]byte, 8)
	binary.LittleEndian.PutUint64(buf, uint64(value))
	m.Buffer = append(m.Buffer, buf...)

	return m
}

func (m *Message) AddString(str string) *Message {
	m.AddUInt32(uint32(len(str)))
	m.Buffer = append(m.Buffer, []byte(str)...)

	return m
}

func (m *Message) AddBytes(bytes []byte) *Message {
	m.AddUInt32(uint32(len(bytes)))
	m.Buffer = append(m.Buffer, bytes...)

	return m
}

func (m *Message) AddMessage(message *Message) *Message {
	m.AddBytes(message.Buffer)

	return m
}

func (m *Message) Reset() {
	m.Index = 0
}

func (m *Message) ReadUInt8() uint8 {
	if m.Index >= len(m.Buffer) {
		return 0
	}

	b := m.Buffer[m.Index]

	m.Index++

	return b
}

func (m *Message) ReadUInt16() uint16 {
	if m.Index+2 > len(m.Buffer) {
		return 0
	}

	value := binary.LittleEndian.Uint16(m.Buffer[m.Index : m.Index+2])

	m.Index += 2

	return value
}

func (m *Message) ReadUInt32() uint32 {
	if m.Index+4 > len(m.Buffer) {
		return 0
	}

	value := binary.LittleEndian.Uint32(m.Buffer[m.Index : m.Index+4])

	m.Index += 4

	return value
}

func (m *Message) ReadUInt64() uint64 {
	if m.Index+8 > len(m.Buffer) {
		return 0
	}

	value := binary.LittleEndian.Uint64(m.Buffer[m.Index : m.Index+8])

	m.Index += 8

	return value
}

func (m *Message) ReadInt8() int8 {
	return int8(m.ReadUInt8())
}

func (m *Message) ReadInt16() int16 {
	return int16(m.ReadUInt16())
}

func (m *Message) ReadInt32() int32 {
	return int32(m.ReadUInt32())
}

func (m *Message) ReadInt64() int64 {
	return int64(m.ReadUInt64())
}

func (m *Message) ReadBytes() []byte {
	var bytes []byte

	if m.Index+4 > len(m.Buffer) {
		return nil
	}

	length := m.ReadUInt32()

	if m.Index+int(length) > len(m.Buffer) {
		return nil
	}

	bytes = m.Buffer[m.Index : m.Index+int(length)]
	m.Index += int(length)

	return bytes
}

func (m *Message) ReadString() string {
	return string(m.ReadBytes())
}

func (m *Message) ReadMessage() *Message {
	return NewMessageFromBuffer(m.ReadBytes())
}
