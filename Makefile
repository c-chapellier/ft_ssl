SRCS =	main.c \
		standards/genrsa.c \
		standards/rsa.c \
		standards/rsautl.c \
		hashes/md5.c \
		hashes/sha256.c \
		ciphers/base64.c \
		ciphers/des.c \
		utils/print_uint.c \
		utils/io.c \
		utils/binary_operations.c

OBJS = ${SRCS:.c=.o}

NAME = ft_ssl

# CFLAGS		= -Wall -Wextra -Werror

all :		${NAME}

${NAME} :	${OBJS}
			gcc -o ${NAME} ${OBJS}

test :		${NAME}
			bash test.sh

clean :
			rm -f ${OBJS}

fclean :	clean
			rm -f ${NAME}

re :		fclean all