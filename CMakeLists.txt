cmake_minimum_required(VERSION 3.12)

project(can-on-serial-com)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include
    vendor/fmt/include
    vendor/CppLinuxSerial/include
)

add_subdirectory(src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# install rule
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION /usr/local/bin
)