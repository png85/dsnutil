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
  set(CPACK_PACKAGE_VERSION_PATCH "2")
  include(CPack)
endif(NOT DSNUTIL_SUBPROJECT)

#
# Check for log4cpp framework
find_package(Log4cpp)

#
# Check for cppcheck and provide macros to run it on other build targets
include(CppCheckTargets)

#
# Prepare config.h
configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

#
# Setup list of header files for system-wide installation
set(libdsnutil_HEADERS netutil.h hexdump.hh Singleton.hh csv.hh)

#
# Setup list of sources for our library
set(libdsnutil_SOURCES netutil.c hexdump.cc csv/Row.cc csv/Iterator.cc)

add_executable(test-csv.Row csv/test-Row.cc)
target_link_libraries(test-csv.Row dsnutil)
add_test(csv.Row test-csv.Row "${CMAKE_SOURCE_DIR}/csv/test.csv")

if(LOG4CPP_FOUND)
  # if we have log4cpp available we build an overloaded hexdump()
  # that supports dumping to a log4cpp::CategoryStream
  set(libdsnutil_SOURCES ${libdsnutil_SOURCES} hexdump-log4cpp.cc)
  include_directories(${LOG4CPP_INCLUDE_DIR})
endif(LOG4CPP_FOUND)

#
# Check for MPI libraries and build utility classes if they're found
find_package(MPI)
if(MPI_FOUND AND LOG4CPP_FOUND)
  # make sure mpicc/mpicxx get used instead of the regular compiler
  set(CMAKE_C_COMPILER ${MPI_C_COMPILER})
  set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})

  # add sources/headers to main library target
  set(libdsnutil_SOURCES ${libdsnutil_SOURCES} mpi/Job.cc
  			 		       mpi/BSend.cc mpi/RSend.cc
					       mpi/SSend.cc mpi/Send.cc
					       mpi/ReceiveFunctor.cc)
  set(libdsnutil_HEADERS ${libdsnutil_HEADERS} mpi.hh)

  add_executable(test-mpi test-mpi.cc)
  target_link_libraries(test-mpi dsnutil)
  target_link_libraries(test-mpi ${LOG4CPP_LIBRARIES})
  add_test(test-mpi test-mpi)
endif(MPI_FOUND AND LOG4CPP_FOUND)

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


#
# Tests for hexdump
add_executable(test-hexdump test-hexdump.cc)
target_link_libraries(test-hexdump dsnutil)
if(LOG4CPP_FOUND)
  target_link_libraries(test-hexdump ${LOG4CPP_LIBRARIES})
endif(LOG4CPP_FOUND)
add_test(test-hexdump test-hexdump "1")
if(LOG4CPP_FOUND)
  add_test(test-hexdump-log4cpp test-hexdump "2")
endif(LOG4CPP_FOUND)


#
# Tests for netutil.[ch]
add_executable(test-netutil test-netutil.cc)
target_link_libraries(test-netutil dsnutil)
if(LOG4CPP_FOUND)
  target_link_libraries(test-netutil ${LOG4CPP_LIBRARIES})
endif(LOG4CPP_FOUND)
add_test(netutil-get_in_addr test-netutil "1")
add_test(netutil-socketUDP test-netutil "2")

#
# Add 'doc' target to build if Doxygen is installed
if(NOT DSNUTIL_SUBPROJECT)
  include(DocTarget)
endif(NOT DSNUTIL_SUBPROJECT)
