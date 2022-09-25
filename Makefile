.PHONY:		all re bonus clean fclean

NAME		= philo
NAME_BONUS	= philo_bonus

DIR_SRCS	= srcs/
DIR_OBJS	= objs/
DIR_INC		= includes/

SRCS		= main.c \
		  philo_utils.c \
		  philo_thread_setup.c \
		  philo_routine.c \
		  philo_actions.c \
		  libphilo.c

SRCS_BONUS	= bonus_main.c \
		  philo_utils.c \
		  bonus_philo_init_threads.c \
		  bonus_philo_actions.c \
		  bonus_philo_routine.c \
		  libphilo.c

OBJS		= $(patsubst %.c, $(DIR_OBJS)%.o, $(SRCS))
OBJS_BONUS	= $(patsubst %.c, $(DIR_OBJS)%.o, $(SRCS_BONUS))
INC		= $(addprefix $(DIR_INC), philosophers.h)
INC_BONUS	= $(addprefix $(DIR_INC), philosophers_bonus.h)

GCC		= gcc
FLAGS   = -Wall -Werror -Wextra -fsanitize=thread  
RM		= rm -rf
ifeq (${shell uname}, Linux)
	LIB = -lpthread
endif

all:		$(NAME)

$(NAME):	$(OBJS) $(INC) Makefile
	@$(GCC) $(FLAGS) -o $(NAME) $(OBJS) $(LIB)
	@echo "$(G)[OK]$(ST)\t$(NAME) created successfully."

$(DIR_OBJS)%.o:	$(DIR_SRCS)%.c $(INC)
	@$(GCC) $(FLAGS) -c $< -I$(DIR_INC)
	@echo "$(G)[OK]$(ST)\t$< compiled successfully."
	@mkdir -p $(DIR_OBJS)
	@mv $(@F) $(DIR_OBJS)

clean:
	@$(RM) $(DIR_OBJS)
	@echo "$(R)[RM]$(ST)\t$(DIR_OBJS) removed."

fclean:		clean
	@$(RM) $(NAME)
	@echo "$(R)[RM]$(ST)\t$(NAME) removed."
ifneq (, $(wildcard ./$(NAME_BONUS)))
	@$(RM) $(NAME_BONUS)
	@echo "$(R)[RM]$(ST)\t$(NAME_BONUS) removed."
endif

re:		fclean all

bonus:		$(NAME_BONUS)

$(NAME_BONUS):	$(OBJS_BONUS)
	@$(GCC) -o $(NAME_BONUS) $(OBJS_BONUS) $(LIB)
	@echo "$(G)[OK]$(ST)\t$(NAME_BONUS) created successfully."

normi:
	@echo "\n$(G)NORMINETTE VERSION:\t\t`norminette -v`$(ST)"
	@norminette srcs/*.c includes/*.h


# --- err mgmt ---
$(INC):
	@echo "$(R)[KO]$(ST)\tFile $(@F) cannot be found."
$(DIR_SRCS)%.c:
	@echo "$(R)[KO]$(ST)\tFile $(@F) cannot be found."
# ----------------

# ---- graphic ----
LG	= \033[92m
G	= \033[32m
R	= \033[91m
ST	= \033[0m
#  ----------------
