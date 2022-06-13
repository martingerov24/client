cmake_minimum_required(VERSION 3.16)

project(serverClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(PkgConfig)

find_package(cppzmq)
# pkg_check_modules(PC_ZeroMQ QUIET zmq)

set(SOURCES
	#code
    src/main.cpp
    src/Client.h
    src/Client.cpp
    src/Server.h
    src/Server.cpp
    #zmq dependencies
    cppzmq/zmq.hpp
    libzmq/include/zmq.h
    libzmq/include/zmq_utils.h
    #dependencies
    src/dpd/json.hpp
    src/dpd/stbi_load.h
    src/dpd/depend.h
    src/dpd/stbi_write.h
    #json creating
    src/Json.h
    src/Json.cpp
)

add_executable(serverClient ${SOURCES})

target_link_directories(${PROJECT_NAME} PUBLIC
)

target_include_directories(${PROJECT_NAME} PUBLIC
    cppzmq-static
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dpd
)

target_link_libraries(${PROJECT_NAME} PUBLIC
cppzmq-static
${CMAKE_DL_LIBS}
)

target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:MSVC>:>)