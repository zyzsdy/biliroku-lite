cmake_minimum_required (VERSION 3.4)
project (Biliroku-lite)

#CURL
find_package(curl)
list(APPEND include_dirs ${CURL_INCLUDE_DIRS})
list(APPEND extra_libs ${CURL_LIBRARIES})

#RapidJSON
if(NOT RAPID_JSON_DIR)
    message(FATAL_ERROR "Set string entry RAPID_JSON_DIR to find RapidJSON.")
    return()
endif()

list(APPEND include_dirs "${RAPID_JSON_DIR}/include")

#Compile src/*
add_definitions(-std=c++11)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src ALL_SRC)
file(GLOB ALL_HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
add_executable(biliroku ${ALL_SRC} ${ALL_HEADERS})
target_include_directories(biliroku PRIVATE ${include_dirs})
target_link_libraries(biliroku PRIVATE ${extra_libs})

#install
install (TARGETS biliroku DESTINATION "${CMAKE_CURRENT_LIST_DIR}/bin")