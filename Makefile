CXX :=  clang++
CXXFLAGS    :=  -Wall -Wextra -std=c++20
CPPFLAGS	:=	-I ./include

ifeq ($(ENV), dev)
	CXXFLAGS	+=	-g3 -DENABLE_DEBUG
endif

ifeq ($(ENV), dev-asan)
	CXXFLAGS	+=	-fsanitize=address -DENABLE_DEBUG
	LDLIBS	+=	-fsanitize=address
endif

SRC :=  src/main.cpp \
		src/Args.cpp \
		src/Reception.cpp \
		src/Kitchen.cpp \
		src/IPCM.cpp \
		src/PizzAbstract.cpp \
		src/Stock.cpp \
		src/PizzaParty.cpp \
		src/Cook.cpp
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
