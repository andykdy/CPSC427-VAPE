cmake_minimum_required(VERSION 3.1)
project(vape)

# Get version
file(READ version.txt versionFile)
if (NOT versionFile)
    message(FATAL_ERROR "version.txt missing, unable to determine version!")
endif()
string(STRIP "${versionFile}" TEST_VERSION)
string(REPLACE "." ";" VERSION_LIST ${TEST_VERSION})
list(GET VERSION_LIST 0 VERSION_MAJOR)
list(GET VERSION_LIST 1 VERSION_MINOR)
list(GET VERSION_LIST 2 VERSION_PATCH)

message("version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Set c++11
# https://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()
set (CMAKE_CXX_STANDARD 11)

# nice hierarchichal structure in MSVC
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif()

#Find OS
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(IS_OS_MAC 1)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(IS_OS_LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(IS_OS_WINDOWS 1)
else()
  message(FATAL_ERROR "OS ${CMAKE_SYSTEM_NAME} was not recognized")
endif()

# Create executable target

# Generate the shader folder location to the header
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/project_path.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/project_path.hpp")


# Download large assets like cutscenes from a remote server.
include(${PROJECT_SOURCE_DIR}/cmake/Modules/assets-download.cmake)
download_assets()


# You can switch to use the file GLOB for simplicity but at your own risk
# file(GLOB SOURCE_FILES src/*.cpp src/*.hpp)

set(SOURCE_FILES
        src/main.cpp
        src/project_path.hpp
        src/common.cpp src/common.hpp

        src/Engine/GameEngine.cpp src/Engine/GameEngine.hpp
        src/Engine/GameState.hpp

        src/Engine/States/LevelState.cpp src/Engine/States/LevelState.hpp
        src/Engine/States/MainMenuState.cpp src/Engine/States/MainMenuState.hpp
        src/Engine/States/TutorialState.cpp src/Engine/States/TutorialState.hpp
        src/Engine/States/IntroState.cpp src/Engine/States/IntroState.hpp
        src/Engine/States/ControlsState.hpp
        src/Engine/States/BetweenLevelsState.cpp src/Engine/States/BetweenLevelsState.hpp
        src/Engine/States/OutroState.cpp src/Engine/States/OutroState.hpp


        src/Engine/ECS/Component.hpp
        src/Engine/ECS/Entity.hpp
        src/Engine/ECS/EntityManager.cpp src/Engine/ECS/EntityManager.hpp
        src/Engine/ECS/System.hpp
        src/Engine/ECS/SystemManager.cpp src/Engine/ECS/SystemManager.hpp
        src/Engine/ECS/ECS.hpp

        src/Engine/Graphics/VideoUtil.cpp src/Engine/Graphics/VideoUtil.hpp
        src/Engine/Graphics/Font.cpp src/Engine/Graphics/Font.hpp

        src/Entities/Effects/Explosion.cpp src/Entities/Effects/Explosion.hpp
        src/Entities/Effects/VampParticleEmitter.cpp src/Entities/Effects/VampParticleEmitter.hpp

        src/Entities/Player.cpp src/Entities/Player.hpp
        src/Entities/Space.cpp src/Entities/Space.hpp
        src/Entities/Vamp.cpp src/Entities/Vamp.hpp
        src/Entities/Intro.cpp src/Entities/Intro.hpp

        src/Entities/Debugging/DebugDot.cpp src/Entities/Debugging/DebugDot.hpp

        src/Entities/Enemies/Enemy.hpp
        src/Entities/Enemies/turtle.cpp src/Entities/Enemies/turtle.hpp
        src/Entities/Enemies/EnemyGenericShooter.cpp src/Entities/Enemies/EnemyGenericShooter.hpp
        src/Entities/Enemies/EnemyTargettedShooter.cpp src/Entities/Enemies/EnemyTargettedShooter.hpp

        src/Entities/UI/Text.cpp src/Entities/UI/Text.hpp
        src/Entities/UI/PlayerHealth/Health.cpp src/Entities/UI/PlayerHealth/Health.hpp
        src/Entities/UI/Dialogue/Dialogue.cpp src/Entities/UI/Dialogue/Dialogue.hpp
        src/Entities/UI/Dialogue/Continue.cpp src/Entities/UI/Dialogue/Continue.hpp
        src/Entities/UI/Vamp/VampCharge.cpp src/Entities/UI/Vamp/VampCharge.hpp
        src/Entities/UI/UIPanel/UIPanel.cpp src/Entities/UI/UIPanel/UIPanel.hpp
        src/Entities/UI/UIPanel/UIPanelBackground.cpp src/Entities/UI/UIPanel/UIPanelBackground.hpp
        src/Entities/UI/BossHealth/BossHealth.cpp src/Entities/UI/BossHealth/BossHealth.hpp
        src/Entities/UI/BossHealth/BossHealthBar.cpp src/Entities/UI/BossHealth/BossHealthBar.hpp
        src/Entities/UI/Button.hpp
        src/Entities/UI/Cursor.cpp src/Entities/UI/Cursor.hpp
        src/Entities/UI/MainMenu/MainMenu.cpp src/Entities/UI/MainMenu/MainMenu.hpp
        src/Entities/UI/MainMenu/ExitButton.cpp src/Entities/UI/MainMenu/ExitButton.hpp
        src/Entities/UI/MainMenu/StartButton.cpp src/Entities/UI/MainMenu/StartButton.hpp
        src/Entities/UI/MainMenu/ContinueButton.cpp src/Entities/UI/MainMenu/ContinueButton.hpp
        src/Entities/UI/MainMenu/TutorialButton.cpp src/Entities/UI/MainMenu/TutorialButton.hpp
        src/Entities/UI/EnterSkip.cpp src/Entities/UI/EnterSkip.hpp
        src/Entities/UI/EnterContinue.cpp src/Entities/UI/EnterContinue.hpp
        src/Entities/UI/PauseMenu/PauseMenu.cpp src/Entities/UI/PauseMenu/PauseMenu.hpp
        src/Entities/UI/PauseMenu/ResumeButton.cpp src/Entities/UI/PauseMenu/ResumeButton.hpp
        src/Entities/UI/PauseMenu/ExitToMenuButton.cpp src/Entities/UI/PauseMenu/ExitToMenuButton.hpp
        src/Entities/UI/PauseMenu/ControlDiagram.cpp src/Entities/UI/PauseMenu/ControlDiagram.hpp


        "src/Entities/Projectiles and Damaging/Projectile.hpp"
        "src/Entities/Projectiles and Damaging/bullet.cpp" "src/Entities/Projectiles and Damaging/bullet.hpp"
        "src/Entities/Projectiles and Damaging/Laser/Laser.cpp" "src/Entities/Projectiles and Damaging/Laser/Laser.hpp"
        "src/Entities/Projectiles and Damaging/Laser/LaserBeamSprite.cpp" "src/Entities/Projectiles and Damaging/Laser/LaserBeamSprite.hpp"

        src/Entities/Bosses/Boss.hpp
        src/Entities/Bosses/Boss1.cpp src/Entities/Bosses/Boss1.hpp
        src/Entities/Bosses/Boss2.cpp src/Entities/Bosses/Boss2.hpp

        src/Entities/Pickups/Pickup.hpp
        src/Entities/Pickups/TestPickup.cpp src/Entities/Pickups/TestPickup.hpp


        src/Entities/Weapons/Weapon.hpp
        src/Entities/Weapons/BulletStraightShot.cpp src/Entities/Weapons/BulletStraightShot.hpp


        src/Components/MotionComponent.hpp
        src/Components/PhysicsComponent.hpp
        src/Components/EffectComponent.hpp
        src/Components/SpriteComponent.hpp
        src/Components/TransformComponent.hpp
        src/Components/MeshComponent.hpp
        src/Components/TextureComponent.hpp
        src/Components/HealthComponent.hpp
        src/Components/BoundaryComponent.hpp
        src/Components/EnemyComponent.hpp
        src/Components/CollisionComponent.hpp


        src/Systems/EnemySpawnerSystem.cpp src/Systems/EnemySpawnerSystem.hpp
        src/Systems/MotionSystem.cpp src/Systems/MotionSystem.hpp
        src/Systems/CollisionSystem.cpp src/Systems/CollisionSystem.hpp
        src/Systems/ProjectileSystem.cpp src/Systems/ProjectileSystem.hpp

        src/Levels/Level.hpp
        src/Levels/Waves.hpp

        src/Levels/Levels.hpp
        src/Levels/Level1.hpp
        src/Levels/Level2.hpp
        src/Levels/Level2.hpp

        src/Utils/SaveData.cpp src/Utils/SaveData.hpp

        src/Entities/Video.cpp src/Entities/Video.hpp

        src/Components/PlayerComponent.hpp src/Entities/Bosses/Boss3.cpp src/Entities/Bosses/Boss3.hpp src/Levels/Level3.hpp src/Entities/Enemies/EnemyExplosivePayload.cpp src/Entities/Enemies/EnemyExplosivePayload.hpp src/Entities/Enemies/EnemySpeedster.cpp src/Entities/Enemies/EnemySpeedster.hpp src/Entities/Weapons/WeaponTriShot.cpp src/Entities/Weapons/WeaponTriShot.hpp)

if (IS_OS_MAC)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC src/)

# Added this so policy CMP0065 doesn't scream
set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 0)
set_target_properties(${name} PROPERTIES
        VERSION   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
        SOVERSION ${VERSION_MAJOR}
        )


