cmake_minimum_required(VERSION 3.10)
project(propagation_lockable)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic)

include_directories("${PROJECT_SOURCE_DIR}/lib" "${PROJECT_SOURCE_DIR}/src")
add_subdirectory(matplotplusplus)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
file(GLOB LIB_SRC_FILES
	"${PROJECT_SOURCE_DIR}/lib/*.hpp"
	"${PROJECT_SOURCE_DIR}/lib/*.cpp"
	)
add_library(propagate ${LIB_SRC_FILES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
foreach(SRC_FILE ${SRC_FILES})
	get_filename_component(OUTPUT ${SRC_FILE} NAME_WE)
	add_executable(${OUTPUT} ${SRC_FILE})
	target_link_libraries(${OUTPUT} PUBLIC propagate matplot)
endforeach(SRC_FILE ${SRC_FILES})

message("Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
