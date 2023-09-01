cmake_minimum_required(VERSION 3.14)
project(named-tuple
  DESCRIPTION "named tuple for modern C++"
  VERSION 1.0.0
  LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(named-tuple INTERFACE)
target_include_directories(named-tuple
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_compile_features(named-tuple INTERFACE cxx_std_20)

# Installation help
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION "share/cmake/${PROJECT_NAME}")

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT)

install(TARGETS named-tuple
    EXPORT ${PROJECT_NAME}-targets
    INCLUDES DESTINATION "${CMAKE_INSTALL_DATADIR}")

install(EXPORT ${PROJECT_NAME}-targets
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
    NAMESPACE tl::
    FILE "${PROJECT_NAME}-targets.cmake")

install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}")

install(DIRECTORY "include/" TYPE INCLUDE)
