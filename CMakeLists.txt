cmake_minimum_required(VERSION 3.10)

project(getemote)


set (
   me_src
   src/request_handler.c
   src/auth_handler.c

   src/twitch_requests.c
   src/emote_info.c

   src/request.c
   src/main.c
)

file(COPY tokens DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(run ${me_src})
target_link_libraries(run curl raylib jansson)
