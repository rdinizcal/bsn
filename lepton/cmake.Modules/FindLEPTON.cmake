###########################################################################
# Try to find LEPTON library.
#
# The user can specify an additional search path using the CMake variable
# LEPTON_DIR

# First, search at the specific user path setting.
IF(NOT ("${LEPTON_DIR}" STREQUAL ""))
    FIND_PATH(LEPTON_INCLUDE_DIR
                 NAMES lepton
                 PATHS ${LEPTON_DIR}/include
                 NO_DEFAULT_PATH)

    FIND_LIBRARY(LEPTON_LIBRARY
                 NAMES lepton lepton-static
                 PATHS ${LEPTON_DIR}/lib
                 NO_DEFAULT_PATH)
ENDIF()
IF(   ("${LEPTON_INCLUDE_DIR}" STREQUAL "LEPTON_INCLUDE_DIR-NOTFOUND")
   OR ("${LEPTON_DIR}" STREQUAL "") )
    MESSAGE(STATUS "Trying to find LEPTON library in default paths.")
    # If not found, use the system's search paths.
    FIND_PATH(LEPTON_INCLUDE_DIR 
                 NAMES lepton
                 PATHS /usr/include
                       /usr/local/include)

    FIND_LIBRARY(LEPTON_LIBRARY
                 NAMES lepton lepton-static
                 PATHS /usr/lib
                       /usr/lib64
                       /usr/local/lib
                       /usr/local/lib64)
ENDIF()

IF("${LEPTON_INCLUDE_DIR}" STREQUAL "")
    MESSAGE(FATAL_ERROR "Could not find LEPTON library.")
ELSE()
    SET (FOUND_LEPTON 1)
ENDIF()

###########################################################################
# Artifacts based on lepton need to link against OpenDaVINCI.
FIND_PACKAGE (OpenDaVINCI REQUIRED)

###########################################################################
# Set linking libraries.
IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    SET(LEPTON_LIBRARIES ${OPENDAVINCI_LIBRARIES}
                               ${LEPTON_LIBRARY}
                               /usr/X11/lib/libglut.3.dylib)
ELSE()
    SET(LEPTON_LIBRARIES ${OPENDAVINCI_LIBRARIES}
                               ${LEPTON_LIBRARY})
ENDIF()

SET(LEPTON_INCLUDE_DIRS ${LEPTON_INCLUDE_DIR})

###########################################################################
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(lepton DEFAULT_MSG
                                  LEPTON_LIBRARY LEPTON_INCLUDE_DIR)
MARK_AS_ADVANCED(LEPTON_INCLUDE_DIR LEPTON_LIBRARY)