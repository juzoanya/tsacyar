# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juzoanya <juzoanya@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/17 22:09:40 by euyi              #+#    #+#              #
#    Updated: 2022/10/31 23:17:19 by juzoanya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

SRC = 	app/main.c	app/check_valid_scene.c	app/run_cube.c	app/raycast.c	\
		assets/check_valid_scene_utils.c	assets/check_valid_scene_utils0.c	\
		assets/check_valid_scene_utils1.c	assets/check_valid_scene_utils2.c	\
		assets/libft_tools.c \

OBJ = $(subst .c,.o,$(SRC))

CFLAGS = -Wall	-Werror	-Wextra

all: $(NAME)

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) assets/gcamerli/libmlx.a -L /usr/include/X11/extensions -lX11 -lXext -lm

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY:	all	clean	fclean	re