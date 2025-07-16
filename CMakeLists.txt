cmake_minimum_required(VERSION 3.10.0)
project(YUVPlayer)

set(CMAKE_CXX_STANDARD 14)

link_directories("/usr/local/lib/")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring for Linux")
    include_directories("/usr/local/include/SDL2/" "/usr/local/include/wx-3.2/" "/usr/local/lib/wx/include/gtk2-unicode-static-3.2")
    set(CMAKE_CXX_FLAGS "-I/usr/local/lib/wx/include/gtk2-unicode-static-3.2 -I/usr/local/include/wx-3.2 -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -pthread")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Configuring for macOS")
    include_directories("/usr/local/include/wx-3.2/" "/usr/local/lib/wx/include/osx_cocoa-unicode-3.2")
    find_package(SDL2 REQUIRED COMPONENTS SDL2)
    set(CMAKE_CXX_FLAGS "-DWXUSINGDLL -D__WXOSX_COCOA__ -D__WXMAC__ -D__WXOSX__")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Configuring for Windows")
    set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SDL")
    set(wxWidgets_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/wxWidgets")
    
    include_directories("${SDL2_ROOT}/include")
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(wxWidgets_CONFIGURATION mswud)
        set(wxWidgets_USE_DEBUG ON)
    else()
        set(wxWidgets_CONFIGURATION mswu)
        set(wxWidgets_USE_DEBUG OFF)
    endif()
    
    find_package(wxWidgets REQUIRED COMPONENTS core base)
    if(wxWidgets_FOUND)
        include(${wxWidgets_USE_FILE})
    endif()
    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    message(STATUS "Unsupported OS ${CMAKE_SYSTEM_NAME}")
endif()

aux_source_directory(. SRCS)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Add resource file for Windows
    set(RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/YUVPlayer.rc")
    list(APPEND SRCS ${RESOURCE_FILES})
endif()

add_executable(YUVPlayer WIN32 ${SRCS})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(YUVPlayer PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(YUVPlayer PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring for Linux")
    target_link_libraries(${PROJECT_NAME} SDL2 wx_gtk2u_xrc-3.2 wx_gtk2u_qa-3.2 wx_baseu_net-3.2 wx_gtk2u_html-3.2 wx_gtk2u_core-3.2 wx_baseu_xml-3.2 wx_baseu-3.2 gthread-2.0 X11 Xxf86vm SM gtk-x11-2.0 gdk-x11-2.0 pangocairo-1.0 atk-1.0 cairo gdk_pixbuf-2.0 gio-2.0 pangoft2-1.0 pango-1.0 gobject-2.0 glib-2.0 harfbuzz fontconfig freetype wxscintilla-3.2 expat pcre2-32 png jpeg tiff jbig z m)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(${PROJECT_NAME} pthread wx_osx_cocoau_xrc-3.2 wx_osx_cocoau_html-3.2 wx_osx_cocoau_qa-3.2 wx_osx_cocoau_core-3.2 wx_baseu_xml-3.2 wx_baseu_net-3.2 wx_baseu-3.2 SDL2::SDL2)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_library(SDL2_LIBRARY SDL2 HINTS "${SDL2_ROOT}/lib/Win64")
    
    if(SDL2_LIBRARY)
        target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
    else()
        message(FATAL_ERROR "SDL2 library not found")
    endif()
    
    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
    
    target_link_libraries(${PROJECT_NAME} 
        user32 gdi32 winspool comdlg32 advapi32 shell32 
        ole32 oleaut32 uuid odbc32 odbccp32
        setupapi imm32 cfgmgr32 winmm version
    )
else()
    message(STATUS "Unsupported OS ${CMAKE_SYSTEM_NAME}")
endif()
