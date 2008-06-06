PROJECT(BREEDOU)

FIND_PACKAGE(Qt4 REQUIRED) # find and setup Qt4 for this project

# SET(CMAKE_VERBOSE_MAKEFILE TRUE)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTOPENGL TRUE)

INCLUDE(${QT_USE_FILE})

IF(WIN32)
	INCLUDE_DIRECTORIES(${CEB_SOURCE_DIR}/CONTRIB/lua-5.1/src)
ELSE(WIN32)
	INCLUDE_DIRECTORIES(/usr/include/lua5.1)
ENDIF(WIN32)

SET(BREEDOU_SRCS
	src/main.cpp
	src/main_window.cpp
	src/battleground.cpp
	src/robot.cpp
	src/keyboard.cpp
	src/item.cpp
	src/bullet.cpp
	src/moving_item.cpp
	src/box.cpp
	src/texture_manager.cpp
)

SET(BREEDOU_UIS
	src/main_window.ui
)

QT4_WRAP_UI(BREEDOU_UIS_H ${BREEDOU_UIS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(BREEDOU_MOCH
	src/main_window.h
	src/battleground.h
	src/keyboard.h
)

ADD_DEFINITIONS(${QT_DEFINITIONS} -DQT_NO_DEBUG)

QT4_WRAP_CPP(BREEDOU_MOC ${BREEDOU_MOCH})

QT4_ADD_RESOURCES(BREEDOU_RES breedou.qrc)

IF(MINGW)
	# resource compilation for MinGW
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/breedou_rc.o
		COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/breedou.rc
		-o ${CMAKE_CURRENT_BINARY_DIR}/breedou_rc.o )
	SET(BREEDOU_SRCS ${BREEDOU_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/breedou_rc.o)
ELSE(MINGW)
	SET(BREEDOU_SRCS ${BREEDOU_SRCS} breedou.rc)
ENDIF(MINGW)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

IF(WIN32)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
ENDIF(WIN32)

ADD_EXECUTABLE(breedou ${BREEDOU_SRCS} ${BREEDOU_UIS_H} ${BREEDOU_MOC} ${BREEDOU_RES})

IF(UNIX)
	TARGET_LINK_LIBRARIES(
		breedou ${QT_LIBRARIES} /usr/lib/liblua5.1.so
	)
ELSEIF(WIN32)
	TARGET_LINK_LIBRARIES(
		breedou ${QT_LIBRARIES} ${BREEDOU_SOURCE_DIR}/CONTRIB/lua-5.1/src/lua51.dll
            -lgdi32 -lcomdlg32 -loleaut32 -limm32 -lwinmm -lwinspool -lmsimg32 -lkernel32 -luser32 -lshell32 -luuid -lole32 -ladvapi32 -lws2_32
	)
ENDIF(UNIX)

#INSTALL(TARGETS breedou
#	RUNTIME DESTINATION bin)
#INSTALL(DIRECTORY share/breedou
#        DESTINATION share
#        PATTERN .svn
#        EXCLUDE)
