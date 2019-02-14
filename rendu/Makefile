##
## Makefile for Bomberman in /home/infitek/Documents/Etna/Bomberman
## 
## Made by REBOUL Simon
## Login   <reboul_s@etna-alternance.net>
## 
## Started on  Fri May  4 16:31:38 2018 REBOUL Simon
## Last update Thu Feb 14 14:24:16 2019 REBOUL Simon
##

CC	:= gcc -g

RM	:= rm -rf

SRCS	:= src/main.c\
	src/init_game.c\
	src/network.c\
	src/cli_game.c\
	src/gere_keyboard.c\
	src/gere_ia.c\
	src/gere_gameplay.c\
	src/gere_display.c\
	src/gere_intro.c\
	src/load_map.c\
	src/linked_list.c\
	src/my_put_nbr.c\
	src/my_string.c\

OBJS	:= $(SRCS:.c=.o)

CFLAGS	:= -Werror -Wall -g

LDFLAGS	:= -lpthread -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

NAME	:= Bomberman

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all re clean fclean
