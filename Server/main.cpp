#include "Server.hpp"


int parsingPort(std::string port)
{
    int lenght = port.length();
    if (lenght > 1 && !isdigit(port[0]))
        throw std::runtime_error("port should be intger");
    if (lenght > 2 && port[0] == '0')
        throw std::runtime_error("port can't start by 0");
    else if (lenght == 0 || lenght > 5)
        throw std::runtime_error("port to larg");
    char *end;
    long number = strtol(port.c_str(), &end, 10);
    if (number > 65535)
        throw std::runtime_error("port is greater than max size of port");
    else if (*end != '\0')
        throw std::runtime_error("port should be intger");
    return number;
}

std::string parsingPassword(std::string pass)
{
    int lenght = pass.length();
    if (lenght == 0)
        throw std::runtime_error("password can't be empty");
    if (pass.find(' ') != std::string::npos)
        throw std::runtime_error("password can't have space");
    return pass;
}

int main(int argc, char *argv[])
{
    // dont forgat to add ./irc -------> !!!!! port  -------> !!!!! password
    if (argc != 3)
    {
        std::cerr << "Error ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    try
    {
        std::string pass = parsingPassword(argv[2]);
        int port = parsingPort(argv[1]);
        Server server(port, pass);
        server.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}