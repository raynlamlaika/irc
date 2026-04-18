
#include "parsing.hpp"

Client *getClientByNick(std::map<int, Client*> _allClients, std::string& value)
{
    for (std::map<int, Client*>::iterator it = _allClients.begin(); it != _allClients.end(); ++it)
    {
        if (it->second->getNick() == value)
            return it->second;
    }
    return NULL;
}

void Parsing::sendfile(Client &client, std::string line, std::map<int, Client*> _allClients)
{
    std::string cmd, nick, path;
    std::stringstream ss(line);
    ss >> cmd >> nick >> path;

    if (cmd.empty() || nick.empty() || path.empty())
    {
        client.sendMsg(": USAGE <SENDFILE> <NICK> </PATH/TO/FILE>\n");
        return;
    }
    if (checkNick(_allClients, nick))
    {
        client.sendMsg(": Sorry no user with this nick " + nick + "\n");
        return;
    }
    Client *target = getClientByNick(_allClients, nick);
    if (!target)
    {
        client.sendMsg(": Internal error: client not found\n");
        return;
    }
    std::ifstream filein(path.c_str(), std::ios::binary | std::ios::ate);
    if (!filein.is_open())
    {
        client.sendMsg(": can't open this file\n");
        return;
    }
    std::streamsize filesize = filein.tellg();
    filein.seekg(0, std::ios::beg);
    std::stringstream header;
    header << "FILE " << filesize << "\n";   
    target->appendToSendBuffer(header.str());
    while (true)
    {
        char buffer[512];
        filein.read(buffer, sizeof(buffer));
        std::streamsize bytes = filein.gcount();
        if (bytes <= 0)
            break;
        target->appendToSendBuffer(std::string(buffer, bytes));
    }
    filein.close();
    client.sendMsg(": File sent successfully\n");
    target->sendMsg(": Write DONE\n");
}



