## http://www.cmake.org/cmake/help/v2.8.12/cmake.html#module:FindQt4
## http://www.cmake.org/cmake/help/v3.0/manual/cmake-qt.7.html#introduction

IF (WIN32)
SET (APHID_DIR "D:/aphid")
elseif (APPLE)
IF (EXISTS /Users/jollyroger)
	SET (APPLE_HOME /Users/jollyroger)
ELSEIF (EXISTS /Users/jianzhang)
	SET (APPLE_HOME /Users/jianzhang)
ENDIF ()

SET (APHID_DIR ${APPLE_HOME}/aphid)
else()
SET (APHID_DIR "~/aphid")
endif ()

if(WIN32)
add_definitions (-DWIN32)
endif()

include_directories (${APHID_DIR}/shared)

SET (EXECUTABLE_OUTPUT_PATH ../bin)

