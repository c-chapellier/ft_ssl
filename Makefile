SRCS =	main.c \
		algorithm_cli.c \
		md5.c \
		sha256.c \
		utils/print_uint.c \
		utils/read_file.c \
		utils/binary_operations.c

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