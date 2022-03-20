cmake_minimum_required(VERSION 3.0)

project(Image C)

foreach(file "LiberationSerif-Regular.ttf")
  file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)