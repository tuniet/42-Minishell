NAME		=		minishell

INC_DIR		=		./include
HEADER_FILE	=		$(INC_DIR)/minishell.h

SRC_DIR		=		./src
SRCS		=		$(addprefix $(SRC_DIR)/, \
					init.c prompt.c string.c tokenizer.c \
					main.c signals.c aux.c \
					execute.c execute_utils.c \
					tree.c tree_utils.c free_tree.c \
					free.c mem.c \
					expand.c expand_utils.c \
					ft_split.c \
					builtins.c )

OBJ_DIR		=		./obj
OBJS		=		$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS		=		-Wall -Wextra -Werror
#CFLAGS		=		-g -O0 -Werror
CFLAGS		=		-g -fsanitize=address -Werror
#LDFLAGS		=		-lreadline #-lhistory
LDFLAGS		=	-L/opt/homebrew/opt/readline/lib -lreadline -lhistory
CC			=		cc

all:	${NAME}

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) && \
	(printf "[minishell] Compiled successfully.\n")

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(HEADER_FILE)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR) -I /opt/homebrew/opt/readline/include/

clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)
	@printf "[minishell] Object files cleaned.\n"

fclean: clean
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	@printf "[minishell] Cleaned successfully.\n"

re:	fclean all

.PHONY:	all clean fclean re
