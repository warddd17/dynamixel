cmake_minimum_required(VERSION 3.8)
project(arm_ax_interfaces)
set(PYTHON_EXECUTABLE "/usr/bin/python3")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# find dependencies
find_package(ament_cmake REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rosidl_default_generators REQUIRED)

set(msg_files
  "msg/SetOnePosition.msg"
)

set(srv_files
  "srv/GetOnePosition.srv"
)

#set(act_files)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  ${srv_files}
  DEPENDENCIES builtin_interfaces
)

ament_package()
