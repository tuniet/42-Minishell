NAME		=		minishell

INC_DIR		=		./include
HEADER_FILE	=		$(INC_DIR)/minishell.h

SRC_DIR		=		./src
SRCS		=		$(addprefix $(SRC_DIR)/, \
					init.c prompt.c string.c tokenizer.c \
					main.c signals.c execute.c )

OBJ_DIR		=		./obj
OBJS		=		$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS		=		-Wall -Wextra -Werror
CFLAGS		=		-Werror
LDFLAGS		=		-lreadline #-lhistory
CC			=		cc

all:	${NAME}

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) && \
	(printf "[minishell] Compiled successfully.\n")

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
	@printf "[minishell] Cleaned successfully.\n"

re:	fclean all

.PHONY:	all clean fclean re
