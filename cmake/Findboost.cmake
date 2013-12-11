################################################################################
# Custom cmake module for CMAKE to find boost libraries
################################################################################
include(FindPackageHandleStandardArgs)

find_path(BOOST_H_PATH NAMES boost/version.hpp PATHS ${BOOST_ROOT}/include)

find_library(BOOST_SYSTEM_LIB NAMES boost_system boost_system-mt PATHS ${BOOST_ROOT})
find_library(BOOST_THREAD_LIB NAMES boost_thread boost_thread-mt PATHS ${BOOST_ROOT})
find_library(BOOST_REGEX_LIB NAMES boost_regex boost_regex-mt PATHS ${BOOST_ROOT})

mark_as_advanced(BOOST_H_PATH BOOST_SYSTEM_LIB BOOST_THREAD_LIB BOOST_REGEX_LIB)

find_package_handle_standard_args(BOOST DEFAULT_MSG BOOST_H_PATH BOOST_SYSTEM_LIB BOOST_THREAD_LIB BOOST_REGEX_LIB)


# set up output vars

if (BOOST_FOUND)
	set (BOOST_INCLUDE_DIR ${BOOST_H_PATH})
    set (BOOST_LIBRARIES ${BOOST_SYSTEM_LIB} ${BOOST_THREAD_LIB} ${BOOST_REGEX_LIB})
else()
    set (BOOST_LIBRARIES)
endif()


