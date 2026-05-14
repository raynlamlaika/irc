CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 #-g3 -fsanitize=address

SERVER = ircserv
SERVER_BONUS = ircserv_bonus

M_SERVER_SRC = ./Server/main.cpp ./Server/Server.cpp ./channel/parsing.cpp ./Server/Client.cpp \
			 ./channel/Join.cpp ./channel/mode.cpp ./channel/prvmsg.cpp ./channel/pass.cpp \
			 ./channel/nick.cpp ./channel/user.cpp ./channel/channel.cpp \
			 ./channel/invite.cpp ./channel/kick.cpp
			   

B_SERVER_SRC = ./bonus/Server/main.cpp ./bonus/Server/Server.cpp ./bonus/channel/parsing.cpp ./bonus/Server/Client.cpp \
			 ./bonus/channel/Join.cpp ./bonus/channel/mode.cpp ./bonus/channel/prvmsg.cpp ./bonus/channel/pass.cpp \
			 ./bonus/channel/nick.cpp ./bonus/channel/user.cpp ./bonus/channel/channel.cpp \
			 ./bonus/channel/invite.cpp ./bonus/channel/kick.cpp ./bonus/channel/bot.cpp

M_HEADERS = Server/Client.hpp Server/Server.hpp channel/channel.hpp \
			channel/parsing.hpp channel/replices.hpp

B_HEADERS = bonus/Server/Client.hpp bonus/Server/Server.hpp channel/channel.hpp \
			bonus/channel/parsing.hpp bonus/channel/replices.hpp

SERVER_OBJ = $(M_SERVER_SRC:.cpp=.o)
BONUS_OBJ = $(B_SERVER_SRC:.cpp=.o)

all: $(SERVER)

bonus: $(SERVER_BONUS)

$(SERVER): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SERVER_BONUS): $(BONUS_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(M_HEADERS) $(B_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(SERVER) $(SERVER_BONUS)

re: fclean all

.PHONY: all bonus clean fclean re