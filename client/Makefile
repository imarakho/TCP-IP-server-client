NAME = client
SRC = main.cpp \
	Client.cpp

OBJ = $(SRC:.cpp=.o)

FLAGS = -I /Users/imarakho/.brew/opt/boost@1.67/include/ -L /Users/imarakho/.brew/opt/boost@1.67/lib/ -lboost_system

all: $(NAME)

$(NAME): $(OBJ)
	@clang++ $(FLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	clang++ $(FLAGS) -o $@ -c $<

clean:
	rm	-rf $(OBJ)

fclean: clean
	rm	-rf $(NAME)

re: fclean all