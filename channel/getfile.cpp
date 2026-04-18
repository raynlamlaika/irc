#include "parsing.hpp"


void Parsing::handleFirstCommand(Client &client, std::string line, std::map<int, Client*> _allClients)
{
    std::string cmd, nick, path;
    std::stringstream ss(line);
    ss >> cmd >> nick >> path;

    if (cmd.empty() || nick.empty() || path.empty())
    {
        client.sendMsg(": USAGE <GETFILE> <NICK> </PATH/TO/FILE>\n");
        client.setheaderIsGet(false);
        client.setstatusFile(false);
        return;
    }
    if (!checkNick(_allClients, nick))
    {
        client.sendMsg(": Internal error: client not found\n");
        client.setheaderIsGet(false);
        client.setstatusFile(false);
        return;
    }
    client.setstatusFile(true);
    client.setfileout(path);
}

void handelGetHeader(Client &client, std::string& line)
{
    std::stringstream hss(client.getSendBuffer());
    std::string tag, cmd;
    size_t filesize;

    hss >> tag >> filesize;

    if (line != "DONE")
    {
        client.setheaderIsGet(false);
        client.setstatusFile(false);
        client.sendMsg(": Invalid file header\n");
        return;
    }
    else
    {
        client.setsizeFile(filesize);
        client.setheaderIsGet(true);
        client.sendMsg(": GET Header \n");
    }
}

void Parsing::getfile(Client &client, std::string line, std::map<int, Client*> _allClients)
{
    if (!client.getstatusFile())
    {
        handleFirstCommand(client, line, _allClients);
        std::cout << "first time " << std::endl;
    }
    else if (client.getstatusFile() && !client.getheaderIsGet())
    {
        handelGetHeader(client, line);
        std::ofstream fileout(client.getfileout().c_str(), std::ios::binary);
        if (!fileout.is_open())
        {
            client.sendMsg(": can't open this file\n");
            client.setheaderIsGet(false);
            client.setstatusFile(false);
            return;
        }
        std::string buffer = client.getSendBuffer();
        std::string content = buffer.substr(client.getSendBuffer().find('\n'));
        fileout.write(content.c_str(), client.getsizeFile());
        fileout.close();
        client.sendMsg(": File received successfully\n");
        client.setheaderIsGet(false);
        client.setstatusFile(false);
        client.setsizeFile(0);
        client.setfileout("");
        client.setBufferEmpty();
    }

}

