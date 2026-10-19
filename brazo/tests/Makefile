# Configuración básica
CXX := g++
CXXFLAGS := -std=c++14 -Wall -Iinclude
LDFLAGS := -Llib -larmAX -lDynamixelAXControl -lDynamixelManager -ldxl_x64_cpp

# Directorios
SRC_DIR := src
BIN_DIR := bin

# Archivo fuente
TARGET := $(BIN_DIR)/prueba

all: $(TARGET)

$(TARGET): $(SRC_DIR)/prueba.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean