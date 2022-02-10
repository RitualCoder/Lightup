#Version Cmake
cmake_minimum_required(VERSION 3.0)

#projet
project("Lightup" C)

include(CTest)
enable_testing()

#variable
set(CMAKE_C_FLAGS "-std=c99 -Wall -pthread -Wextra -Wunused-parameter")
set(CMAKE_C_FLAGS_DEBUG "-g --coverage")    # use CMake option: -DCMAKE_BUILD_TYPE=DEBUG
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")   # use CMake option: -DCMAKE_BUILD_TYPE=RELEASE


set(GameSources libgame/game.c libgame/game_ext.c libgame/game_aux.c libgame/game_private.c libgame/game_tools.c)
set(GameHeaders libgame/game.h libgame/game_ext.h libgame/game_aux.h libgame/game_private.h libgame/game_tools.c)

set(StackSources libstack/queue.c)
set(StackHeaders libstack/queue.h)

add_library(game ${GameSources} ${StackSources} ${GameHeaders} ${StackHeaders})

#création de l'éxécutable
add_executable(game_text game_text.c)
add_executable(game_load_test game_test.c)

#on lie les librairie
target_link_libraries(game_text game)
target_link_libraries(game_load_test game)

#on ajoute les tests
add_subdirectory(test)

#On copie les jeux de test
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game_save/default.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game_save/default_solution.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})