cmake_minimum_required(VERSION 3.0.0)
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
else()
    message(STATUS "Unsupported OS ${CMAKE_SYSTEM_NAME}")
endif()

aux_source_directory(. SRCS)
add_executable(YUVPlayer ${SRCS})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring for Linux")
    target_link_libraries(${PROJECT_NAME} SDL2 wx_gtk2u_xrc-3.2 wx_gtk2u_qa-3.2 wx_baseu_net-3.2 wx_gtk2u_html-3.2 wx_gtk2u_core-3.2 wx_baseu_xml-3.2 wx_baseu-3.2 gthread-2.0 X11 Xxf86vm SM gtk-x11-2.0 gdk-x11-2.0 pangocairo-1.0 atk-1.0 cairo gdk_pixbuf-2.0 gio-2.0 pangoft2-1.0 pango-1.0 gobject-2.0 glib-2.0 harfbuzz fontconfig freetype wxscintilla-3.2 expat pcre2-32 png jpeg tiff jbig z m)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # wx-config --cxxflags --libs
    target_link_libraries(${PROJECT_NAME} pthread wx_osx_cocoau_xrc-3.2 wx_osx_cocoau_html-3.2 wx_osx_cocoau_qa-3.2 wx_osx_cocoau_core-3.2 wx_baseu_xml-3.2 wx_baseu_net-3.2 wx_baseu-3.2 SDL2::SDL2)
else()
    message(STATUS "Unsupported OS ${CMAKE_SYSTEM_NAME}")
endif()
