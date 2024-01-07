cmake_minimum_required(VERSION 3.16)
project(cwasm)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

set(SRCS
    src/section/cwasm_code.c
    src/section/cwasm_data.c
    src/section/cwasm_element.c
    src/section/cwasm_export.c
    src/section/cwasm_function.c
    src/section/cwasm_global.c
    src/section/cwasm_import.c
    src/section/cwasm_table.c
    src/section/cwasm_memory.c
    src/section/cwasm_type.c
    src/cwasm_type.c
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
