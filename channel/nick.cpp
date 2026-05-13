
#include "parsing.hpp"

bool isValidNick(const std::string& nick)
{
    if (nick.empty())
        return false;

    if (nick[0] == '#' || nick[0] == ':')
        return false;

    for (size_t i = 0; i < nick.size(); i++)
    {
        if (nick[i] == ' ' || !std::isprint(nick[i]))
            return false;
    }

    return true;
}
bool Parsing::checkNick(std::map<int, Client*> _allClients, std::string& value)
{
    for (std::map<int, Client*>::iterator it = _allClients.begin(); it != _allClients.end(); ++it)
    {
        if (it->second->getNick() == value)
            return false;
    }
    return true;
}


void Parsing::nick(Client &client, std::string line, std::map<int, Client*> _allClients)
{
    std::stringstream ss(line);
    std::string cmd, value;
    ss >> cmd >> value;

    std::string server = "ircserv";
    std::string nick = client.getNick().empty() ? "*" : client.getNick();

    if (value.empty())
    {
        client.appendSendBuffer(":" + server + " 431 " + nick +
            " :No nickname given\r\n", _pollFds);
        return;
    }

    if (!isValidNick(value))
    {
        client.appendSendBuffer(":" + server + " 432 " + nick +
            " " + value + " :Erroneus nickname\r\n", _pollFds);
        return;
    }

    if (!checkNick(_allClients, value))
    {
        client.appendSendBuffer(":" + server + " 433 " + nick +
            " " + value + " :Nickname is already in use\r\n", _pollFds);
        return;
    }

    std::string oldNick = client.getNick();

    client.setNick(value);

    if (!oldNick.empty())
    {
        std::string msg = ":" + oldNick + "!" +
            client.getName() + "@"+ _gethostname() +" NICK :" + value + "\r\n";
        client.appendSendBuffer(msg, _pollFds);
    }
}


