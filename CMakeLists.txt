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

#
# CTest/CDash configuration; see CTestConfig.cmake for more info!
include(CTest)
enable_testing()
set(BUILDNAME "${BUILDNAME}" "Name of build on CDash dashboard")

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
set(libdsnutil_HEADERS hexdump.hh Singleton.hh assert.hh)


#
# Setup list of sources for our library
set(libdsnutil_SOURCES hexdump.cc)
if(WANT_CSV)
  set(libdsnutil_SOURCES ${libdsnutil_SOURCES} csv/Row.cc csv/Iterator.cc)
  set(libdsnutil_HEADERS ${libdsnutil_HEADERS} csv.hh)
endif(WANT_CSV)

if(NOT WIN32)
  set(libdsnutil_SOURCES ${libdsnutil_SOURCES} netutil.c)
  set(libdsnutil_HEADERS ${libdsnutil_HEADERS} netutil.h)
endif(NOT WIN32)

#
# Tests for CSV parser classes
if(BUILD_TESTING AND WANT_CSV)
  add_executable(test-csv.Row csv/test-Row.cc)
  target_link_libraries(test-csv.Row dsnutil)
  add_test(csv.Row test-csv.Row
    "${CMAKE_CURRENT_SOURCE_DIR}/csv/test.csv")

  add_executable(test-csv.Iterator csv/test-Iterator.cc)
  target_link_libraries(test-csv.Iterator dsnutil)
  add_test(csv.Iterator test-csv.Iterator
    "${CMAKE_CURRENT_SOURCE_DIR}/csv/test.csv")
endif(BUILD_TESTING AND WANT_CSV)

#
# Check for MPI libraries and build utility classes if they're found
if(WANT_MPI)
find_package(MPI)
  if(MPI_FOUND AND LOG4CPP_FOUND AND WANT_LOG4CPP)
    # make sure mpicc/mpicxx get used instead of the regular compiler
    set(CMAKE_C_COMPILER ${MPI_C_COMPILER})
    set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})

    # add sources/headers to main library target
    set(libdsnutil_SOURCES ${libdsnutil_SOURCES} mpi/Job.cc
  			 		       	 mpi/BSend.cc mpi/RSend.cc
					       	 mpi/SSend.cc mpi/Send.cc
					       	 mpi/ReceiveFunctor.cc)
    set(libdsnutil_HEADERS ${libdsnutil_HEADERS} mpi.hh)

    if(BUILD_TESTING AND WANT_LOG4CPP)
      add_executable(test-mpi test-mpi.cc)
      target_link_libraries(test-mpi dsnutil)
      target_link_libraries(test-mpi ${LOG4CPP_LIBRARIES})
      add_test(test-mpi test-mpi)
    endif(BUILD_TESTING AND WANT_LOG4CPP)
  endif(MPI_FOUND AND LOG4CPP_FOUND AND WANT_LOG4CPP)
endif(WANT_MPI)

#
# intercept_stream class to redirect std::ifstream through log4cpp
if(WANT_LOG4CPP AND LOG4CPP_FOUND)
  set(libdsnutil_SOURCES ${libdsnutil_SOURCES} intercept_stream.cc)
  set(libdsnutil_HEADERS ${libdsnutil_HEADERS} intercept_stream.hh)
  if(BUILD_TESTING)
    add_executable(test-intercept_stream test-intercept_stream.cc)
    target_link_libraries(test-intercept_stream dsnutil)
    target_link_libraries(test-intercept_stream ${LOG4CPP_LIBRARIES})
    add_test(intercept_stream test-intercept_stream)
  endif(BUILD_TESTING)
endif(WANT_LOG4CPP AND LOG4CPP_FOUND)

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
  # Tests for hexdump
  add_executable(test-hexdump test-hexdump.cc)
  target_link_libraries(test-hexdump dsnutil)
  if(LOG4CPP_FOUND AND WANT_LOG4CPP)
    target_link_libraries(test-hexdump ${LOG4CPP_LIBRARIES})
  endif(LOG4CPP_FOUND AND WANT_LOG4CPP)
  add_test(hexdump.ostream test-hexdump "1")
  if(LOG4CPP_FOUND AND WANT_LOG4CPP)
    add_test(hexdump.log4cpp test-hexdump "2")
  endif(LOG4CPP_FOUND AND WANT_LOG4CPP)

  #
  # Test for throwing assert(3) from assert.hh
  add_executable(test-assert test-assert.cc)
  add_test(assert.hh test-assert)


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