# External header-only libraries in the ext/
target_include_directories(${PROJECT_NAME} PUBLIC ext/stb_truetype/)
target_include_directories(${PROJECT_NAME} PUBLIC ext/stb_image/)
target_include_directories(${PROJECT_NAME} PUBLIC ext/gl3w)

# Find OpenGL
find_package(OpenGL REQUIRED)

if (OPENGL_FOUND)
   target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
   target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_gl_LIBRARY})
endif()

# glfw, sdl could be precompiled (on windows) or installed by a package manager (on OSX and Linux)

if (IS_OS_LINUX OR IS_OS_MAC)
    # Try to find packages rather than to use the precompiled ones
    # Since we're on OSX or Linux, we can just use pkgconfig.
    find_package(PkgConfig REQUIRED)

    pkg_search_module(GLFW REQURIED glfw3)

    pkg_search_module(SDL2 REQURIED sdl2)
    pkg_search_module(SDL2MIXER REQURIED SDL2_mixer)

    # Link Frameworks on OSX
    if (IS_OS_MAC)
       find_library(COCOA_LIBRARY Cocoa)
       find_library(CF_LIBRARY CoreFoundation)
       target_link_libraries(${PROJECT_NAME} PUBLIC ${COCOA_LIBRARY} ${CF_LIBRARY})
    endif()
