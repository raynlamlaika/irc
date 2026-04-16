
#include "parsing.hpp"

void Parsing::pass(Client &client, std::string line)
{
    std::string cmd, value;
    std::stringstream ss(line);
    ss >> cmd >> value;
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
}

