package main

import (
	"bytes"
	"testing"
)

func TestNormal(t *testing.T) {
	t.Log("TestNormal")

	msg := NewMessage()
	msg.AddString("Hello")
	msg.AddUInt16(42)
	msg.AddBytes([]byte{34, 13, 37})

	msgNested := NewMessage()
	msgNested.AddString("Nested")
	msgNested.AddInt64(-42069)
	msg.AddMessage(msgNested)

	t.Logf("Serialized: %v", msg)

	if len(msg.Buffer) != 40 {
		t.Errorf("Expected length 40, got %d", len(msg.Buffer))
	}

	buffer := msg.Buffer

	deserialized := NewMessageFromBuffer(buffer)
	t.Logf("Deserialized: %v", deserialized)

	s := deserialized.ReadString()
	if s != "Hello" {
		t.Errorf("Expected Hello, got %s", s)
	}

	u := deserialized.ReadUInt16()
	if u != 42 {
		t.Errorf("Expected 42, got %d", u)
	}

	b := deserialized.ReadBytes()
	if len(b) != 3 {
		t.Errorf("Expected length 3, got %d", len(b))
	}

	if !bytes.Equal(b, []byte{34, 13, 37}) {
		t.Errorf("Expected [34, 13, 37], got %v", b)
	}

	nested := deserialized.ReadMessage()

	s = nested.ReadString()
	if s != "Nested" {
		t.Errorf("Expected Nested, got %s", s)
	}

	i := nested.ReadInt64()
	if i != -42069 {
		t.Errorf("Expected -42069, got %d", u)
	}
}

func TestBrokenLength(t *testing.T) {
	t.Log("TestBrokenLength")

	msg := NewMessageFromBuffer([]byte{0xFF, 0xFF, 0xFF, 0xFF, 0x01})

	u := msg.ReadUInt32()
	if u != 0xFFFFFFFF {
		t.Errorf("Expected 0xFFFFFFFF, got 0x%08X", u)
	}

	msg.Reset()

	b := msg.ReadBytes()
	if b != nil {
		t.Errorf("Expected nil, got %v", b)
	}

	msg.Reset()

	s := msg.ReadString()
	if s != "" {
		t.Errorf("Expected empty string, got %s", s)
	}

	msg.Reset()

	msg = msg.ReadMessage()
	if len(msg.Buffer) != 0 {
		t.Errorf("Expected length 0, got %d", len(msg.Buffer))
	}
}

func TestOutOfBounds(t *testing.T) {
	t.Log("TestOutOfBounds")

	msg := NewMessageFromBuffer([]byte{0x01})

	u := msg.ReadUInt32()
	if u != 0 {
		t.Errorf("Expected 0, got %d", u)
	}

	msg.Reset()

	b := msg.ReadBytes()
	if b != nil {
		t.Errorf("Expected nil, got %v", b)
	}

	msg.Reset()

	s := msg.ReadString()
	if s != "" {
		t.Errorf("Expected empty string, got %s", s)
	}

	msg.Reset()

	msg = msg.ReadMessage()
	if len(msg.Buffer) != 0 {
		t.Errorf("Expected length 0, got %d", len(msg.Buffer))
	}
}
