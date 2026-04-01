
#include "parsing.hpp"

void Parsing::pass(Client &client, std::string line)
{
    std::string cmd, value;
    std::stringstream ss(line);
    ss >> cmd >> value;
    std::cout <<"v: " << "'" << value << "'"  << std::endl;
    if (client.getPass())
        client.sendMsg(": You may not reregister\r\n");
    else
    {
        if (value.empty())
            client.sendMsg(": Not enough parameters\r\n");
        else if (value != client.password)
            client.sendMsg(": Password incorrect\r\n");
        else
            client.setpass();
    }
    // std::cout <<"line is : [cmd] : "  << cmd << " [val] : " << value << std::endl;
    // client.sendMsg(": oky !\r\n");
}

