# install runtime files

if(WIN32)
	install(FILES
		${QT_BINARY_DIR}/QtCore4.dll
		${QT_BINARY_DIR}/QtGui4.dll
		${QT_BINARY_DIR}/QtXml4.dll
		${QT_BINARY_DIR}/QtOpenGL4.dll
		${QT_BINARY_DIR}/QtSvg4.dll
		DESTINATION bin
		CONFIGURATIONS Release)
	install(FILES
		${QT_BINARY_DIR}/QtCored4.dll
		${QT_BINARY_DIR}/QtGuid4.dll
		${QT_BINARY_DIR}/QtXmld4.dll
		${QT_BINARY_DIR}/QtOpenGLd4.dll
		${QT_BINARY_DIR}/QtSvgd4.dll
		DESTINATION bin
		CONFIGURATIONS Debug)
endif()

if(MINGW)
	get_filename_component(MINGW_BIN_PATH ${CMAKE_CXX_COMPILER} PATH)
	install(FILES 
		${MINGW_BIN_PATH}/libgcc_s_dw2-1.dll
		${MINGW_BIN_PATH}/mingwm10.dll
		DESTINATION bin)
endif()


if(WIN32)
	set(color_folder    colors)
	set(tutorial_folder tutorial)
	set(font_folder fonts)
else()
	set(color_folder    share/config/colors)
	set(tutorial_folder share/apps/kst/tutorial)
	SET(font_folder     share/apps/kst/fonts)
endif()


# install data files
if(UNIX)
	if(NOT gyrodata)
		configure_file(${kst_dir}/misc/tutorial/gyrodata.dat.gz ${CMAKE_BINARY_DIR}/gyrodata.dat.gz COPYONLY)
		execute_process(WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND gunzip gyrodata.dat.gz)
		set(gyrodata ${CMAKE_BINARY_DIR}/gyrodata.dat CACHE STRING "gyrodata decompressed")
	endif()
endif()
file(GLOB dat_files ${kst_dir}/misc/tutorial/*.dat)
install(FILES ${dat_files} ${gyrodata} DESTINATION ${tutorial_folder})


# install colors
file(GLOB idl_files ${kst_dir}/misc/IDL*)
install(FILES ${idl_files} DESTINATION ${color_folder})


#install fonts
if(UNIX)
	file(GLOB fonts ${kst_dir}/misc/fonts/*.gz)
	install(FILES ${fonts} DESTINATION ${font_folder})
endif()
