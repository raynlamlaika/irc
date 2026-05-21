CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SERVER = ircserv
SERVER_BONUS = ircserv_bonus

M_SERVER_SRC = ./mandatory/Server/main.cpp ./mandatory/Server/Server.cpp ./mandatory/channel/parsing.cpp ./mandatory/Server/Client.cpp \
			 ./mandatory/channel/Join.cpp ./mandatory/channel/mode.cpp ./mandatory/channel/prvmsg.cpp ./mandatory/channel/pass.cpp \
			 ./mandatory/channel/nick.cpp ./mandatory/channel/user.cpp ./mandatory/channel/channel.cpp \
			 ./mandatory/channel/invite.cpp ./mandatory/channel/kick.cpp
			   

B_SERVER_SRC = ./bonus/Server/main.cpp ./bonus/Server/Server.cpp ./bonus/channel/parsing.cpp ./bonus/Server/Client.cpp \
			 ./bonus/channel/Join.cpp ./bonus/channel/mode.cpp ./bonus/channel/prvmsg.cpp ./bonus/channel/pass.cpp \
			 ./bonus/channel/nick.cpp ./bonus/channel/user.cpp ./bonus/channel/channel.cpp \
			 ./bonus/channel/invite.cpp ./bonus/channel/kick.cpp ./bonus/channel/bot.cpp

M_HEADERS = mandatory/Server/Client.hpp mandatory/Server/Server.hpp mandatory/channel/channel.hpp \
			mandatory/channel/parsing.hpp mandatory/channel/replices.hpp

B_HEADERS = bonus/Server/Client.hpp bonus/Server/Server.hpp bonus/channel/channel.hpp \
			bonus/channel/parsing.hpp bonus/channel/replices.hpp

SERVER_OBJ = $(M_SERVER_SRC:.cpp=.o)
BONUS_OBJ = $(B_SERVER_SRC:.cpp=.o)

all: $(SERVER)

bonus: $(SERVER_BONUS)

$(SERVER): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SERVER_BONUS): $(BONUS_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^


bonus/%.o: bonus/%.cpp $(B_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


mandatory/%.o: mandatory/%.cpp $(M_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(SERVER_BONUS)
	@mkdir -p logs
	@stdbuf -oL -eL ./$(SERVER_BONUS) 6667 password 2>&1 | tee logs/server.log &
	cd irc-back-end && mvn spring-boot:run


clean:
	rm -f $(SERVER_OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(SERVER) $(SERVER_BONUS)

re: fclean all

.PHONY: clean run