CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SERVER = ircserv

SERVER_SRC = ./Server/main.cpp ./Server/Server.cpp ./channel/parsing.cpp ./channel/channel.cpp ./Server/Client.cpp \
				./channel/Join.cpp ./channel/mode.cpp ./channel/kick.cpp ./channel/prvmsg.cpp ./channel/pass.cpp \
				./channel/nick.cpp ./channel/user.cpp ./channel/getfile.cpp ./channel/sendfile.cpp ./channel/boot.cpp \
				./channel/invite.cpp

HEADERS = ./Server/Server.hpp ./Server/client.hpp ./channel/parsing.hpp

SERVER_OBJ = $(SERVER_SRC:.cpp=.o)

all: $(SERVER)


$(SERVER): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f  $(SERVER_OBJ)

fclean : clean
	rm -f $(SERVER)

re: fclean all

.PHONY: clean 
