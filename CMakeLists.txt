cmake_minimum_required(VERSION 3.20)

project(serverClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cppzmq)

set(SOURCES
	main.cpp
    cppzmq/zmq.hpp
    libzmq/include/zmq.h
    libzmq/include/zmq_utils.h

    client/client.h
    dependencies/depend.h
    render/render.h
)

add_executable(serverClient ${SOURCES})

target_link_directories(${PROJECT_NAME} PUBLIC
)

target_include_directories(${PROJECT_NAME} PUBLIC
cppzmq-static
)

target_link_libraries(${PROJECT_NAME} PUBLIC
cppzmq-static
${CMAKE_DL_LIBS}
)

target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:MSVC>:
                       >)