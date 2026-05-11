CXX :=  clang++
CXXFLAGS    :=  -Wall -Wextra -std=c++20

ifeq ($(ENV), dev)
	CXXFLAGS	+=	-g3
endif

ifeq ($(ENV), dev-asan)
	CXXFLAGS	+=	-fsanitize=address
	LDLIBS	+=	-fsanitize=address
endif

SRC :=  src/main.cpp
OBJ :=  $(SRC:.cpp=.o)

BINARY  :=  plazza

all:    $(BINARY)

$(BINARY):	$(OBJ)
	$(CXX) -o $(BINARY) $(OBJ) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(BINARY)

re: fclean all

.PHONY: all clean fclean re
