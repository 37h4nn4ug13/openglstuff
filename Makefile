main: test.cpp glad.c
	g++ test.cpp glad.c -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -Wall -Wextra -Wpedantic -Werror
