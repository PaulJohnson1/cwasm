cmake_minimum_required(VERSION 3.16)
project(cwasm)

set(SRCS
    section/cwasm_code.c
    section/cwasm_data.c
    section/cwasm_element.c
    section/cwasm_export.c
    section/cwasm_function.c
    section/cwasm_global.c
    section/cwasm_import.c
    section/cwasm_table.c
    section/cwasm_memory.c
    section/cwasm_type.c
    cwasm_type.c
    cwasm.c
    pb.c
    test-main.c
)

include_directories(.)

if(DEBUG_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -gdwarf-4")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DPROTO_BUG_NDEBUG -march=native -utune=native")
endif()

add_executable(cwasm ${SRCS})
