
#include "parsing.hpp"

bool isStringPrintable(const std::string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isprint(str[i])) {
            return false;
        }
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
    std::string cmd, value;
    std::stringstream ss(line);
    ss >> cmd >> value;
    if (!client.getPass())
        client.sendMsg(": You may not reregister\r\n");
    else
    {
        if (value.empty())
            client.sendMsg(": No nickname given\r\n");
        else if (!isStringPrintable(value))
            client.sendMsg(": Erroneus nickname\r\n");
        else if (!checkNick(_allClients, value))
            client.sendMsg(": Nickname is already in use\r\n");
        else
            client.setNick(value);
    }
}


