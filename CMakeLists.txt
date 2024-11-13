
cmake_minimum_required(VERSION 3.11...3.18)
project(shim C CXX)
find_package(HDF5 REQUIRED COMPONENTS C CXX)

set(SOURCES 
    gridShim.C
    sendToHDF5.C
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(TARGET hdf5::hdf5)
    # cmake >= 3.19
    target_link_libraries(hdf PRIVATE hdf5::hdf5 hdf5::hdf5_cpp)
else()
    # cmake < 3.19
    target_link_libraries(hdf PRIVATE ${HDF5_C_LIBRARIES} ${HDF5_CXX_LIBRARIES})
    target_include_directories(hdf PRIVATE ${HDF5_C_INCLUDE_DIRS} ${HDF5_CXX_INCLUDE_DIRS})
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
