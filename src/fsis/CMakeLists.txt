include(FetchContent)

# cpr
FetchContent_Declare(
	cpr
	GIT_REPOSITORY https://github.com/libcpr/cpr.git
	GIT_TAG 1.10.0
)
FetchContent_MakeAvailable(cpr)

set(FSIS_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Achievement.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ClientApplication.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/FSISCharacter.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/FSISGameMode.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Monster.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/NetworkClient.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Projectile.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Sorcerer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/states/AuthState.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/states/GameState.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/states/MenuState.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/states/TotalState.cpp"
	)

add_executable(fsis-game ${FSIS_SOURCES})

target_link_libraries(fsis-game
    PRIVATE
        fsis::engine
		cpr::cpr
)

target_include_directories(fsis-game
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)