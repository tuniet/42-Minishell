NAME		=		minishell

INC_DIR		=		./include
HEADER_FILE	=		$(INC_DIR)/minishell.h
LIBFT		=		libft
SRC_DIR		=		./src
SRCS		=		$(addprefix $(SRC_DIR)/, \
					main.c \
					init.c prompt.c \
					tokenizer.c tokenizer_utils.c \
					expand.c expand_wildcard.c expand_utils.c argv_funcs.c \
					execute.c execute_utils.c execute_tree.c\
					tree.c tree_utils.c tree_utils2.c free_tree.c \
					free.c mem.c \
					builtins.c builtins_utils.c unset.c export.c envp.c\
					heredoc.c signals.c prompt_utils.c \
					validate.c error.c )

OBJ_DIR		=		./obj
OBJS		=		$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#CFLAGS		=		-Wall -Wextra -Werror
#CFLAGS		=		-g -O0 -Werror
CFLAGS		=		-g -fsanitize=address -Wall -Wextra -Werror
LDFLAGS		=		-lreadline -lhistory
#LDFLAGS		=	-L/opt/homebrew/opt/readline/lib -lreadline #-lhistory
CC			=		gcc

all:	${NAME}

$(NAME): $(OBJS)
	@make -C $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT) -lft -o $(NAME) $(LDFLAGS) && \
	(printf "minishell compiled successfully.\n")

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(HEADER_FILE)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)

clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)
	@printf "[minishell] Object files cleaned.\n"

fclean: clean
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	@make -s -C $(LIBFT) fclean
	@printf "[minishell] Cleaned successfully.\n"

re:	fclean all

.PHONY:	all clean fclean re