elseif (IS_OS_WINDOWS)
# https://stackoverflow.com/questions/17126860/cmake-link-precompiled-library-depending-on-os-and-architecture
    set(GLFW_FOUND TRUE)
    set(SDL2_FOUND TRUE)

    set(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/include")
    set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/ext/sdl/include/SDL")

    if (${CMAKE_SIZEOF_VOID_P} MATCHES "8")
        set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/lib/glfw3dll-x64.lib")
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/ext/sdl/lib/SDL2-x64.lib")
        set(SDL2MIXER_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/ext/sdl/lib/SDL2_mixer-x64.lib")

        set(GLFW_DLL "${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/lib/glfw3-x64.dll")
        set(SDL_DLL "${CMAKE_CURRENT_SOURCE_DIR}/ext/sdl/lib/SDL2-x64.dll")
        set(SDLMIXER_DLL "${CMAKE_CURRENT_SOURCE_DIR}/ext/sdl/lib/SDL2_mixer-x64.dll")
    else()
        set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/lib/glfw3dll-x86.lib")
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/ext/sdl/lib/SDL2-x86.lib")
        set(SDL2MIXER_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/ext/sdl/lib/SDL2_mixer-x86.lib")

        set(GLFW_DLL "${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/lib/glfw3-x86.dll")
        set(SDL_DLL "${CMAKE_CURRENT_SOURCE_DIR}/ext/sdl/lib/SDL2-x86.dll")
        set(SDLMIXER_DLL "${CMAKE_CURRENT_SOURCE_DIR}/ext/sdl/lib/SDL2_mixer-x86.dll")
    endif()

    # Copy and rename dlls
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLFW_DLL}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/glfw3.dll")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL_DLL}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2.dll")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDLMIXER_DLL}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_mixer.dll")
endif()

# Can't find the include and lib. Quit.
if (NOT GLFW_FOUND OR NOT SDL2_FOUND)
   if (NOT GLFW_FOUND)
      message(FATAL_ERROR "Can't find GLFW." )
   else ()
      message(FATAL_ERROR "Can't find SDL." )
   endif()
endif()

add_subdirectory(ext/ffmpeg)
if (IS_OS_WINDOWS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIBAVCODEC_DLL}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/avcodec-58.dll")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIBAVFORMAT_DLL}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/avformat-58.dll")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIBAVFILTER_DLL}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/avfilter-7.dll")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIBAVDEVICE_DLL}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/avdevice-58.dll")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIBAVUTIL_DLL}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/avutil-56.dll")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIBSWRESAMPLE_DLL}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/swresample-3.dll")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIBSWSCALE_DLL}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/swscale-5.dll")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${FFMPEG_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${FFMPEG_LIBRARIES})

# Needed to add this
if(IS_OS_LINUX)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(OUTPUT_DIR ./releases)
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
    set(OUTPUT_ZIP ${OUTPUT_DIR}/Vape.zip)
    if (IS_OS_WINDOWS)
        set(OUTPUT_ZIP ${OUTPUT_DIR}/Vape-Windows.zip)
    elseif (IS_OS_MAC)
        set(OUTPUT_ZIP ${OUTPUT_DIR}/Vape-Mac.zip)
    elseif (IS_OS_WINDOWS)
        set(OUTPUT_ZIP ${OUTPUT_DIR}/Vape-Linux.zip)
    endif()

    set(ZIP_FILES
            bin
            data
            shaders
            README.md
            version.txt)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E tar cf ${OUTPUT_ZIP} --format=zip -- ${ZIP_FILES}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Packaging Release ${OUTPUT_ZIP} this may take a minute...")
endif()