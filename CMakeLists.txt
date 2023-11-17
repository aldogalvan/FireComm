cmake_minimum_required(VERSION 3.26)
project(SerialExample)

set(CMAKE_CXX_STANDARD 17)

add_executable(SerialExample main.cpp)
add_executable(SerialExample2 main2.cpp)
add_executable(SerialExample3 main3.cpp)

# Find and link the libserial library
find_library(LIBSERIAL_LIBRARY NAMES serial PATHS /usr/local/lib/libserial.so)
if(NOT LIBSERIAL_LIBRARY)
    message(FATAL_ERROR "libserial.so not found. Please provide the correct path to the library.")
endif()

find_library(LIBSERIALPORT_LIBRARY NAMES serialport PATHS /usr/local/lib/libserialport.so)
if(NOT LIBSERIALPORT_LIBRARY)
    message(FATAL_ERROR "libserial.so not found. Please provide the correct path to the library.")
endif()

target_include_directories(SerialExample PUBLIC /usr/local/include/libserial)
target_link_libraries(SerialExample PUBLIC ${LIBSERIAL_LIBRARY})
target_include_directories(SerialExample2 PUBLIC /usr/local/include/libserial)
target_link_libraries(SerialExample2 PUBLIC ${LIBSERIAL_LIBRARY})
target_include_directories(SerialExample3 PUBLIC /usr/local/include/)
target_link_libraries(SerialExample3 PUBLIC ${LIBSERIALPORT_LIBRARY})
