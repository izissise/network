# - Try to find enet
# Once done this will define
#
# ENET_FOUND - system has enet
# ENet_INCLUDE_DIRS - the enet include directory
# ENet_LIBRARIES - the libraries needed to use enet
#
# $ENET_ROOT is an environment variable used for finding enet.
#


FIND_PATH(ENet_INCLUDE_DIRS enet/enet.h
			    PATHS
			    ${ENV}${ENET_ROOT}
			    /usr/local
			    /usr
			    PATH_SUFFIXES include
			    )

FIND_LIBRARY(ENet_LIBRARY
				NAMES enet
				PATHS
				${ENV}${ENET_ROOT}
				/usr/local
				/usr
				PATH_SUFFIXES lib
				)

# handle the QUIETLY and REQUIRED arguments and set ENET_FOUND to TRUE if
# all listed variables are TRUE

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ENet DEFAULT_MSG ENet_LIBRARY ENet_INCLUDE_DIRS)

IF (ENET_FOUND)
   IF(WIN32)
	SET(WINDOWS_ENET_DEPENDENCIES "ws2_32" "winmm")
	SET(ENet_LIBRARIES ${ENet_LIBRARY} ${WINDOWS_ENET_DEPENDENCIES})
   ELSE()
	SET(ENet_LIBRARIES ${ENet_LIBRARY})
   ENDIF(WIN32)
ELSE()
	MESSAGE(WARNING "ENet library isn't found")
ENDIF (ENET_FOUND)

MARK_AS_ADVANCED(ENet_LIBRARY ENet_LIBRARIES ENet_INCLUDE_DIRS)