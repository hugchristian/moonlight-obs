cmake_minimum_required(VERSION 3.16...3.26)

project(moonlight-obs VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Find OBS Studio library
find_package(libobs REQUIRED)

# Find FFmpeg for video decoding
find_package(FFmpeg REQUIRED COMPONENTS avcodec avformat avutil swscale)

# Plugin source files
set(moonlight-obs_SOURCES
    src/plugin-main.c
    src/moonlight-source.c
    src/moonlight-client.c
    src/video-decoder.c
    src/audio-decoder.c
)

set(moonlight-obs_HEADERS
    src/plugin-main.h
    src/moonlight-source.h
    src/moonlight-client.h
    src/video-decoder.h
    src/audio-decoder.h
)

# Create the plugin library
add_library(moonlight-obs MODULE
    ${moonlight-obs_SOURCES}
    ${moonlight-obs_HEADERS}
)

# Include directories
target_include_directories(moonlight-obs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${FFMPEG_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(moonlight-obs
    OBS::libobs
    ${FFMPEG_LIBRARIES}
)

# Set up proper plugin structure
set_target_properties(moonlight-obs PROPERTIES
    PREFIX ""
    FOLDER "plugins"
)

# Installation
if(OS_WINDOWS)
    set(PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/obs-plugins/64bit")
elseif(OS_MACOS)
    set(PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/obs-plugins")
else()
    set(PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/obs-plugins")
endif()

install(TARGETS moonlight-obs
    LIBRARY DESTINATION "${PLUGIN_OUTPUT_DIR}"
    RUNTIME DESTINATION "${PLUGIN_OUTPUT_DIR}"
)

# Install data files
install(DIRECTORY data/
    DESTINATION "${CMAKE_INSTALL_DATADIR}/obs/obs-plugins/moonlight-obs"
)
