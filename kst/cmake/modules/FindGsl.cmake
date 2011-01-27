
include(FindPkgConfig)

pkg_check_modules(GSL QUIET gsl)

if(GSL_INCLUDEDIR AND GSL_LIBRARIES)
	set(GSL_LIBRARY -L${GSL_LIBRARY_DIRS} ${GSL_LIBRARIES})
else()
	set(GSL_INCLUDEDIR GSL_INCLUDEDIR-NOTFOUND CACHE STRING "" FORCE)
	FIND_PATH(GSL_INCLUDEDIR gsl_version.h
		HINTS
		ENV GSL_DIR
		PATH_SUFFIXES include/gsl include
		PATHS
		)
	FIND_LIBRARY(GSL_LIBRARIES gsl 
		HINTS
		ENV GSL_DIR
		PATH_SUFFIXES lib
		PATHS
		)
endif()

#message(STATUS "GSL: ${GSL_INCLUDEDIR}")
#message(STATUS "GSL: ${GSL_LIBRARIES}")
IF(GSL_INCLUDEDIR AND GSL_LIBRARIES)
	MESSAGE(STATUS "Gsl found at ${GSL_INCLUDEDIR}")
	SET(GSL_INCLUDE_DIR ${GSL_INCLUDEDIR} ${GSL_INCLUDEDIR}/..)
    if (UNIX)
		SET(GSL_LIBRARIES ${GSL_LIBRARIES} m)
	endif()
	SET(gsl 1)
ELSE()
	MESSAGE(STATUS "Gsl not found.")
ENDIF()



