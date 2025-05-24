#Version Cmake
cmake_minimum_required(VERSION 3.10)

#projet
project("Lightup" C)

include(CTest)
enable_testing()

#on cherche SDL2
find_package(PkgConfig REQUIRED)

# on cherche SDL2, SDL2_image et SDL2_ttf via pkg-config
pkg_check_modules(SDL2     REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_IMG REQUIRED SDL2_image)

# includes communs
include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
  ${SDL2_IMG_INCLUDE_DIRS}
)

# chemins de linkage
link_directories(
  ${SDL2_LIBRARY_DIRS}
  ${SDL2_TTF_LIBRARY_DIRS}
  ${SDL2_IMG_LIBRARY_DIRS}
)

set(SDL2_ALL_LIBS
  ${SDL2_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${SDL2_IMG_LIBRARIES}
)

message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

include_directories(${SDL2_ALL_INC})

#variable
set(CMAKE_C_FLAGS "-std=c99 -Wall -pthread -Wextra -Wunused-parameter")
set(CMAKE_C_FLAGS_DEBUG "-g --coverage -D DEBUG")    # use CMake option: -DCMAKE_BUILD_TYPE=DEBUG
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")   # use CMake option: -DCMAKE_BUILD_TYPE=RELEASE


set(GameSources libgame/game.c libgame/game_ext.c libgame/game_aux.c libgame/game_private.c libgame/game_tools.c)
set(GameHeaders libgame/game.h libgame/game_ext.h libgame/game_aux.h libgame/game_private.h libgame/game_tools.c)

set(StackSources libstack/queue.c)
set(StackHeaders libstack/queue.h)

add_library(game ${GameSources} ${StackSources} ${GameHeaders} ${StackHeaders})

#on ajoute la lib SDLUtil
add_subdirectory(libSDLUtils)

#création de l'éxécutable
add_executable(game_text game_text.c)
add_executable(game_solve game_solve.c)
add_executable(game_sdl game_sdl.c)

#on lie les librairie
target_link_libraries(game_text game)
target_link_libraries(game_solve game)
target_link_libraries(game_sdl SDLUtils game ${SDL2_ALL_LIBS})

#on ajoute les tests
add_subdirectory(test)

#on copire les images
add_subdirectory(img)

#On copie les jeux de test dans le dossier der test
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game_save/default.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game_save/default_solution.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game_save/3x3.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game_save/mark_test.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test)
