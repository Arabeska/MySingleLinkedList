cmake_minimum_required(VERSION 3.11)

project(SingleLinkedList CXX)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

add_executable(MyList main.cpp SingleLinkedList.h Tests.h)

target_link_libraries(MyList ${SYSTEM_LIBS})