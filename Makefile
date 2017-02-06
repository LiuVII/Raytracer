# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mfilipch <mfilipch@student.42.us.org>       +#+  +:+       +#+        #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/26 17:34:20 by mfilipch          #+#    #+#              #
#    Updated: 2016/10/26 17:34:22 by mfilipch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	RTv1

FUNCS		+=	main.c\
				hooks.c\
				read.c\
				draw.c\
				process.c\
				raytrace.c\
				intersection.c\
				effects.c\
				rec_data.c\
				vect_dist.c\
				vect_dmath.c\
				vect_imath.c\
				ft_trs.c\

OBJS		=	$(addprefix $(OBJS_DIR), $(FUNCS:.c=.o))

LIB			=	./libft/libft.a
LIBINC		=	-I./libft
LIBLINK		=	-L./libft -lft

MLX			=	./minilibx/libmlx.a
MLXINC		=	-I./minilibx
MLXLINK		=	-L./minilibx -lmlx -framework OpenGL -framework AppKit

CLNAME		=	mykernel.cl
CLSRC		=	$(CLNAME:.cl=.cl.c)
CLOBJ		=	$(CLNAME:.cl=.cl.o)
OPENCLC		=	/System/Library/Frameworks/OpenCL.framework/Libraries/openclc
CLFLAGS		=	-x cl -cl-std=CL1.1 -Os
CLLINK		=	-framework OpenCL
CLNORMFLG	=	-x c -arch x86_64

i386NAME	=	$(CLNAME:.cl=.cl.i386.bc)
x86_64NAME	=	$(CLNAME:.cl=.cl.x86_64.bc)
GPUNAME		=	$(CLNAME:.cl=.cl.gpu_32.bc)
GPU64NAME	=	$(CLNAME:.cl=.cl.gpu_64.bc)

PTHREADLINK	=	-lpthread

OBJS_DIR	=	./build/
INC_DIR		=	./includes/

CC			=	gcc
FLAGS		=	-Wall -Werror -Wextra -O2 -march=native -funroll-loops

.PHONY: all clean flcean $(NAME) re 

all: $(LIB) $(MLX) $(NAME)

build:
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o: %.c | build
	@$(CC) $(FLAGS) $(CLNORMFLG) $(LIBINC) $(MLXINC) -I $(INC_DIR) -c $< -o $@

$(LIB):
	@make -C ./libft

$(MLX):
	@make -C ./minilibx

# $(CLNAME):
# 	@$(OPENCLC) $(CLFLAGS) -emit-gcl $(CLNAME)

# arch: $(CLNAME)
# 	@$(OPENCLC) $(CLFLAGS) -arch i386 -emit-llvm -c $(CLNAME) -o $(i386NAME)
# 	@$(OPENCLC) $(CLFLAGS) -arch x86_64 -emit-llvm -c $(CLNAME) -o $(x86_64NAME)
# 	@$(OPENCLC) $(CLFLAGS) -arch gpu_32 -emit-llvm -c $(CLNAME) -o $(GPUNAME)
# 	@$(OPENCLC) $(CLFLAGS) -arch gpu_64 -emit-llvm -c $(CLNAME) -o $(GPU64NAME)

# $(CLOBJ): arch
# 	@$(CC) -c $(GCCFLAGS) -x c -arch x86_64 -o $(CLOBJ) $(CLSRC)

$(NAME): $(OBJS) #$(CLOBJ)
#	@$(CC) -o $(NAME) $(OBJS) $(CLOBJ) $(LIBLINK) $(MLXLINK) $(PTHREADLINK) $(CLLINK)
	@$(CC) -o $(NAME) $(OBJS) $(LIBLINK) $(MLXLINK) $(PTHREADLINK)

clean:
	@/bin/rm -rf $(OBJS_DIR)
	@make -C ./libft clean
	@make -C ./minilibx clean
	@/bin/rm -f *.o
	@/bin/rm -f *.out
	@/bin/rm -f ._*
	@/bin/rm -f .DS*

fclean: clean
	@/bin/rm -f $(NAME)
	@/bin/rm -f $(i386NAME)
	@/bin/rm -f $(x86_64NAME)
	@/bin/rm -f $(GPUNAME)
	@/bin/rm -f $(GPU64NAME)
	@/bin/rm -f $(i386NAME)
	@/bin/rm -f $(CLSRC)
	@make -C ./libft fclean

re: fclean all
