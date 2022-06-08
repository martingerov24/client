cmake_minimum_required(VERSION 3.16)

project(serverClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(PkgConfig)

find_package(cppzmq)
# pkg_check_modules(PC_ZeroMQ QUIET zmq)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/Json
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies
    ${CMAKE_CURRENT_SOURCE_DIR}/client
)

target_link_libraries(${PROJECT_NAME} PUBLIC
cppzmq-static
# ${ZeroMQ_LIBRARY}
${CMAKE_DL_LIBS}
)

target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:MSVC>:
                       >)