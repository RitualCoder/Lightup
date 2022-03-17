#Version Cmake
cmake_minimum_required(VERSION 3.0)

#projet
project("Lightup" C)

include(CTest)
enable_testing()

#on cherche SDL2
find_package(SDL2 REQUIRED)

set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS})
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} -lSDL2_ttf -lSDL2_image) # add extra SDL2 libraries

message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

include_directories(${SDL2_ALL_INC})

#variable
set(CMAKE_C_FLAGS "-std=c99 -Wall -pthread -Wextra -Wunused-parameter -L SDL2")
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
