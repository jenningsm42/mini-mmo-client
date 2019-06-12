cmake_minimum_required(VERSION 3.1)

project(MiniMMOClient
        VERSION 1.0
        DESCRIPTION "Mini-'MMO' Client"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        src/AnimatedSprite.cpp
        src/AssetCache.cpp
        src/Character.cpp
        src/CharacterCreationScene.cpp
        src/CharacterSelectionScene.cpp
        src/Chatbox.cpp
        src/Game.cpp
        src/GameScene.cpp
        src/InputHandler.cpp
        src/LoginScene.cpp
        src/main.cpp
        src/Message.cpp
        src/OtherPlayer.cpp
        src/Player.cpp
        src/PlayerPool.cpp
        src/Scene.cpp
        src/SceneHandler.cpp
        src/Socket.cpp
        src/ColorPickerWidget.cpp)

set(PROTO_FILES
        proto/Characters.proto
        proto/Chat.proto
        proto/Login.proto
        proto/PlayerJoin.proto
        proto/PlayerMove.proto
        proto/Register.proto
        proto/Version.proto)

set(SPINE_DIR spine)
file(GLOB SPINE_SOURCE_FILES "${SPINE_DIR}/spine/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
find_package(TGUI 0.8.3 REQUIRED)
find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

include_directories(${SPINE_DIR})

add_executable(client ${SOURCE_FILES} ${PROTO_SRCS} ${SPINE_SOURCE_FILES})

if (MSVC)
    target_compile_options(client PRIVATE /W4 /WX)
else (MSVC)
    target_compile_options(client PRIVATE -Wall -Wextra -pedantic -Werror)
endif (MSVC)

target_link_libraries(client
        tgui
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        ${Protobuf_LIBRARIES})
