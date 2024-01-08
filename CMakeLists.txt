cmake_minimum_required(VERSION 3.16)
project(cwasm)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

set(SRCS
    src/section/code.c
    src/section/custom.c
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
)

include_directories(src)

if(TEST_BUILD)
    # todo: Paul
    # add tests
else()
    list(APPEND SRCS "src/test-main.c")
endif()

if(DEBUG_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -gdwarf-4")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DPROTO_BUG_NDEBUG -march=native -utune=native")
endif()

add_executable(cwasm ${SRCS})
