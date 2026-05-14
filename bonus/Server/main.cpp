#include "Server.hpp"


int parsingPort(std::string port)
{
    if (port.length() == 0)
        throw std::runtime_error("port is empty");
    char *end;
    long number = strtol(port.c_str(), &end, 10);
    if (number <= 0 || number > 65535)
        throw std::runtime_error("port is greater than max size of port");
    else if (*end != '\0')
        throw std::runtime_error("port should be intger");
    return number;
}

std::string parsingPassword(std::string pass)
{
    if (pass.empty())
        throw std::runtime_error("password can't be empty");

    for (size_t i = 0; i < pass.length(); i++)
    {
        if (!isprint(pass[i]))
            throw std::runtime_error("password contains non printable characters");
        if (pass[i] == ' ' || pass[i] == '\t')
            throw std::runtime_error("password can't contain spaces or tabs");
    }

    return pass;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Error ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    try
    {
        signal(SIGPIPE, SIG_IGN);
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