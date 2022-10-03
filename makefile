# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/22 16:50:02 by tpereira          #+#    #+#              #
#    Updated: 2022/10/03 22:34:10 by tpereira         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MINISHELL   = minishell
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g -lreadline -L /opt/homebrew/Cellar/readline/8.1.2/lib/ -I /opt/homebrew/Cellar/readline/8.1.2/include
SRCS        = ./srcs/*.c
LIB         = libft/libft.a

all :
	@make -s -C libft
	@$(CC) $(CFLAGS) $(SRCS) $(LIB) -o $(MINISHELL) -lreadline -L /opt/homebrew/Cellar/readline/8.1.2/lib/ -I /opt/homebrew/Cellar/readline/8.1.2/include
	@printf "\e[38;5;46m ╔════════════════════════════════════╗\e[0m\n"
	@printf "\e[38;5;46m█▓▒░         Compiled Libft         ░▒▓█ \e[0m\n"
	@printf "\e[38;5;46m█▓▒░   Minishell build successful   ░▒▓█ \e[0m\n"
	@printf "\e[38;5;46m ╚════════════════════════════════════╝\e[0m\n"

clean :
	@make -s clean -C libft
	@rm -rf ./srcs/*.o
	@printf "\e[31;5;200m ╔════════════════════════════════════╗\e[0m\n"
	@printf "\e[31;5;200m░▒▓█         Clean complete        █▓▒░\e[0m\n"
	@printf "\e[31;5;200m ╚════════════════════════════════════╝\e[0m\n"

fclean : clean
	@make -s fclean -C libft
	@rm -rf $(MINISHELL)*

re : fclean all
