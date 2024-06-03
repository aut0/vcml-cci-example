cmake_minimum_required(VERSION 3.27)
project(vcml-cci-example)

add_subdirectory(deps/systemc)
add_subdirectory(deps/cci)
add_subdirectory(deps/vcml)
add_subdirectory(deps/vcml-cci)

add_executable(vp main.cpp)
target_link_libraries(vp SystemC::cci)
target_link_libraries(vp SystemC::systemc)
target_link_libraries(vp vcml)
target_link_libraries(vp vcml-cci)