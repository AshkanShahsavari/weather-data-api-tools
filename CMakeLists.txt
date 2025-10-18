cmake_minimum_required(VERSION 3.12)
project(weather_data_api_tools VERSION 1.0.0 LANGUAGES CXX)

# --- Option: generate only documentation ---
option(DOXYGEN_DOC "Generate API documentation with Doxygen" OFF)
if(DOXYGEN_DOC)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        message(STATUS "Doxygen found. Generating documentation...")
        configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_SOURCE_DIR}/Doxyfile @ONLY)
        execute_process(
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        message(STATUS "Documentation generated in docs/")
    else()
        message(WARNING "Doxygen not found. Please install it to generate documentation.")
    endif()
    return()  # <--- stop here if only doc generation requested
endif()

# --- Normal build process below ---

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(weather_data_api_tools ${SRC_FILES})

find_library(CPR_LIB cpr)
find_library(INFLUXDB_LIB InfluxDB)

target_link_libraries(weather_data_api_tools PRIVATE ${CPR_LIB} ${INFLUXDB_LIB})

set_target_properties(weather_data_api_tools PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
