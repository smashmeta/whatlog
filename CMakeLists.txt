
cmake_minimum_required(VERSION 3.20)

# CPack settings
set(CPACK_PACKAGE_VENDOR "whatlog")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "simple logging library")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/whatlog")
set(CPACK_PACKAGE_CONTACT "smashmeta")

set(CMAKE_TOOLCHAIN_FILE "C:/Users/fredrhe/Desktop/void/programs/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(WHATLOG_VERSION 1.0.0)
project(whatlog VERSION ${WHATLOG_VERSION} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# adding boost dependencies
find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED)

# adding windows dependencies
# target_link_libraries(whatlog wsock32 ws2_32 IPHLPAPI wbemuuid)
# end windows dependencies

# adding thirdparty include directories
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${fmt_INCLUDE_DIRS})

add_library(whatlog STATIC src/logger.cpp include/whatlog/logger.hpp)
target_include_directories(whatlog PUBLIC "include")

# adding thirdparty library dependencies
target_link_libraries(whatlog PRIVATE ${Boost_LIBRARIES})
target_link_libraries(whatlog PRIVATE fmt::fmt-header-only)

install(TARGETS whatlog LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})