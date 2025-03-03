cmake_minimum_required(VERSION 3.5)

project(twist_mux)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_ros REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(diagnostic_updater REQUIRED)

add_library(twist_mux 
  src/twist_mux.cpp
  src/twist_mux_diagnostics.cpp
  src/twist_marker.cpp
)
add_library(twist_mux::twist_mux ALIAS twist_mux)
target_compile_features(twist_mux PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
target_include_directories(twist_mux PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
ament_target_dependencies(
  twist_mux
  "rclcpp"
  "rclcpp_components"
  "std_msgs"
  "geometry_msgs"
  "visualization_msgs"
  "diagnostic_updater"
)

install(
  DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)
install(
  TARGETS twist_mux
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

add_executable(twist_mux_node 
  src/twist_mux_node.cpp
)
target_include_directories(twist_mux_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_link_libraries(twist_mux_node twist_mux)

rclcpp_components_register_nodes(twist_mux "twist_mux::TwistMux")
rclcpp_components_register_nodes(twist_mux "twist_mux::TwistMarkerPublisher")

install(TARGETS twist_mux_node
  DESTINATION lib/${PROJECT_NAME})

add_executable(twist_marker
  src/twist_marker_node.cpp
)
target_include_directories(twist_marker PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_link_libraries(twist_marker twist_mux)

install(TARGETS twist_marker
  DESTINATION lib/${PROJECT_NAME})

install(PROGRAMS scripts/joystick_relay.py
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY config
    DESTINATION share/${PROJECT_NAME})

install(DIRECTORY launch
    DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(launch_testing_ament_cmake)
  add_launch_test(test/test_joystick_relay.py)

  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(
  "include/${PROJECT_NAME}"
)
ament_export_libraries(
  twist_mux
)
ament_export_targets(
  export_${PROJECT_NAME}
)

ament_package()
