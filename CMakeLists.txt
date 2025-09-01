cmake_minimum_required(VERSION 3.10)

project(getemote)


set (
   me_src
   src/request_handler.c
   src/request.c
   src/main.c
)


add_executable(run ${me_src})
target_link_libraries(run curl raylib)
