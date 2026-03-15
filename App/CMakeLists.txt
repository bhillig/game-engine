project(App)

set(SOURCES 
    src/GameLayer.cpp
    src/Main.cpp
    src/GameSceneLayer.cpp
    src/GUILayer.cpp
    src/Scene/Scene.cpp)

set(HEADERS
    include/GameLayer.h
    include/GameSceneLayer.h
    include/GUILayer.h
	include/Scene/Scene.h)

add_executable(App ${SOURCES} ${HEADERS})

target_link_libraries(App PRIVATE Engine)

target_include_directories(App PRIVATE include)

# Sync Visual Studio file directories to what's on disk
set(ALL_FILES ${SOURCES} ${HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ALL_FILES})

# Define AUDIO_DIR
target_compile_definitions(App PRIVATE AUDIO_DIR="$<TARGET_FILE_DIR:App>/rc/audio")
# Define MODEL_DIR
target_compile_definitions(App PRIVATE MODEL_DIR="$<TARGET_FILE_DIR:App>/rc/models")
# Define SHADER_DIR
target_compile_definitions(App PRIVATE SHADER_DIR="$<TARGET_FILE_DIR:App>/rc/shaders")
# Define TEXTURE_DIR
target_compile_definitions(App PRIVATE TEXTURE_DIR="$<TARGET_FILE_DIR:App>/rc/textures")

# Copy resources to build folder
add_custom_command(TARGET App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/rc"
        "$<TARGET_FILE_DIR:App>/rc"
)
