cmake_minimum_required(VERSION 3.29.2)

project("TestProject" VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Renderer Selection ===
if (NOT DEFINED KALAWINDOW_RENDERER)
    message(FATAL_ERROR "[KALATESTPROJECT] You must define -DKALAWINDOW_RENDERER=opengl or vulkan")
endif()

string(TOLOWER "${KALAWINDOW_RENDERER}" KALAWINDOW_RENDERER)
message(STATUS "[KALATESTPROJECT] Selected renderer: ${KALAWINDOW_RENDERER}")

set(ENABLE_OPENGL FALSE)
set(ENABLE_VULKAN FALSE)

if(KALAWINDOW_RENDERER STREQUAL "opengl")
    set(ENABLE_OPENGL TRUE)
elseif(KALAWINDOW_RENDERER STREQUAL "vulkan")
    set(ENABLE_VULKAN TRUE)
else()
    message(FATAL_ERROR "[KALATESTPROJECT] Invalid renderer: '${KALAWINDOW_RENDERER}'. Must be 'vulkan' or 'opengl'.")
endif()

# === Platform Detection ===
if (WIN32)
    message(STATUS "[KALATESTPROJECT] Platform = Windows")
elseif(UNIX)
    find_package(X11 REQUIRED)
    message(STATUS "[KALATESTPROJECT] Platform = Linux/Unix with X11")
else()
    message(FATAL_ERROR "[KALATESTPROJECT] Unsupported platform. Must be WIN32 or UNIX with X11.")
endif()

# === Build Type Detection ===
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(IS_DEBUG TRUE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(IS_RELEASE TRUE)
else()
    message(FATAL_ERROR "Unknown CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'! Must be Debug, Release, or RelWithDebInfo.")
endif()

# === MSVC Runtime Config ===
if (MSVC)
    if(IS_DEBUG)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()
endif()

# === Optional Resource File ===
if (WIN32)
    set(RESOURCE_FILE "${CMAKE_SOURCE_DIR}/files/icon.rc")
endif()

# === Directory Setup ===
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(EXT_WINDOW_DIR "${CMAKE_SOURCE_DIR}/_external_shared/KalaWindow")
#set(EXT_CRASH_DIR "${CMAKE_SOURCE_DIR}/_external_shared/KalaCrashHandler")

# === Dependencies ===
if (ENABLE_OPENGL)
    find_package(OpenGL REQUIRED)
endif()
if (ENABLE_VULKAN)
    find_package(Vulkan REQUIRED)
endif()

# === Library Paths ===
if (WIN32)
    if(IS_RELEASE)
        if (ENABLE_OPENGL)
            set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/release_opengl/KalaWindow_opengl.lib")
        endif()
        if (ENABLE_VULKAN)
            set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/release_vulkan/KalaWindow_vulkan.lib")
        endif()
        #set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/release/KalaCrashHandler.lib")
    else()
        if (ENABLE_OPENGL)
            set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/debug_opengl/KalaWindow_openglD.lib")
        endif()
        if (ENABLE_VULKAN)
            set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/debug_vulkan/KalaWindow_vulkanD.lib")
        endif()
        #set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/debug/KalaCrashHandlerD.lib")
    endif()
else()
    if(IS_RELEASE)
        if (ENABLE_OPENGL)
            set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/release_opengl/libKalaWindow_opengl.so")
        endif()
        if (ENABLE_VULKAN)
            set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/release_vulkan/libKalaWindow_vulkan.so")
        endif()
        #set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/release/libKalaCrashHandler.so")
    else()
        if (ENABLE_OPENGL)
            set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/debug_opengl/libKalaWindow_openglD.so")
        endif()
        if (ENABLE_VULKAN)
            set(WINDOW_LIBRARY_PATH "${EXT_WINDOW_DIR}/debug_vulkan/libKalaWindow_vulkanD.so")
        endif()
        #set(CRASH_LIBRARY_PATH "${EXT_CRASH_DIR}/debug/libKalaCrashHandlerD.so")
    endif()
endif()

# === Source Files ===
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*/*.cpp
)

# === Executable ===
add_executable(TestProject ${SOURCE_FILES})

if (WIN32 AND RESOURCE_FILE)
    #set_source_files_properties(${RESOURCE_FILE} PROPERTIES LANGUAGE RC)
    #target_sources(TestProject PRIVATE ${RESOURCE_FILE})
endif()

set_target_properties(TestProject PROPERTIES OUTPUT_NAME "TestProject")
target_compile_features(TestProject PRIVATE cxx_std_20)

# === Includes ===
target_include_directories(TestProject PRIVATE
	${INCLUDE_DIR}
    "${EXT_WINDOW_DIR}/include"
    #${EXT_CRASH_DIR}
)

# === Preprocessor Defines ===
if (WIN32)
    target_compile_definitions(TestProject PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

if (ENABLE_OPENGL)
    target_compile_definitions(TestProject PRIVATE KALAWINDOW_SUPPORT_OPENGL=1)
endif()

if (ENABLE_VULKAN)
    target_compile_definitions(TestProject PRIVATE KALAWINDOW_SUPPORT_VULKAN=1)
endif()

# === Linking ===
target_link_libraries(TestProject PRIVATE ${WINDOW_LIBRARY_PATH}) #${CRASH_LIBRARY_PATH})

if (WIN32)
    if (ENABLE_OPENGL)
        target_link_libraries(TestProject PRIVATE opengl32)
    endif()
    if (ENABLE_VULKAN)
        target_link_libraries(TestProject PRIVATE Vulkan::Vulkan)
    endif()
else()
    if (ENABLE_OPENGL)
        target_link_libraries(TestProject PRIVATE OpenGL::GL)
    endif()
    if (ENABLE_VULKAN)
        target_link_libraries(TestProject PRIVATE Vulkan::Vulkan)
    endif()
    target_link_libraries(TestProject PRIVATE ${X11_LIBRARIES})
endif()

# === Windows Subsystem for Release builds ===
if(IS_RELEASE AND WIN32)
    set_target_properties(TestProject PROPERTIES WIN32_EXECUTABLE TRUE)
    if (MSVC)
        set_target_properties(TestProject PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
    endif()
endif()

# === Installation ===
set(CMAKE_INSTALL_BINDIR bin)
install(TARGETS TestProject DESTINATION ${CMAKE_INSTALL_BINDIR})

# === Copy files directory ===
#add_custom_command(TARGET TestProject POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:TestProject>/files"
#    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/files" "$<TARGET_FILE_DIR:TestProject>/files"
#)

# === Copy external DLLs (Windows only) ===
if (WIN32)
	if (ENABLE_OPENGL)
	    if(IS_RELEASE)
			set(DLL_KALAWINDOW "${CMAKE_SOURCE_DIR}/_external_shared/KalaWindow/release_opengl")
		else()
			set(DLL_KALAWINDOW "${CMAKE_SOURCE_DIR}/_external_shared/KalaWindow/debug_opengl")
		endif()
	endif()
	if (ENABLE_VULKAN)
	    if(IS_RELEASE)
			set(DLL_KALAWINDOW "${CMAKE_SOURCE_DIR}/_external_shared/KalaWindow/release_vulkan")
		else()
			set(DLL_KALAWINDOW "${CMAKE_SOURCE_DIR}/_external_shared/KalaWindow/debug_vulkan")
		endif()
	endif()

    file(GLOB DLL_FILES 
		"${DLL_KALAWINDOW}/*.dll"
	)
    set(DLL_TARGET_DIR "$<TARGET_FILE_DIR:TestProject>")

    foreach(DLL_FILE ${DLL_FILES})
        get_filename_component(DLL_NAME ${DLL_FILE} NAME)
        add_custom_command(TARGET TestProject POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${DLL_FILE}" "${DLL_TARGET_DIR}/${DLL_NAME}"
        )
    endforeach()
endif()

# === Copy docs ===
#set(COPY_TARGETS LICENSE.md LIBRARIES.md)
#foreach(FILE ${COPY_TARGETS})
#  add_custom_command(TARGET TestProject POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E remove "$<TARGET_FILE_DIR:TestProject>/${FILE}"
#    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/${FILE}" #"$<TARGET_FILE_DIR:TestProject>/${FILE}"
#  )
#endforeach()

# === Package ===
include(CPack)
