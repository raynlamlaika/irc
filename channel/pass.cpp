
#include "parsing.hpp"

void Parsing::pass(Client &client, std::string line)
{
    std::string cmd, value;
    std::stringstream ss(line);
    ss >> cmd >> value;

    std::string server = "ircserv";
    std::string nick = client.getNick().empty() ? "*" : client.getNick();

    if (client.getAuth())
    {
        client.sendMsg(":" + server + " 462 " + nick +
            " :You may not reregister\r\n");
        return;
    }

    if (value.empty())
    {
        client.sendMsg(":" + server + " 461 " + nick +
            " PASS :Not enough parameters\r\n");
        return;
    }

    if (value != client.password)
    {
        client.sendMsg(":" + server + " 464 " + nick +
            " :Password incorrect\r\n");
        return;
    }

    client.setpass();
}

