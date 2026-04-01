# ◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆
#   Hitung Pajak  ·  Makefile  ·  2026
#   Cross-platform: Linux · macOS · Windows (MinGW)
# ◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆

CC     := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -O2
SRC    := main.c
TARGET := hitung_pajak

ifeq ($(OS),Windows_NT)
  TARGET := hitung_pajak.exe
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f hitung_pajak hitung_pajak.exe
