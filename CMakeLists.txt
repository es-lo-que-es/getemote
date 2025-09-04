cmake_minimum_required(VERSION 3.14)

project(getemote)

include_directories(src/)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set (
   req_src
   src/reqs/channel_lookup.c
   src/reqs/request_handler.c
   src/reqs/auth_handler.c
   src/reqs/twitch_requests.c
   src/reqs/request.c
)


set (
   emote_src
   src/emotes/emote_list.c
   src/emotes/emote_info.c
   src/emotes/emote.c
)


set (
   interface_src
   src/interface/input_line.c
   src/interface/search_bar.c
)


set (
   app_src

   ${req_src}
   ${emote_src}
   ${interface_src}

   src/channel_view.c
   src/resources.c
   src/config.c
   src/main.c
   src/util.c

   src/app.c
)

file(COPY tokens DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
make_directory(${CMAKE_CURRENT_BINARY_DIR}/cache)

add_executable(run ${app_src})
target_link_libraries(run curl raylib jansson m)
