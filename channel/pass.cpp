
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
        client.appendSendBuffer(":" + server + " 462 " + nick +
            " :You may not reregister\r\n", _pollFds);
        return;
    }

    if (value.empty())
    {
        client.appendSendBuffer(":" + server + " 461 " + nick +
            " PASS :Not enough parameters\r\n", _pollFds);
        return;
    }

    if (value != client.password)
    {
        client.appendSendBuffer(":" + server + " 464 " + nick +
            " :Password incorrect\r\n", _pollFds);
        return;
    }

    client.setpass();
}

