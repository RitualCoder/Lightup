cmake_minimum_required(VERSION 3.0)

project(Font C)

foreach(file "LiberationSerif-Regular.ttf")
  file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)