
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <cstring>

int main()
{
    int port = 9090;
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0)
    {
        std::cout << "Error socket" << std::endl;
        return(1);
    }
    int opt = 1;
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(serverfd, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        std::cout << "Error in bind" << std::endl;
        return 1;
    }
    if (listen(serverfd, SOMAXCONN) < 0)
    {
        std::cout << "list failed" << std::endl;
        return 1;
    }
    
    return 0;
}




