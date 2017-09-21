###########################################################################
# Try to find OpenBASN library.
#
# The user can specify an additional search path using the CMake variable
# OPENBASN_DIR

# First, search at the specific user path setting.
IF(NOT ("${OPENBASN_DIR}" STREQUAL ""))
    FIND_PATH(OPENBASN_INCLUDE_DIR
                 NAMES openbasn
                 PATHS ${OPENBASN_DIR}/include
                 NO_DEFAULT_PATH)

    FIND_LIBRARY(OPENBASN_LIBRARY
                 NAMES openbasn openbasn-static
                 PATHS ${OPENBASN_DIR}/lib
                 NO_DEFAULT_PATH)
ENDIF()
IF(   ("${OPENBASN_INCLUDE_DIR}" STREQUAL "OPENBASN_INCLUDE_DIR-NOTFOUND")
   OR ("${OPENBASN_DIR}" STREQUAL "") )
    MESSAGE(STATUS "Trying to find OpenBASN library in default paths.")
    # If not found, use the system's search paths.
    FIND_PATH(OPENBASN_INCLUDE_DIR 
                 NAMES openbasn
                 PATHS /usr/include
                       /usr/local/include)

    FIND_LIBRARY(OPENBASN_LIBRARY
                 NAMES openbasn openbasn-static
                 PATHS /usr/lib
                       /usr/lib64
                       /usr/local/lib
                       /usr/local/lib64)
ENDIF()

IF("${OPENBASN_INCLUDE_DIR}" STREQUAL "")
    MESSAGE(FATAL_ERROR "Could not find OpenBASN library.")
ELSE()
    SET (FOUND_OPENBASN 1)
ENDIF()

###########################################################################
# Artifacts based on libaala need to link against OpenDaVINCI.
FIND_PACKAGE (OpenDaVINCI REQUIRED)

###########################################################################
# Set linking libraries.
IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    SET(OPENBASN_LIBRARIES ${OPENDAVINCI_LIBRARIES}
                               ${OPENBASN_LIBRARY}
                               /usr/X11/lib/libglut.3.dylib)
ELSE()
    SET(OPENBASN_LIBRARIES ${OPENDAVINCI_LIBRARIES}
                               ${OPENBASN_LIBRARY})
ENDIF()

SET(OPENBASN_INCLUDE_DIRS ${OPENBASN_INCLUDE_DIR})

###########################################################################
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libaala DEFAULT_MSG
                                  OPENBASN_LIBRARY OPENBASN_INCLUDE_DIR)
MARK_AS_ADVANCED(OPENBASN_INCLUDE_DIR OPENBASN_LIBRARY)