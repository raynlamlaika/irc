#include "parsing.hpp"

std::string rot13(std::string str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = ((str[i] - 'A' + 13) % 26) + 'A';
        }
        else if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = ((str[i] - 'a' + 13) % 26) + 'a';
        }
    }
    return str;
}

void Parsing::boot(Client &client, std::string msg)
{
    msg = msg.erase(0, std::string("BOOT").length() + 1);
    if (msg.empty()) 
    {
        client.appendSendBuffer(":BOOTBOT PRIVMSG " + client.getNick()
            + " :Usage: BOOT <TEXT>\r\n", _pollFds, 0);
        return;
    }

    std::string encoded = rot13(msg);
    client.appendSendBuffer(":BOOTBOT PRIVMSG " + client.getNick()
        + " :" + encoded +"\r\n", _pollFds, 0);
}
