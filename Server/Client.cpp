#include "Client.hpp"



int Client::getFd() const
{
    return _fd;
}



void Client::setname(std::string nameHolder)
{
    name = nameHolder;
}
