###########################################################################
# Try to find OpenBASN library.
#
# The user can specify an additional search path using the CMake variable
# BSN_DIR

# First, search at the specific user path setting.
IF(NOT ("${BSN_DIR}" STREQUAL ""))
    FIND_PATH(BSN_INCLUDE_DIR
                 NAMES bsn
                 PATHS ${BSN_DIR}/include
                 NO_DEFAULT_PATH)

    FIND_LIBRARY(BSN_LIBRARY
                 NAMES bsn bsn-static
                 PATHS ${BSN_DIR}/lib
                 NO_DEFAULT_PATH)
ENDIF()
IF(   ("${BSN_INCLUDE_DIR}" STREQUAL "BSN_INCLUDE_DIR-NOTFOUND")
   OR ("${BSN_DIR}" STREQUAL "") )
    MESSAGE(STATUS "Trying to find OpenBASN library in default paths.")
    # If not found, use the system's search paths.
    FIND_PATH(BSN_INCLUDE_DIR 
                 NAMES bsn
                 PATHS /usr/include
                       /usr/local/include)

    FIND_LIBRARY(BSN_LIBRARY
                 NAMES bsn bsn-static
                 PATHS /usr/lib
                       /usr/lib64
                       /usr/local/lib
                       /usr/local/lib64)
ENDIF()

IF("${BSN_INCLUDE_DIR}" STREQUAL "")
    MESSAGE(FATAL_ERROR "Could not find OpenBASN library.")
ELSE()
    SET (FOUND_BSN 1)
ENDIF()

###########################################################################
# Artifacts based on libaala need to link against OpenDaVINCI.
FIND_PACKAGE (OpenDaVINCI REQUIRED)

###########################################################################
# Set linking libraries.
IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    SET(BSN_LIBRARIES ${OPENDAVINCI_LIBRARIES}
                               ${BSN_LIBRARY}
                               /usr/X11/lib/libglut.3.dylib)
ELSE()
    SET(BSN_LIBRARIES ${OPENDAVINCI_LIBRARIES}
                               ${BSN_LIBRARY})
ENDIF()

SET(BSN_INCLUDE_DIRS ${BSN_INCLUDE_DIR})

###########################################################################
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libaala DEFAULT_MSG
                                  BSN_LIBRARY BSN_INCLUDE_DIR)
MARK_AS_ADVANCED(BSN_INCLUDE_DIR BSN_LIBRARY)