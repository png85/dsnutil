libdsnutil README
(c) 2012 #das-system Networks

# General info
this library contains various little utility/helper functions that have been
used in other projects.


# Optional dependencies
 * MPI implementation
 * log4cpp logging library


# Contents
## Overview
csv/             - Simple CSV parser interface
mpi/             - Message Passing Interface utility classes
assert           - Implementation of assert(3) that throws an exception
intercept_stream - Intercept std::ostreams and re-route them through log4cpp
hexdump          - Write hex dumps of memory areas
Singleton        - Template to create singleton classes

## Libraries
+ libdsnutil-mpi - Utility classes for applications using the Message Passing Interface
+ libdsnutil-csv - CSV Parser classes
+ libdsnutil     - Various utility classes/functions that didn't fit into the above


# Building
Use CMake to configure and build as needed. Useful options might be:

+ WANT_MPI      - Compile MPI utility classes
+ WANT_CSV      - Compile CSV parser
+ WANT_LOG4CPP  - Enable log4cpp support

+ BUILD_TESTING - Build tests for all components


