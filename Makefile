SRCS =	main.c \
		md5.c \
		sha256.c \
		print_binary.c

OBJS = ${SRCS:.c=.o}

NAME = ft_ssl

CFLAGS		= -Wall -Wextra -Werror

all :		${NAME}

${NAME} :	${OBJS}
			gcc -o ${NAME} ${OBJS}

clean :
			rm -f ${OBJS}

fclean :	clean
			rm -f ${NAME}

re :		fclean all