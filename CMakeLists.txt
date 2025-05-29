cmake_minimum_required(VERSION 3.10)
project(sokol_bitmap_font C)

set(CMAKE_C_STANDARD 11)

set(LIB_NAME skbmf)
set(EXE_NAME skbmf_test)

set(TEST_ENTRY ${CMAKE_SOURCE_DIR}/test/main.c)

set(SOKOL_SRC_DIR ${CMAKE_SOURCE_DIR}/vendor/sokol)
set(SOKOL_HEADERS
  ${CMAKE_SOURCE_DIR}/vendor/sokol/sokol_gfx.h
  ${CMAKE_SOURCE_DIR}/vendor/sokol/sokol_app.h
  ${CMAKE_SOURCE_DIR}/vendor/sokol_gp/sokol_gp.h
  ${CMAKE_SOURCE_DIR}/sokol_bitmap_font.h
)

set(SOKOL_GP_DIR ${CMAKE_SOURCE_DIR}/vendor/sokol_gp)

set(STB_DIR ${CMAKE_SOURCE_DIR}/vendor/stb)

# sokol stuff

# mac build
if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    add_library(sokol STATIC ${CMAKE_SOURCE_DIR}/sokol.c ${SOKOL_HEADERS})
    target_include_directories(sokol PRIVATE ${SOKOL_SRC_DIR} ${SOKOL_GP_DIR})
    target_compile_options(sokol PRIVATE -x objective-c)
    target_link_libraries(sokol PUBLIC
        "-framework QuartzCore"
        "-framework Cocoa"
        "-framework MetalKit"
        "-framework Metal")
else()

# linux build
    add_library(sokol STATIC ${CMAKE_SOURCE_DIR}/sokol.c ${SOKOL_HEADERS})
    target_include_directories(sokol PRIVATE ${SOKOL_SRC_DIR} ${SOKOL_GP_DIR} ${CMAKE_SOURCE_DIR})
    if (CMAKE_SYSTEM_NAME STREQUAL Linux)
        target_link_libraries(sokol INTERFACE X11 Xi Xcursor GL dl m)
    endif()
endif()

# end sokol stuff

add_executable(${EXE_NAME} 
  ${TEST_ENTRY}
  ${CMAKE_SOURCE_DIR}/test/img.c
  ${CMAKE_SOURCE_DIR}/test/img.h
)
target_include_directories(${EXE_NAME} PRIVATE ${SOKOL_SRC_DIR} ${SOKOL_GP_DIR} ${CMAKE_SOURCE_DIR} ${STB_DIR})
target_compile_options(${EXE_NAME} PRIVATE -Wall)
target_link_libraries(${EXE_NAME} sokol)


set_target_properties(${EXE_NAME} PROPERTIES LINKER_LANGUAGE C)
