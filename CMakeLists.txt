cmake_minimum_required(VERSION 3.16)
project(cwasm)

set(SRCS
    src/section/code.c
    src/section/data.c
    src/section/element.c
    src/section/export.c
    src/section/function.c
    src/section/global.c
    src/section/import.c
    src/section/table.c
    src/section/memory.c
    src/section/type.c
    src/type.c
    src/cwasm.c
    src/pb.c
    src/test-main.c
)

include_directories(src)

if(DEBUG_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -gdwarf-4")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DPROTO_BUG_NDEBUG -march=native -utune=native")
endif()

add_executable(cwasm ${SRCS})
