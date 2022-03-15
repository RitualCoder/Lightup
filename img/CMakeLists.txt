cmake_minimum_required(VERSION 3.0)

project(Image C)

foreach(file "wall1.png" "wall2.png" "wall3.png" "wall4.png" "wallu.png" "lightbulb.png")
  file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)