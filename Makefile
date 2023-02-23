SRCS	=	ft_printf/ft_printchar_fd.c ft_printf/ft_printnbr_fd.c \
			ft_printf/ft_uibasefd.c ft_printf/ft_printf.c \
			ft_printf/ft_printstr_fd.c ft_printf/ft_printmemory_fd.c \
			ft_printf/ft_printunbr_fd.c \
			libft/ft_split.c libft/ft_strdup.c libft/ft_strjoin.c

MANDATORY	=	main.c pipex_utils.c path.c libft/ft_strchr.c

BONUS		=	bonus/free.c bonus/here_doc.c bonus/main_bonus.c bonus/paths.c\
				bonus/pipex_utils_bonus.c\
				gnl/get_next_line.c gnl/get_next_line_utils.c \
				libft/ft_strncmp.c

OBJS	=	${SRCS:.c=.o}

OBJS_MANDATORY	=	${MANDATORY:.c=.o}

OBJS_BONUS		=	${BONUS:.c=.o}

CC		=	gcc

CFLAGS	=	-g3 -Wall -Werror -Wextra

RM	=	rm -f

NAME	=	pipex

NAME_B	=	pipex_bonus

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS} ${OBJS_MANDATORY}
	${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${OBJS_MANDATORY}

${NAME_B}: ${OBJS} ${OBJS_BONUS}
	${CC} ${CFLAGS} -o ${NAME_B} ${OBJS} ${OBJS_BONUS}

all: ${NAME}

bonus: ${NAME_B}

clean:
	${RM} ${OBJS} ${OBJS_MANDATORY} ${OBJS_BONUS}

fclean: clean
	${RM} ${NAME} ${NAME_B}

re: fclean all

norm:
	norminette ${SRCS} ${MANDATORY} ${BONUS} ft_printf/libftprintf.h \
	libft/libft.h pipex.h bonus/pipex_bonus.h

.PHONY: all bonus clean fclean re norm
