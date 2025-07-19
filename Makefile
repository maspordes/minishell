# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/05 12:00:00 by user              #+#    #+#              #
#    Updated: 2025/07/18 16:14:15 by shutan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

# 包含libft库
LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

# 头文件路径
INCLUDES = -I./includes -I./libft/includes

# 源文件路径
SRCS_PATH = ./srcs

# 源文件
SRCS = $(SRCS_PATH)/main.c \
       $(SRCS_PATH)/main/main_utils.c \
       $(SRCS_PATH)/main/input_handler.c \
       $(SRCS_PATH)/main/terminal_utils.c \
       $(SRCS_PATH)/main/command_processor.c \
       $(SRCS_PATH)/lexer/lexer.c \
       $(SRCS_PATH)/lexer/lexer_utils.c \
       $(SRCS_PATH)/lexer/lexer_special.c \
       $(SRCS_PATH)/lexer/lexer_word.c \
       $(SRCS_PATH)/parser/parser.c \
       $(SRCS_PATH)/parser/parser_utils.c \
       $(SRCS_PATH)/parser/parser_redirect.c \
       $(SRCS_PATH)/parser/parser_args.c \
       $(SRCS_PATH)/expansion/expansion.c \
       $(SRCS_PATH)/expansion/variable_expansion.c \
       $(SRCS_PATH)/expansion/quote_removal.c \
       $(SRCS_PATH)/expansion/expansion_utils.c \
       $(SRCS_PATH)/expansion/expansion_helpers.c \
       $(SRCS_PATH)/expansion/expansion_vars.c \
       $(SRCS_PATH)/expansion/expansion_memory.c \
       $(SRCS_PATH)/executor/executor.c \
       $(SRCS_PATH)/executor/executor_utils.c \
       $(SRCS_PATH)/executor/heredoc.c \
       $(SRCS_PATH)/executor/filename_utils.c \
       $(SRCS_PATH)/executor/builtin_handler.c \
       $(SRCS_PATH)/executor/pipe_handler.c \
       $(SRCS_PATH)/executor/pipe_setup.c \
       $(SRCS_PATH)/executor/redirection_handler.c \
       $(SRCS_PATH)/executor/redirection_utils.c \
       $(SRCS_PATH)/executor/external_cmd.c \
       $(SRCS_PATH)/executor/process_handler.c \
       $(SRCS_PATH)/builtins/echo.c \
       $(SRCS_PATH)/builtins/cd.c \
       $(SRCS_PATH)/builtins/cd_utils.c \
       $(SRCS_PATH)/builtins/pwd.c \
       $(SRCS_PATH)/builtins/export.c \
       $(SRCS_PATH)/builtins/unset.c \
       $(SRCS_PATH)/builtins/env.c \
       $(SRCS_PATH)/builtins/exit.c \
       $(SRCS_PATH)/builtins/exit_utils.c \
       $(SRCS_PATH)/builtins/exit_validation.c \
       $(SRCS_PATH)/signals/signals.c \
       $(SRCS_PATH)/env/env.c \
       $(SRCS_PATH)/env/env_operations.c \
       $(SRCS_PATH)/env/env_utils.c \
       $(SRCS_PATH)/utils/error_utils.c \
       $(SRCS_PATH)/utils/string_utils.c

# 目标文件
OBJS = $(SRCS:.c=.o)

# readline库
READLINE = -lreadline

# 编译规则
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)
	find $(SRCS_PATH) -name "*.o" -delete
	make -C $(LIBFT_PATH) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re
