cmake_minimum_required(VERSION 3.0)

project(Levels C)

foreach(file "level1.txt" "level2.txt" "level3.txt" "level4.txt" "level5.txt" "level6.txt" "level7.txt" "level8.txt" "level9.txt" )
  file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)