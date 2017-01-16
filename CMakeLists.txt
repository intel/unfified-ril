cmake_minimum_required(VERSION 2.8)
project(uril)

option(WITH_TESTS "Unit tests" OFF)

set(URIL_MAJOR_VERSION 0)
set(URIL_MINOR_VERSION 1)
set(URIL_PATCH_VERSION 0)
set(URIL_VERSION_STRING ${URIL_MAJOR_VERSION}.${URIL_MINOR_VERSION}.${URIL_PATCH_VERSION})

#-----------------------------------CLIENT-------------------------------------

if(WITH_TESTS)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DURIL_LOG_TO_STDOUT")
ADD_DEFINITIONS(-DURIL_LOG_TO_STDOUT)
endif()

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
#set_property(TARGET uril_client PROPERTY C_STANDARD 99)
set (CMAKE_C_FLAGS "--std=gnu99 -g -Wall -Werror ${CMAKE_C_FLAGS}")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(inc)
include_directories(inc/internal)
include_directories(service/inc)

file(GLOB_RECURSE uril_client_c_files client/src/*.c)
file(GLOB uril_client_public_h_files inc/*.h)

add_library(uril_client
    SHARED
    ${uril_client_c_files})
target_link_libraries(uril_client "-pthread")

set_target_properties(uril_client
    PROPERTIES
    VERSION ${URIL_VERSION_STRING}
    SOVERSION ${URIL_VERSION_STRING})

install(TARGETS uril_client LIBRARY DESTINATION lib)
install(FILES ${uril_client_public_h_files}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/uril)

#-----------------------------------SERVICE------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(inc)
include_directories(inc/internal)
include_directories(service/inc)

file(GLOB_RECURSE uril_service_cpp_files service/src/*.cpp)
file(GLOB uril_client_public_h_files inc/*.h)
file(GLOB uril_service_public_h_files service/inc/*.h)

add_executable(uril_service
    ${uril_service_cpp_files})
target_link_libraries(uril_service "-pthread")
target_link_libraries(uril_service ${CMAKE_DL_LIBS})

set_target_properties(uril_service
    PROPERTIES
    VERSION ${URIL_VERSION_STRING}
    SOVERSION ${URIL_VERSION_STRING})
install(TARGETS uril_service RUNTIME DESTINATION bin)
install(FILES ${uril_service_public_h_files}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/uril_service)

#-----------------------------------TESTS--------------------------------------

if(WITH_TESTS)

project(gtest_builder C CXX)
include(ExternalProject)

ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
               -DCMAKE_CXX_FLAGS=${MSVC_COMPILER_DEFS}
               -Dgtest_force_shared_crt=ON
               -DBUILD_GTEST=ON
     PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
# Disable install step
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/googletest/include)

# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest binary_dir)
set(GTEST_LIBS_DIR ${binary_dir}/googlemock/gtest)

#-------------------------------CLIENT TESTS-----------------------------------
project(urilclient_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(../inc)
include_directories(../inc/internal)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB uril_client_public_h_files inc/*.h)
file(GLOB uril_client_internal_h_files inc/internal/*.h)

file(GLOB_RECURSE uril_client_test_c_files tests/client/src/*.cpp)

add_executable(urilclient_tests ${uril_client_test_c_files})


MESSAGE( STATUS "GTEST_LIBRARIES:         " ${GTEST_LIBRARIES} )
MESSAGE( STATUS "GTEST_MAIN_LIBRARIES:         " ${GTEST_MAIN_LIBRARIES} )
MESSAGE( STATUS "GTEST_INCLUDE_DIRS:         " ${GTEST_INCLUDE_DIRS} )
MESSAGE( STATUS "GTEST_LIBS_DIR:         " ${GTEST_LIBS_DIR} )


target_link_libraries(urilclient_tests
    liburil_client
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    ${GMOCK_LIBRARIES}
)

install(TARGETS urilclient_tests DESTINATION bin)

#-------------------------------SERVICE TESTS----------------------------------
project(urilservice_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(../inc)
include_directories(../inc/internal)
include_directories(../service/inc/)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB uril_service_public_h_files service/inc/*.h)
file(GLOB_RECURSE uril_service_test_cpp_files tests/service/src/*.cpp)
file(GLOB_RECURSE uril_service_adapters_cpp_files service/src/modem_adapters/*.cpp)
file(GLOB_RECURSE uril_service_processors_cpp_files service/src/processor/*.cpp)

add_executable(urilservice_tests
    ${uril_service_test_cpp_files}
    ${uril_service_adapters_cpp_files}
    ${uril_service_processors_cpp_files}
    service/src/MessageTracker.cpp
)

MESSAGE( STATUS "GTEST_LIBRARIES:         " ${GTEST_LIBRARIES} )
MESSAGE( STATUS "GTEST_MAIN_LIBRARIES:         " ${GTEST_MAIN_LIBRARIES} )
MESSAGE( STATUS "GTEST_INCLUDE_DIRS:         " ${GTEST_INCLUDE_DIRS} )
MESSAGE( STATUS "GTEST_LIBS_DIR:         " ${GTEST_LIBS_DIR} )


target_link_libraries(urilservice_tests
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    ${GMOCK_LIBRARIES}
)
target_link_libraries(urilservice_tests "-pthread")
target_link_libraries(urilservice_tests ${CMAKE_DL_LIBS})

install(TARGETS urilservice_tests DESTINATION bin)

endif()

