cmake_minimum_required(VERSION 3.30.3)

# Specify the compiler for Linux builds
if (UNIX)
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_MAJOR_VERSION 1)
set(PROJECT_MINOR_VERSION 0)
set(PROJECT_PATCH_VERSION 0)
set(PROJECT_VERSION "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION}")

project("TestProject" VERSION ${PROJECT_VERSION})

if (MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()
endif()

# Specify resource file for Windows
if (WIN32)
    set(RESOURCE_FILE "${CMAKE_SOURCE_DIR}/files/icon.rc")
endif()

# Project files
set(CORE_DIR "${CMAKE_SOURCE_DIR}/include/core")
set(GRAPHICS_DIR "${CMAKE_SOURCE_DIR}/include/graphics")

# External files
set(EXT_WINDOW_DIR "${CMAKE_SOURCE_DIR}/_external_shared/KalaWindow")
set(EXT_CRASH_DIR "${CMAKE_SOURCE_DIR}/_external_shared/KalaCrashHandler")
set(EXT_UTILS_DIR "${CMAKE_SOURCE_DIR}/_external_shared/KalaUtils")

# Add KalaWindow
if (WIN32)
	if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/release/KalaWindow.lib")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/debug/KalaWindowD.lib")
	endif()
else()
	if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/release/libKalaWindow.so")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/debug/libKalaWindowD.so")
	endif()
endif()

# Add KalaCrashHandler
if (WIN32)
	if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/release/KalaCrashHandler.lib")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/debug/KalaCrashHandlerD.lib")
	endif()
else()
	if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/release/libKalaCrashHandler.so")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/debug/libKalaCrashHandlerD.so")
	endif()
endif()

# Add KalaUtils
if (WIN32)
	if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		set(UTILS_LIBRARY_PATH "${EXT_UTILS_DIR}/release/KalaUtils.lib")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(UTILS_LIBRARY_PATH "${EXT_UTILS_DIR}/debug/KalaUtilsD.lib")
	endif()
else()
	if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		set(UTILS_LIBRARY_PATH "${EXT_UTILS_DIR}/release/libKalaUtils.so")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(UTILS_LIBRARY_PATH "${EXT_UTILS_DIR}/debug/libKalaUtilsD.so")
	endif()
endif()

# Add your source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
	${CMAKE_SOURCE_DIR}/src/*.cpp
	${CMAKE_SOURCE_DIR}/src/*/*.cpp
)

# Build as Project executable
add_executable(TestProject ${SOURCE_FILES})

if (WIN32)
	set_source_files_properties(${RESOURCE_FILE} PROPERTIES LANGUAGE RC)
	target_sources(TestProject PRIVATE ${RESOURCE_FILE})
endif()

# Forces the name to be TestProject
set_target_properties(TestProject PROPERTIES OUTPUT_NAME "TestProject")

target_compile_features(TestProject PRIVATE cxx_std_20)

target_include_directories(TestProject PRIVATE  
	${CORE_DIR}
	${GRAPHICS_DIR}
	
	#External headers
	${EXT_WINDOW_DIR}
	${EXT_CRASH_DIR}
	${EXT_UTILS_DIR}
)

# Apply all directories to target
target_include_directories(TestProject PRIVATE ${Project_INCLUDE_DIRS})

if (WIN32)
	target_link_libraries(TestProject PRIVATE 
		${WINDOW_LIBRARY_PATH}
		${CRASH_LIBRARY_PATH}
		opengl32)
else()
	find_package(Wayland QUIET)
	find_package(X11 QUIET)
	if (Wayland_FOUND)
		target_link_libraries(TestProject PRIVATE 
			${WINDOW_LIBRARY_PATH}
			${CRASH_LIBRARY_PATH}
			Wayland::Client)
	elseif (X11_FOUND)
		target_link_libraries(TestProject PRIVATE 
			${WINDOW_LIBRARY_PATH}
			${CRASH_LIBRARY_PATH}
			${X11_LIBRARIES})
	endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	# Set the subsystem to WINDOWS
	set_target_properties(TestProject PROPERTIES
		WIN32_EXECUTABLE TRUE
	)

# Set the entry point (MSVC-specific)
	if (MSVC)
		set_target_properties(TestProject PROPERTIES
		LINK_FLAGS "/ENTRY:mainCRTStartup"
		)
	endif()
endif()

# Set the folder inside the install folder where the exe will be placed for this project
set(CMAKE_INSTALL_BINDIR bin)
install(TARGETS TestProject DESTINATION ${CMAKE_INSTALL_BINDIR})

# Copy "files" directory to the install directory after build
add_custom_command(TARGET TestProject POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E remove_directory
    $<TARGET_FILE_DIR:TestProject>/files
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/files $<TARGET_FILE_DIR:TestProject>/files
)

# Copy all DLLs to the install directory after build
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	set(DLL_ORIGIN_DIR "${CMAKE_SOURCE_DIR}/files/external dlls/release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DLL_ORIGIN_DIR "${CMAKE_SOURCE_DIR}/files/external dlls/debug")
endif()
set(DLL_TARGET_DIR "$<TARGET_FILE_DIR:TestProject>")

file(GLOB DLL_FILES "${DLL_ORIGIN_DIR}/*")
	
foreach(DLL_FILE ${DLL_FILES})
    get_filename_component(DLL_NAME ${DLL_FILE} NAME)
    add_custom_command(TARGET TestProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${DLL_FILE} 
            ${DLL_TARGET_DIR}/${DLL_NAME}
    )
endforeach()

set(COPY_TARGETS LICENSE.md README.md LIBRARIES.md)
foreach(FILE ${COPY_TARGETS})
  add_custom_command(TARGET TestProject POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E remove
	$<TARGET_FILE_DIR:TestProject>/${FILE}
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/${FILE}
	$<TARGET_FILE_DIR:TestProject>/${FILE}
  )
endforeach()

# Include CPack to enable packaging
include(CPack)
