.PHONY:		all re bonus clean fclean

NAME		= philo

DIR_SRCS	= srcs/
DIR_OBJS	= objs/
DIR_INC		= includes/

SRCS		= philo.c
OBJS		= $(patsubst %.c, $(DIR_OBJS)%.o, $(SRCS))
INC		= $(addprefix $(DIR_INC), philo.h)

GCC		= gcc -Wall -Werror -Wextra
RM		= rm -rf

# ---- graphic ----
LG	= \033[92m
G	= \033[32m
R	= \033[91m
ST	= \033[0m
#  ----------------

all:		$(NAME)

$(NAME):	$(OBJS)
	@$(GCC) -o $(NAME) $(OBJS)
	@echo "$(G)[OK]$(ST)\t$(NAME) created successfully."

$(DIR_OBJS)%.o:	$(DIR_SRCS)%.c $(INC)
	@$(GCC) -c $< -I$(DIR_INC)
	@echo "$(G)[OK]$(ST)\t$< compiled successfully."
	@mkdir -f $(DIR_OBJS)
	@mv $(@F) $(DIR_OBJS)

clean:
	@$(RM) $(DIR_OBJS)
	@echo "$(R)[RM]$(ST)\t$(DIR_OBJS) removed."

fclean:		clean
	@$(RM) $(NAME)
	@echo "$(R)[RM]$(ST)\t$(NAME) removed."

re:		fclean all

# --- err mgmt ---
$(INC):
	@echo "$(R)[KO]$(ST)\tFile $(@F) cannot be found."
$(DIR_SRCS)%.c:
	@echo "$(R)[KO]$(ST)\tFile $(@F) cannot be found."
# ----------------

