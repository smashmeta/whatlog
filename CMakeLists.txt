
cmake_minimum_required(VERSION 3.20)

set(WHATLOG_VERSION 1.0.0)
project(whatlog VERSION ${WHATLOG_VERSION} LANGUAGES CXX)

# CPack settings
set(CPACK_PACKAGE_VENDOR "whatlog")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "simple logging library")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/whatlog")
set(CPACK_PACKAGE_CONTACT "smashmeta")

add_library(whatlog STATIC src/logger.cpp include/whatlog/logger.hpp)
target_include_directories(whatlog PUBLIC "include")

# adding windows dependencies
# target_link_libraries(whatlog wsock32 ws2_32 IPHLPAPI wbemuuid)
# end windows dependencies

# adding boost dependencies
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT "D:/libraries/boost_1_75_0")
find_package(Boost REQUIRED COMPONENTS date_time system filesystem thread log log_setup)

add_definitions("-DBOOST_ALL_NO_LIB")
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(whatlog ${Boost_LIBRARIES})

# end boost dependency

install(TARGETS whatlog LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})