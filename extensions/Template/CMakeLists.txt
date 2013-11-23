
cmake_minimum_required(VERSION 2.8.11)

project(hSDK_EXTENSION)

set(EXT_NAME hSDK_Template)
message(STATUS "Extension Filename = ${EXT_NAME}.mfx")

#Grab source & header files
file(GLOB_RECURSE EXT_SOURCES "${PROJECT_SOURCE_DIR}/*.c*")
file(GLOB_RECURSE EXT_HEADERS "${PROJECT_SOURCE_DIR}/*.h*")
file(GLOB_RECURSE SDK_SOURCES "${PROJECT_SOURCE_DIR}/../../src/*.c*")
file(GLOB_RECURSE SDK_HEADERS "${PROJECT_SOURCE_DIR}/../../include/*.h*")
set(EXT_SOURCES ${EXT_SOURCES} ${SDK_SOURCES})
set(EXT_HEADERS ${EXT_HEADERS} ${SDK_HEADERS})
include_directories("${PROJECT_SOURCE_DIR}/../../include/" "${PROJECT_SOURCE_DIR}/")
link_directories("${PROJECT_SOURCE_DIR}/" "${PROJECT_SOURCE_DIR}/../../lib/")
message(STATUS "Extension Sources = ${EXT_SOURCES}")
message(STATUS "Extension Headers = ${EXT_HEADERS}")

set(CMAKE_CXX_FLAGS "-std=c++11 -O2 ${CMAKE_CXX_FLAGS} -Wno-multichar")
set(CMAKE_BUILD_TYPE "RELEASE")

list(APPEND EXT_MFX_TYPE "Plain")
list(APPEND EXT_MFX_TYPE "HWA")
list(APPEND EXT_MFX_TYPE "Unicode")
list(APPEND EXT_BUILD_TYPE "Debug")
list(APPEND EXT_BUILD_TYPE "Release")
list(APPEND EXT_BUILD_TYPE "RunOnly")

foreach(curr_mfx_t ${EXT_MFX_TYPE})
    foreach(curr_build_t ${EXT_BUILD_TYPE})
        set(curr_name ${curr_mfx_t}${curr_build_t})

        add_library(${curr_name} SHARED ${EXT_SOURCES})

        set(curr_defs "_WINDLL;COXSDK;IN_EXT_VERSION2;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;_WINDOWS")

        if(curr_build_t STREQUAL "Release")
           set(out_dir "${CMAKE_CURRENT_BINARY_DIR}/MFX/Extensions/")
           list(APPEND curr_defs "EDITOR")

        elseif(curr_build_t STREQUAL "RunOnly")
           set(out_dir "${CMAKE_CURRENT_BINARY_DIR}/MFX/Data/Runtime/")
           list(APPEND curr_defs "RUN_ONLY")

        else()
            set(out_dir "${CMAKE_CURRENT_BINARY_DIR}/MFX_Debug/Extensions/")
            list(APPEND curr_defs "EDITOR;DEBUG;_DEBUG")
        endif()

        if(curr_mfx_t STREQUAL "HWA")
           set(out_dir "${out_dir}HWA/")
           set(curr_libs "mmfs2hwa")
           list(APPEND curr_defs "HWABETA;HWA")

        elseif(curr_mfx_t STREQUAL "Unicode")
           set(out_dir "${out_dir}Unicode/")
           set(curr_libs "mmfs2u")
           list(APPEND curr_defs "_UNICODE;UNICODE")

        else()
            set(curr_libs "mmfs2")
        endif()

		message(STATUS "Target ${curr_name} outputs to ${out_dir}")

        target_link_libraries(${curr_name} ${curr_libs})
        SET_TARGET_PROPERTIES(${curr_name} PROPERTIES
            LIBRARY_OUTPUT_NAME "${EXT_NAME}.mfx"
            LIBRARY_OUTPUT_DIRECTORY "${out_dir}"
            COMPILE_DEFINITIONS "${curr_defs}"
        )
    endforeach()
endforeach()
