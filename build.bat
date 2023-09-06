@echo off
set deps=deps/src/glad/glad.c
set shader=src/shader.cpp
set resource_manager=src/resource_manager.cpp
set renderer=src/renderer.cpp
set text_renderer=src/text_renderer.cpp
set game_object=src/game_object.cpp
set ball=src/ball.cpp
set files=src/main.cpp %deps% %shader% %resource_manager% %renderer% %text_renderer% %game_object% %ball%

g++ -std=c++20 %files% -I deps/include -L deps/libs -lglfw3 -lopengl32 -lgdi32 -lfreetype -mwindows -o pong.exe && pong.exe