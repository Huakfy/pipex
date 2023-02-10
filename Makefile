SRCS	=	main.c pipex_utils.c\
			ft_printf/ft_printchar_fd.c ft_printf/ft_printnbr_fd.c \
			ft_printf/ft_uibasefd.c ft_printf/ft_printf.c \
			ft_printf/ft_printstr_fd.c ft_printf/ft_printmemory_fd.c \
			ft_printf/ft_printunbr_fd.c \
			libft/ft_split.c libft/ft_strdup.c libft/ft_strjoin.c

OBJS	=	${SRCS:.c=.o}

CC		=	gcc

CFLAGS	=	-g3 -Wall -Werror -Wextra

RM	=	rm -f

NAME	=	pipex

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -o ${NAME} ${OBJS}

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

norm:
	norminette ${SRCS} ft_printf/libftprintf.h libft/libft.h pipex.h

.PHONY: all bonus clean fclean re norm
