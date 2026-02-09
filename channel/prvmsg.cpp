
#include "parsing.hpp"

void Parsing::prvmsghelpre(bool flag, std::string message, Channel &ref, Client& refClient) // 1 for users, 0 for channels
{
    if (flag)
        std::cout << "passs a msg to client\n";
    else
        std::cout << "passs a msg to cHANNLE\n";

}


void Parsing::prvmsg(std::string line)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 2)
        {std::cout << " invalid number of args";return;}
    else
    {
        std::string target = holder[1];
        size_t index = line.find(":");
        if (index == std::string::npos)
            {std::cout <<"invalid format\n";return;}
        std::string message = line.substr(index + 1);

        if (holder[0] != "PRIVMSG")
            return;
        if (isalpha(target[0]))
        {
            //dealing with client
            std::cout << "this is the target client: " << target << "\n";
            std::cout << "Searching for client in channels...\n";
            if(!searchForClient(target))
                {std::cout << "cant find user in any channel\n" ;return;}
            
            std::cout << "Client found in channels, getting reference...\n";
            Client *clientHolder = searchForClientref(target);
            
            if (clientHolder == NULL)
                {std::cout << "cant find user\n" ;return;}
            // Send message to client (avoiding NULL dereference)
            clientHolder->sendMsg(message);
            std::cout << "Sending message to client: " << target << " - " << message << std::endl;
        }
        else
        {
            // check channel syntax it is valid channel name
            // search if it exist
            if(!searchForChannel(target))
                {std::cout << "invalid channel\n" ;return ;}
            Channel *channelHolder = searchForChannelref(target);
            if (channelHolder == NULL)
                {std::cout << "cant find channel\n" ;return;}
            // Send message to channel (avoiding NULL dereference)
            std::cout << "Broadcasting message to channel: " << target << " - " << message << std::endl;
        }
    }
}