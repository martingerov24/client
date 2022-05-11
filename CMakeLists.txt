cmake_minimum_required(VERSION 3.20)

project(serverClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cppzmq)

set(SOURCES
	#code
    main.cpp
    client/client.h
    client/client.cpp
    #zmq dependencies
    cppzmq/zmq.hpp
    libzmq/include/zmq.h
    libzmq/include/zmq_utils.h
    #dependencies
    dependencies/json.hpp
    dependencies/stbi_load.h
    dependencies/depend.h
    dependencies/stbi_write.h
    #json creating
    Json/Json.h 
    Json/Json.cpp
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