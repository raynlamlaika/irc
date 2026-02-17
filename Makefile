CXX = c++
CXXFLAGS = -Wall -Wextra -std=c++98 -g

SERVER = server

SERVER_SRC = ./Server/main.cpp ./Server/Server.cpp ./channel/parsing.cpp ./channel/channel.cpp ./Server/Client.cpp \
				./channel/Join.cpp ./channel/mode.cpp ./channel/kick.cpp ./channel/prvmsg.cpp 

HEADERS = ./Server/Server.hpp ./Server/client.hpp

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
