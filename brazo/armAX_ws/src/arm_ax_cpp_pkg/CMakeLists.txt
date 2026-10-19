cmake_minimum_required(VERSION 3.8)
project(arm_ax_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Buscar dependencias
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(arm_ax_interfaces REQUIRED)

# Configurar rutas
set(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib)
include_directories(
  include
  ${rclcpp_INCLUDE_DIRS}
  ${arm_ax_interfaces_INCLUDE_DIRS}
)

# Enlazar bibliotecas estáticas
add_library(armax STATIC IMPORTED)
set_target_properties(armax PROPERTIES
  IMPORTED_LOCATION ${LIBRARY_DIR}/libarmAX.a
)

add_library(ax_manager STATIC IMPORTED)
set_target_properties(ax_manager PROPERTIES
  IMPORTED_LOCATION ${LIBRARY_DIR}/libDynamixelAXControl.a
)

add_library(dxl_manager STATIC IMPORTED)
set_target_properties(dxl_manager PROPERTIES
  IMPORTED_LOCATION ${LIBRARY_DIR}/libDynamixelManager.a
)

# Crear ejecutable
add_executable(arm_ax_node 
  src/arm_ax_cpp_node.cpp
)

# Enlazar dependencias
target_link_libraries(arm_ax_node
  armax
  ax_manager
  dxl_manager
  rclcpp::rclcpp
  arm_ax_interfaces::arm_ax_interfaces__rosidl_typesupport_cpp
  dxl_x64_cpp
)

# Instalación
install(TARGETS arm_ax_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

install(DIRECTORY lib/
  DESTINATION lib
)

ament_package()