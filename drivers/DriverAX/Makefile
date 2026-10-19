#---------------->> Compilacion de libreria y modulo de python

# Compilador
CXX := g++
# Directorios
SRC_DIR := src
OBJ_DIR := obj
LIB_DIR := lib
BUILD_DIR := build
INCLUDE_DIR := include

# Configuración básica
PYTHON_INCLUDES := $(shell /usr/bin/python3 -c "from sysconfig import get_paths; print('-I' + get_paths()['include'])")
PYTHON_LDFLAGS := $(shell /usr/bin/python3-config --ldflags)
PYBIND11_HEADERS := $(shell /usr/bin/python3 -m pybind11 --includes) 

# Opciones de compilación
CXXFLAGS := -std=c++14 -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/dynamixel_sdk -fPIC $(PYTHON_INCLUDES)
LDFLAGS := -L$(LIB_DIR) -lDynamixelAXControl -lDynamixelManagerP1 -ldxl_x64_cpp -shared $(PYTHON_LDFLAGS)

# Archivos fuente
LIB_SRCS := $(SRC_DIR)/DynamixelAXControl.cpp
PYMOD_SRCS := $(SRC_DIR)/DynamixelLinkPython.cpp
LIB_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(LIB_SRCS))
PYMOD_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(PYMOD_SRCS))

# Objetivos principales
all: $(LIB_DIR)/libDynamixelAXControl.a $(BUILD_DIR)/DynamixelAXControl.so

# Regla para la biblioteca estática
$(LIB_DIR)/libDynamixelAXControl.a: $(LIB_OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Regla para el módulo Python
$(BUILD_DIR)/DynamixelAXControl.so: $(PYMOD_OBJS) $(LIB_DIR)/libDynamixelAXControl.a $(LIB_DIR)/libDynamixelManagerP1.a
	@mkdir -p $(BUILD_DIR)
	$(CXX) -shared $^ $(LDFLAGS) -o $@ $(PYBIND11_HEADERS)

# Regla genérica para archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)/*.so $(LIB_DIR)/*.a

.PHONY: all clean