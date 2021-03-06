#
# libdsnutil: DAS-SYSTEM Networks utility library
#
# This library contains various small helper functions and classes.
#
project(libdsnutil)
cmake_minimum_required(VERSION 2.8)

#
# This is required to load our custom .cmake files
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(CheckCXX11Features)
foreach(flag ${CXX11_FEATURE_LIST})
  message(STATUS "Enabling C++11 feature: ${flag}")
  add_definitions(-D ${flag})
endforeach()

#
# Enable more verbose warnings for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++0x")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")
endif(CMAKE_COMPILER_IS_GNUCXX)

#
# Enable more verbose warnings for Clang
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message(STATUS "Enabling Clang specific compiler options...")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -std=c++11")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

#
# CTest/CDash configuration; see CTestConfig.cmake for more info!
include(CTest)
enable_testing()
set(BUILDNAME "${BUILDNAME}" "Name of build on CDash dashboard")

find_package(Threads)

#
# CPack configuration to build binary packages
if(NOT DSNUTIL_SUBPROJECT)
  set(CPACK_PACKAGE_DESCRIPTION "#das-system Networks util library")
  set(CPACK_PACKAGE_VERSION_MAJOR "0")
  set(CPACK_PACKAGE_VERSION_MINOR "99")
  set(CPACK_PACKAGE_VERSION_PATCH "7")
  include(CPack)
endif(NOT DSNUTIL_SUBPROJECT)

option(BUILD_TESTING "Build tests for all components" OFF)
option(WANT_LOG4CPP "Enable log4cpp support" ON)
option(WANT_MPI "Enable MPI related classes" ON)
option(WANT_CSV "Enable CSV utility classes" ON)


#
# Check for log4cpp framework
if(WANT_LOG4CPP)
  find_package(Log4cpp)
  if(LOG4CPP_FOUND)
    include_directories(${LOG4CPP_INCLUDE_DIR})
  endif(LOG4CPP_FOUND)
endif(WANT_LOG4CPP)

#
# Check for cppcheck and provide macros to run it on other build targets
if(BUILD_TESTING)
  include(CppCheckTargets)
endif(BUILD_TESTING)

#
# Prepare config.h
configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})


#
# Setup list of header files for system-wide installation
set(libdsnutil_HEADERS "")


#
# Setup list of sources for our library
if(WANT_CSV)
  set(CSV_SOURCES csv/Row.cc csv/Iterator.cc)
  set(libdsnutil_HEADERS ${libdsnutil_HEADERS} csv.hh)
  add_library(dsnutil-csv ${CSV_SOURCES})
  install(TARGETS dsnutil-csv ARCHIVE DESTINATION lib)
endif(WANT_CSV)

if(NOT WIN32)
  set(libdsnutil_SOURCES ${libdsnutil_SOURCES} netutil.c)
  set(libdsnutil_HEADERS ${libdsnutil_HEADERS} netutil.h)
endif(NOT WIN32)

#
# Tests for CSV parser classes
if(BUILD_TESTING AND WANT_CSV)
  add_executable(test-csv.Row csv/test-Row.cc)
  target_link_libraries(test-csv.Row dsnutil-csv)
  add_test(csv.Row test-csv.Row
    "${CMAKE_CURRENT_SOURCE_DIR}/csv/test.csv")

  add_executable(test-csv.Iterator csv/test-Iterator.cc)
  target_link_libraries(test-csv.Iterator dsnutil-csv)
  add_test(csv.Iterator test-csv.Iterator
    "${CMAKE_CURRENT_SOURCE_DIR}/csv/test.csv")
endif(BUILD_TESTING AND WANT_CSV)

#
# Check for MPI libraries and build utility classes if they're found
#if(WANT_MPI)
#find_package(MPI)
#  if(MPI_FOUND AND LOG4CPP_FOUND AND WANT_LOG4CPP)
#    # make sure mpicc/mpicxx get used instead of the regular compiler
#    set(CMAKE_C_COMPILER ${MPI_C_COMPILER})
#    set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
#
#    add_library(dsnutil-mpi mpi/Job.cc mpi/BSend.cc mpi/RSend.cc mpi/SSend.cc mpi/Send.cc mpi/ReceiveFunctor.cc)
#    set(libdsnutil_HEADERS ${libdsnutil_HEADERS} mpi.hh)
#
#    if(BUILD_TESTING AND WANT_LOG4CPP)
#      add_executable(test-mpi test-mpi.cc)
#      target_link_libraries(test-mpi dsnutil dsnutil-mpi)
#      target_link_libraries(test-mpi ${LOG4CPP_LIBRARIES})
#      add_test(test-mpi test-mpi)
#    endif(BUILD_TESTING AND WANT_LOG4CPP)

#    install(TARGETS dsnutil-mpi ARCHIVE DESTINATION lib)
#  endif(MPI_FOUND AND LOG4CPP_FOUND AND WANT_LOG4CPP)
#endif(WANT_MPI)


#
# libdsnutil main build target
add_library(dsnutil ${libdsnutil_SOURCES})
if(CPPCHECK_FOUND)
  add_cppcheck(dsnutil STYLE)
endif(CPPCHECK_FOUND)

#
# Installation targets for libdsnutil static library and header files
install(TARGETS dsnutil ARCHIVE DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h DESTINATION include/libdsnutil)
install(FILES ${libdsnutil_HEADERS} DESTINATION include/libdsnutil)


if(BUILD_TESTING)
  #
  # Tests for netutil.[ch]
  if(NOT WIN32)
    add_executable(test-netutil test-netutil.cc)
    target_link_libraries(test-netutil dsnutil)
    if(LOG4CPP_FOUND AND WANT_LOG4CPP)
      target_link_libraries(test-netutil ${LOG4CPP_LIBRARIES})
    endif(LOG4CPP_FOUND AND WANT_LOG4CPP)
    add_test(netutil-get_in_addr test-netutil "1")
    add_test(netutil-socketUDP test-netutil "2")
  endif(NOT WIN32)
endif(BUILD_TESTING)

#
# Add 'doc' target to build if Doxygen is installed
if(NOT DSNUTIL_SUBPROJECT)
  include(DocTarget)
endif(NOT DSNUTIL_SUBPROJECT)
