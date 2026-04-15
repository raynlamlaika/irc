
#include "parsing.hpp"


void Parsing::user(Client &client, std::string line)
{
    std::string cmd, value, hostname, servername, realname;
    std::stringstream ss(line);
    ss >> cmd >> value >> hostname >> servername >> realname;

    if (!client.getPass())
        client.sendMsg(": You may not reregister\r\n");
    else
    {
        if (value.empty() || hostname.empty() || servername.empty() || realname.empty())
            client.sendMsg(": Not enough parameters\r\n");
        else if (!client.getName().empty())
            client.sendMsg(": You may not reregister\r\n");
        else
        {
            if (realname[0] == ':')
            {
                if (realname.size() == 1)
                {
                    client.sendMsg(": Error in realname");
                    return;
                }
                std::string newvalue = line.substr(line.find(realname) + 1);
                client.setrealname(newvalue);
                client.setname(value);
            } else
            {
                client.setrealname(realname);
                client.setname(value);
            }
        }
    }
}


