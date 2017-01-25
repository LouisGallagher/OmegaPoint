cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OpenCV REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Pangolin 0.4 REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(Boost COMPONENTS date_time REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LCM REQUIRED lcm)

file(GLOB lcmtypes lcmtypes/*.lcm)
execute_process(COMMAND lcm-gen -x ${lcmtypes} --cpp-hpath ${CMAKE_CURRENT_SOURCE_DIR}/lcmtypes/)
 
set(CMAKE_CXX_FLAGS "-msse2 -msse3 -std=c++11 -g")#-O3 

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/utils)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/networking)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/data)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lcmtypes/)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${LCM_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Pangolin_INCLUDE_DIRS})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK ON)   # force dynamic linking for all libraries



set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,. -L${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB srcs ./*.cpp
         data/*.cpp
         networking/*.cpp)

add_executable(MultiCameraLogViewer ${srcs})

target_link_libraries(MultiCameraLogViewer                      
                      ${Boost_LIBRARIES}
                      ${OpenCV_LIBS}
                      ${LCM_LIBRARIES}
                      ${ZLIB_LIBRARY}
                      ${Pangolin_LIBRARIES}
                      boost_system
                      boost_filesystem
                      boost_thread)