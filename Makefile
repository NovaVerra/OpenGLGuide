# NAME OF EXECUTABLE
NAME = opengl

# COMPILER
CC = g++
CFLAGS = -Wall -Wextra -Werror -Wno-int-to-void-pointer-cast -g -std=c++17 -fsanitize=address -g

# DIRECTORIES FOR BASE PROJECT
SRC_DIR = ./srcs/
OBJ_DIR = ./objs/
INC_DIR = ./includes/

# GLFW
GLFW_DIR = $(shell brew --prefix glfw)
GLFW_INC = $(GLFW_DIR)/include/
GLFW_LINK = -L $(GLFW_DIR)/lib/ -lglfw -framework OpenGL

# GLEW
GLEW_DIR = $(shell brew --prefix glew)
GLEW_INC = $(GLFW_DIR)/include/
GLEW_LINK = -L $(GLFW_DIR)/lib/ -lGLEW

# HEADER (includes all INC)
HEADER = -I $(INC_DIR) -I $(GLFW_INC) -I $(GLEW_INC)

# SRCS & OBJS
SRCS = main.cpp \
		Renderer.cpp \
		VertexBuffer.cpp \
		IndexBuffer.cpp \
		VertexBufferLayout.cpp \
		VertexArray.cpp \
		Shader.cpp \
		Texture.cpp \
		stb_image.cpp

OBJS = $(addprefix $(OBJ_DIR),$(SRCS:.cpp=.o))

# INSTRUCTIONS
all: objs $(NAME)

objs:
	mkdir -p $(OBJ_DIR)

# .cpp to .o
$(OBJ_DIR)%.o:$(SRC_DIR)%.cpp
	$(CC) $(CFLAGS) $(HEADER) -o $@ -c $<

# .o to ./wireframe
$(NAME):$(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(GLFW_LINK) $(GLEW_LINK) $(OBJS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re
