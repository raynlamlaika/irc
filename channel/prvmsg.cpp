
#include "parsing.hpp"

void Parsing::prvmsghelpre(bool flag, std::string message, Channel &ref, Client& refClinet) // 1 for users, 0 for channels
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
        if (line.find(":") == std::string::npos)
            {std::cout <<"invalid format\n";return;}
        std::string message = line.substr(index, line.size() - 1);


        if (holder[0] != "PRIVMSG")
            return;
        if (isalpha(target[0]))
        {
            //dealing with clinet
            Channel *ref = NULL;
            seachForClient(target);
            Client *holder =  searchForClientref(target);
            if (holder == NULL)
                {std::cout << "cant find user\n" ;return;}
            prvmsghelpre(1, message, *ref , *holder);
        }
        else
        {
            Client *ref = NULL;
            // check channel syntax it is valid channel name
            // search  if it exist
            if(!searchForChannel(target))
                {std::cout << " invalid channel \n" ;return ;}
            Channel *holder = searchForChannelref(target);
            // apply the private msg
            prvmsghelpre(0, message,*holder, *ref );
        }
    }

}