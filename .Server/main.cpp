#include "Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error ./server password" << std::endl;
        return 1;
    }
    try
    {
        std::string pass = argv[1];
        Server server(1234, pass);
        server.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}