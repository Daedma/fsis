cmake_minimum_required(VERSION 4.1)
project(fsis-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

include(FetchContent)

# SFML
FetchContent_Declare(
	SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG        2.6.2
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL
	SYSTEM
	FIND_PACKAGE_ARGS NAMES SFML VERSION 2.6.2
)
FetchContent_MakeAvailable(SFML)
	
# TGUI
FetchContent_Declare(
	TGUI
	GIT_REPOSITORY https://github.com/texus/TGUI.git
	GIT_TAG        v0.9.5
	FIND_PACKAGE_ARGS NAMES TGUI VERSION 0.9.5
)
FetchContent_MakeAvailable(TGUI)
		
# EASTL
FetchContent_Declare(
	EASTL
	GIT_REPOSITORY https://github.com/electronicarts/EASTL.git
	GIT_TAG        4.0.2
	FIND_PACKAGE_ARGS NAMES EASTL
)
FetchContent_MakeAvailable(EASTL)
		
# Boost
FetchContent_Declare(
	Boost
	URL https://github.com/boostorg/boost/releases/download/boost-1.81.0/boost-1.81.0.tar.xz
	DOWNLOAD_EXTRACT_TIMESTAMP ON
	FIND_PACKAGE_ARGS NAMES Boost VERSION 1.81.0 COMPONENTS json container
)
set(BOOST_INCLUDE_LIBRARIES json container)
FetchContent_MakeAvailable(Boost)

set(ACTORS_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/actors/Actor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/actors/Character.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/actors/Map.cpp"
)
	
set(ANIMATION_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/animation/Animation.cpp"
)
	
set(COMPONENTS_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/components/ActorComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/AudioComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/BoxComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/CharacterAnimComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/CollisionComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/MovementComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/PrimitiveComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/RampComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/SceneComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/SphereComponent.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/components/SpriteComponent.cpp"
)

set(CONTROLLERS_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/controllers/AIController.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/controllers/Controller.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/controllers/PlayerController.cpp"
)

set(CORE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/core/AssetManager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/CollisionResolver.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/GameMode.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/GameObject.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/InputEventListener.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/JSON.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/World.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/New.cpp"
)

set(GUI_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/gui/HUD.cpp"
)

set(RENDER_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/render/Camera.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/render/Canvas.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/render/FollowCamera.cpp"
)

set(ENGINE_SOURCES
	${ACTORS_SOURCES}
	${ANIMATION_SOURCES}
	${COMPONENTS_SOURCES}
	${CONTROLLERS_SOURCES}
	${CORE_SOURCES}
	${GUI_SOURCES}
	${RENDER_SOURCES}
)

add_library(fsis-engine
    ${ENGINE_SOURCES}
)

target_include_directories(fsis-engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(fsis-engine
    PUBLIC
        EASTL
        Boost::json
        sfml-graphics
        sfml-window
        sfml-system
        tgui
)

add_library(fsis::engine ALIAS fsis-engine)