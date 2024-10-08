cmake_minimum_required(VERSION 3.16)
project(cwasm)

set(SRCS
    src/section/code.c
    src/section/custom.c
    src/section/data_count.c
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

include_directories(include)

if(WASM_BUILD)
    set(CMAKE_C_COMPILER "emcc")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWASM_BUILD -sMODULARIZE=1 -sEXPORT_NAME=create_module -sTOTAL_MEMORY=1gb -sEXPORTED_FUNCTIONS=_main,_malloc,_free")
else()
endif()

if(DEBUG_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -gdwarf-4")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG -march=native -utune=native")
endif()

add_library(cwasm ${SRCS})

install(TARGETS cwasm
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
 