
#include "parsing.hpp"


void Parsing::user(Client &client, std::string line)
{
    std::stringstream ss(line);
    std::string cmd, username, mode, unused, realname;

    ss >> cmd >> username >> mode >> unused >> realname;

    std::string server = "ircserv";
    std::string nick = client.getNick().empty() ? "*" : client.getNick();

    size_t pos = line.find(" :");

    if (client.getAuth())
    {
        client.appendSendBuffer(":" + server + " 462 " + nick +
            " :You may not reregister\r\n", _pollFds, 0);
        return;
    }
    if (username.empty() || mode.empty() || unused.empty() || realname.empty())
    {
        client.appendSendBuffer(":" + server + " 461 " + nick +
            " USER :Not enough parameters\r\n", _pollFds, 0);
        return;
    }
    if (std::string::npos != pos)
    {
        realname = line.substr(pos + 2);
    }
    client.setname(username);
    client.setrealname(realname);
}

