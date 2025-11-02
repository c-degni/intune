cmake_minimum_required(VERSION 3.16)
project(SignalProcessor LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find OpenCV (installed via brew or apt)
find_package(OpenCV REQUIRED)

# Include the main source directory and the signal_processor subfolder
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/signal_processor
    ${OpenCV_INCLUDE_DIRS}
)

# Collect all source files recursively
file(GLOB SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/signal_processor/*.cpp"
)

# Create the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link OpenCV
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
