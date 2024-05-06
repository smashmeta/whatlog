
cmake_minimum_required(VERSION 3.20)

set(PROJECT_VERSION_MAJOR 2)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_NAME "whatlog")
project(whatlog VERSION ${WHATLOG_VERSION} LANGUAGES CXX)

# CPack settings
set(CPACK_PACKAGE_VENDOR ${PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "simple logging library")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/smashmeta/whatlog")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MAINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_CONTACT "smashmeta")

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")

add_compile_definitions(WHATLOG_DLL)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
set(CMAKE_DEBUG_POSTFIX d)

# adding boost dependencies
find_package(Boost REQUIRED COMPONENTS log log_setup)

# adding thirdparty include directories
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB whatlog_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/whatlog/*.hpp")
file(GLOB whatlog_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(whatlog SHARED ${whatlog_SOURCE_FILES} ${whatlog_HEADER_FILES})

set_target_properties(whatlog PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(whatlog
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# adding thirdparty library dependencies
target_link_libraries(whatlog 
    PUBLIC 
        Boost::log 
        Boost::log_setup
)

############# INSTALL SECTION #######################################

set(PROJECT_EXPORTS_CONFIG "${PROJECT_NAME}Config")

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_EXPORTS_CONFIG}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT ${PROJECT_EXPORTS_CONFIG}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
    NAMESPACE ${PROJECT_NAME}::
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/whatlog" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# # setting up our whatlogConfig.cmake file for other projects to use
# install(TARGETS whatlog EXPORT whatlogTargets DESTINATION "install/lib")
# install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/whatlog" DESTINATION "install/include")
# install(EXPORT whatlogTargets FILE whatlogTargets.cmake DESTINATION "install/cmake")
# include(CMakePackageConfigHelpers)
# 
# # generate the config file that is includes the exports
# configure_package_config_file(
#     ${CMAKE_CURRENT_SOURCE_DIR}/whatlogConfig.cmake.in
#     "${CMAKE_CURRENT_BINARY_DIR}/whatlogConfig.cmake"
#     INSTALL_DESTINATION "install/cmake"
#     NO_SET_AND_CHECK_MACRO
#     NO_CHECK_REQUIRED_COMPONENTS_MACRO
# )
# 
# write_basic_package_version_file(
#   "${CMAKE_CURRENT_BINARY_DIR}/whatlogConfigVersion.cmake"
#   VERSION "${whatlog_VERSION_MAJOR}.${whatlog_VERSION_MINOR}"
#   COMPATIBILITY AnyNewerVersion
# )
# 
# install(FILES
#     ${CMAKE_CURRENT_BINARY_DIR}/whatlogConfig.cmake
#     ${CMAKE_CURRENT_BINARY_DIR}/whatlogConfigVersion.cmake
#     DESTINATION "install/cmake"
# )