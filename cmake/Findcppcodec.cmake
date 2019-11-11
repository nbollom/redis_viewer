# Try to find cppcodec
# Once done, this will define
#
# CPPCODEC_FOUND        - system has cppcodec
# CPPCODEC_INCLUDE_DIRS - cppcodec include directories
#
# CPPCODEC_ROOT_DIR may be defined as a hint for where to look

if(CPPCODEC_INCLUDE_DIRS AND CPPCODEC_LIBRARIES)
    set(CPPCODEC_FIND_QUIETLY TRUE)
    add_definitions(-DHAVE_CPPCODEC)
else()
    find_path(
            CPPCODEC_INCLUDE_DIR
            NAMES cppcodec/base64_rfc4648.hpp
            HINTS ${CPPCODEC_ROOT_DIR}
            PATH_SUFFIXES include)

    set(CPPCODEC_INCLUDE_DIRS ${CPPCODEC_INCLUDE_DIR})

    include (FindPackageHandleStandardArgs)
    find_package_handle_standard_args(
            cppcodec DEFAULT_MSG CPPCODEC_INCLUDE_DIR)

    add_definitions(-DHAVE_CPPCODEC)
    mark_as_advanced(CPPCODEC_INCLUDE_DIR)
endif()
