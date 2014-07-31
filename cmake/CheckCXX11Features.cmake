# Checks for C++11 features
#  CXX11_FEATURE_LIST - a list containing all supported features
#  HAS_CXX11_AUTO               - auto keyword
#  HAS_CXX11_NULLPTR            - nullptr
#  HAS_CXX11_LAMBDA             - lambdas
#  HAS_CXX11_STATIC_ASSERT      - static_assert()
#  HAS_CXX11_RVALUE_REFERENCES  - rvalue references
#  HAS_CXX11_DECLTYPE           - decltype keyword
#  HAS_CXX11_CSTDINT_H          - cstdint header
#  HAS_CXX11_LONG_LONG          - long long signed & unsigned types
#  HAS_CXX11_VARIADIC_TEMPLATES - variadic templates
#  HAS_CXX11_CONSTEXPR          - constexpr keyword
#  HAS_CXX11_SIZEOF_MEMBER      - sizeof() non-static members
#  HAS_CXX11_FUNC               - __func__ preprocessor constant
#
# Original script by Rolf Eike Beer
# Modifications by Andreas Weis
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

SET(CHECK_CXX11_OLD_CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
IF(CMAKE_COMPILER_IS_GNUCXX)
	SET(CMAKE_CXX_FLAGS "-std=c++0x")
endif()

MACRO(CXX11_CHECK_FEATURE FEATURE_NAME FEATURE_NUMBER RESULT_VAR)
	IF (NOT DEFINED ${RESULT_VAR})
		SET(_bindir "${CMAKE_CURRENT_BINARY_DIR}/cxx11/cxx11_${FEATURE_NAME}")

		IF (${FEATURE_NUMBER})
			SET(_SRCFILE_BASE ${CMAKE_CURRENT_LIST_DIR}/c++11-test-${FEATURE_NAME}-N${FEATURE_NUMBER})
			SET(_LOG_NAME "\"${FEATURE_NAME}\" (N${FEATURE_NUMBER})")
		ELSE (${FEATURE_NUMBER})
			SET(_SRCFILE_BASE ${CMAKE_CURRENT_LIST_DIR}/c++11-test-${FEATURE_NAME})
			SET(_LOG_NAME "\"${FEATURE_NAME}\"")
		ENDIF (${FEATURE_NUMBER})
		MESSAGE(STATUS "Checking C++11 support for ${_LOG_NAME}")

		SET(_SRCFILE "${_SRCFILE_BASE}.cpp")
		SET(_SRCFILE_FAIL "${_SRCFILE_BASE}_fail.cpp")
		SET(_SRCFILE_FAIL_COMPILE "${_SRCFILE_BASE}_fail_compile.cpp")

		IF (CROSS_COMPILING)
			try_compile(${RESULT_VAR} "${_bindir}" "${_SRCFILE}")
			IF (${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL})
				try_compile(${RESULT_VAR} "${_bindir}_fail" "${_SRCFILE_FAIL}")
			ENDIF (${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL})
		ELSE (CROSS_COMPILING)
			try_run(_RUN_RESULT_VAR _COMPILE_RESULT_VAR
					"${_bindir}" "${_SRCFILE}")
			IF (_COMPILE_RESULT_VAR AND NOT _RUN_RESULT_VAR)
				SET(${RESULT_VAR} TRUE)
			ELSE (_COMPILE_RESULT_VAR AND NOT _RUN_RESULT_VAR)
				SET(${RESULT_VAR} FALSE)
			ENDIF (_COMPILE_RESULT_VAR AND NOT _RUN_RESULT_VAR)
			IF (${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL})
				try_run(_RUN_RESULT_VAR _COMPILE_RESULT_VAR
						"${_bindir}_fail" "${_SRCFILE_FAIL}")
				IF (_COMPILE_RESULT_VAR AND _RUN_RESULT_VAR)
					SET(${RESULT_VAR} TRUE)
				ELSE (_COMPILE_RESULT_VAR AND _RUN_RESULT_VAR)
					SET(${RESULT_VAR} FALSE)
				ENDIF (_COMPILE_RESULT_VAR AND _RUN_RESULT_VAR)
			ENDIF (${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL})
		ENDIF (CROSS_COMPILING)
		IF (${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL_COMPILE})
			try_compile(_TMP_RESULT "${_bindir}_fail_compile" "${_SRCFILE_FAIL_COMPILE}")
			IF (_TMP_RESULT)
				SET(${RESULT_VAR} FALSE)
			ELSE (_TMP_RESULT)
				SET(${RESULT_VAR} TRUE)
			ENDIF (_TMP_RESULT)
		ENDIF (${RESULT_VAR} AND EXISTS ${_SRCFILE_FAIL_COMPILE})

		IF (${RESULT_VAR})
			MESSAGE(STATUS "Checking C++11 support for ${_LOG_NAME} -- works")
			LIST(APPEND CXX11_FEATURE_LIST ${RESULT_VAR})
		ELSE (${RESULT_VAR})
			MESSAGE(STATUS "Checking C++11 support for ${_LOG_NAME} -- not supported")
		ENDIF (${RESULT_VAR})
		SET(${RESULT_VAR} ${${RESULT_VAR}} CACHE INTERNAL "C++11 support for ${_LOG_NAME}")
	ENDIF (NOT DEFINED ${RESULT_VAR})
ENDMACRO(CXX11_CHECK_FEATURE)

CXX11_CHECK_FEATURE("auto"               2546 HAS_CXX11_AUTO)
CXX11_CHECK_FEATURE("nullptr"            2431 HAS_CXX11_NULLPTR)
CXX11_CHECK_FEATURE("lambda"             2927 HAS_CXX11_LAMBDA)
CXX11_CHECK_FEATURE("static_assert"      1720 HAS_CXX11_STATIC_ASSERT)
CXX11_CHECK_FEATURE("rvalue_references"  2118 HAS_CXX11_RVALUE_REFERENCES)
CXX11_CHECK_FEATURE("decltype"           2343 HAS_CXX11_DECLTYPE)
CXX11_CHECK_FEATURE("cstdint"            ""   HAS_CXX11_CSTDINT_H)
CXX11_CHECK_FEATURE("long_long"          1811 HAS_CXX11_LONG_LONG)
CXX11_CHECK_FEATURE("variadic_templates" 2555 HAS_CXX11_VARIADIC_TEMPLATES)
CXX11_CHECK_FEATURE("constexpr"          2235 HAS_CXX11_CONSTEXPR)
CXX11_CHECK_FEATURE("sizeof_member"      2253 HAS_CXX11_SIZEOF_MEMBER)
CXX11_CHECK_FEATURE("__func__"           2340 HAS_CXX11_FUNC)
CXX11_CHECK_FEATURE("std_array"          ""   HAS_CXX11_STD_ARRAY)
CXX11_CHECK_FEATURE("initializer_list"   ""   HAS_CXX11_INITIALIZER_LIST)
CXX11_CHECK_FEATURE("all_of"             ""   HAS_CXX11_ALL_OF)
CXX11_CHECK_FEATURE("any_of"             ""   HAS_CXX11_ANY_OF)
CXX11_CHECK_FEATURE("none_of"            ""   HAS_CXX11_NONE_OF)
CXX11_CHECK_FEATURE("range-based_for"    ""   HAS_CXX11_RANGE_BASED_FOR)

SET(CXX11_FEATURE_LIST ${CXX11_FEATURE_LIST} CACHE STRING "C++11 feature support list")
MARK_AS_ADVANCED(FORCE CXX11_FEATURE_LIST)

SET(CMAKE_CXX_FLAGS ${CHECK_CXX11_OLD_CMAKE_CXX_FLAGS})
UNSET(CHECK_CXX11_OLD_CMAKE_CXX_FLAGS)

