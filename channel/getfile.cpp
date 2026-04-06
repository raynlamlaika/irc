


#include "parsing.hpp"


void handleFirstCommand(Client &client, std::string line, std::map<int, Client*> _allClients)
{
    (void)_allClients;
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
        // std::string &buffer = client.getSendBuffer();
        // std::cout << "BUFFER : " << buffer << std::endl;
        std::cout << "second time " << std::endl;
        handelGetHeader(client, line);
    
        std::cout << "++++++++++++++++++" << std::endl;
        std::ofstream fileout(client.getfileout().c_str(), std::ios::binary);
        if (!fileout.is_open())
        {
            client.sendMsg(": can't open this file\n");
            client.setheaderIsGet(false);
            client.setstatusFile(false);
            return;
        }
        std::cout << "is open" <<std::endl;
        // size_t received = 0;
        // char buffer[512];
        std::string buffer = client.getSendBuffer();
        std::string content = buffer.substr(client.getSendBuffer().find('\n'));
        // while (received < client.getsizeFile())
        // {
        //     int bytes = client.receive(buffer, sizeof(buffer));
        //     if (bytes <= 0)
        //         break;

        fileout.write(content.c_str(), client.getsizeFile());
        //     received += bytes;
        // }
        fileout.close();
        client.sendMsg(": File received successfully\n");
    }

}



    // std::string cmd, nick, path;
    // std::stringstream ss(line);
    // ss >> cmd >> nick >> path;

    

    // (void)_allClients;
    // if (checkNick(_allClients, nick))
    // {
    //     client.sendMsg(": Sorry no user with this nick " + nick + "\n");
    //     return;
    // }

    

    // std::cout << "++[ 1 ]++" << std::endl;
    // char header[128];
    // int hbytes = client.receive(header, sizeof(header) - 1);
    // if (hbytes <= 0)
    //     return;

    // header[hbytes] = '\0';
    // std::cout << "++[ 2 ]++ " << header  << std::endl;

    // std::stringstream hss(header);
    // std::string tag;
    // size_t filesize;

    // hss >> tag >> filesize;

    // if (tag != "FILE")
    // {
    //     client.sendMsg(": Invalid file header\n");
    //     return;
    // }

    // size_t received = 0;
    // char buffer[512];

    // while (received < filesize)
    // {
    //     int bytes = client.receive(buffer, sizeof(buffer));
    //     if (bytes <= 0)
    //         break;

    //     fileout.write(buffer, bytes);
    //     received += bytes;
    // }

    // fileout.close();
    // client.sendMsg(": File received successfully\n");





