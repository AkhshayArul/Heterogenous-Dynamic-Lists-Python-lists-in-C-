
cmake_minimum_required(VERSION 3.20)

project(PythonList VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(PythonList src/List.cpp)

target_include_directories(PythonList
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(PythonList PUBLIC cxx_std_17)

# add_executable(PythonListDemo main.cpp)
# target_link_libraries(PythonListDemo PRIVATE PythonList)

include(CTest)

if(BUILD_TESTING)
    add_executable(PythonListRegressionTests
        tests/regression_test.cpp
    )

    target_link_libraries(PythonListRegressionTests PRIVATE PythonList)

    add_test(
        NAME PythonListRegressionTests
        COMMAND PythonListRegressionTests
    )
endif()

install(TARGETS PythonList
    EXPORT PythonListTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES
        include/PythonList
        include/List.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT PythonListTargets
    FILE PythonListTargets.cmake
    NAMESPACE PythonList::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PythonList
)

configure_package_config_file(
    cmake/PythonListConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/PythonListConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/PythonList"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/PythonListConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/PythonListConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/PythonListConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/PythonList"
)
