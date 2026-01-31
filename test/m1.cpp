
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <cstdio>
#include <vector>


int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        std::cerr << "Erorr : fail to run socket" << std::endl;
        return 1;
    }
    std::cout << "main ser : " << server_fd << std::endl;
    const int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(1234);

    if (inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }


    if (bind(server_fd, (sockaddr*)&hint, sizeof(hint)) < 0)
    {
        perror("bind");
        return 1;
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        return 1;
    }
    std::cout << "Server is renning on port " << 1234 << std::endl;

    std::vector<pollfd> fds;
    pollfd server_poll;
    server_poll.fd = server_fd;
    server_poll.events = POLLIN;
    fds.push_back(server_poll);

    while (1)
    {
        int ret = poll(&fds[0], fds.size(), -1);
        if (ret < 0)
        {
            std::cerr << "Error : poll" << std::endl;
            break;
        }
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == server_fd)
                {
                    sockaddr_in clinet;
                    socklen_t len = sizeof(clinet);
                    int client_fd = accept(server_fd, (sockaddr*)&clinet, &len);
                    if (client_fd >= 0)
                    {
                        pollfd cli;
                        cli.fd = client_fd;
                        cli.events = POLLIN;
                        fds.push_back(cli);
                        std::cout << "New client : fd["<< client_fd << "]" << std::endl;
                        std::string reply = "hello from server\n";
                        send(client_fd, reply.c_str(), reply.size(), 0);
                    }
                }
                else
                {
                    char buffer[512];
                    int bytes = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                    if (bytes <= 0)
                    {
                        std::cout << "Client disconnected fd[" << fds[i].fd << "]\n";
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--;
                    }
                    else
                    {
                        buffer[bytes] = '\0';
                        // creat in object of client


                        // check command

                        std::cout << "Client[" << fds[i].fd << "] -> " << buffer;

                        for (size_t j = 0; j < fds.size(); j++)
                        {
                            if (fds[j].fd != server_fd && fds[j].fd != fds[i].fd)
                            {
                                send(fds[j].fd, buffer, bytes, 0);
                            }
                        }
                    
                    }
                }
            }
        }
    }


    return 0;
}






