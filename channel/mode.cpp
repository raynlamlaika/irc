
#include "parsing.hpp"

void Parsing::mode(Client &clinet, std::string line)
{
    std::map<std::string, Channel>& chs = Getchannel();
    std::vector<std::string> splitMode = Parsing::HelperSplit(line, ' ');
    if (splitMode.size() < 2)
    {
        std::cout << " invalid number of argemment \n";
    }
    else if (splitMode[0] == "MODE")
    {
        if (splitMode[1][0] == '#') // deals with channel
        {
            std::map<std::string, Channel>::iterator it = chs.find(splitMode[0]);
            if (it == chs.end())
            {
                std::cout << " channel not exist asbi\n";
                return ; 
            }
            else
            {

            }
        }
        else if (splitMode[1][0] != '#') // deal with client
        {

        }
        else
        {
            
        }
        

    }
     // take name 
        // mode

        // MODE #chan +ie
        // MODE #chan +k password
        // MODE #chan +l 10
        // MODE #chan -k

        // parce the line
        // if (holder.size() <= 2)
        //     std::cout << " invliad number of argemmet\n";
        // else
        // {
        //     std::string name = holder[1];
        //     if (name[0] == '#' || name.size() > 2 || validStr(name))
        //     {
        //         // check if the channel exsist
        //         std::map<std::string, Channel>& chs = Getchannel();
        //         std::map<std::string, Channel>::iterator it = chs.find(name);
        //         if (it == chs.end())
        //         {
        //             std::cout << " channel does not exsist \n";
        //             return (false);
        //         }
        //         else
        //         {
        //             Channel& chan = it->second;
        //             std::string mode = holder[2];
        //             if (mode[0] == '+')
        //             {
        //                 for (size_t i = 1; i < mode.size(); ++i)
        //                 {
        //                     if (mode[i] == 'i')
        //                     {
        //                         // set invite only
        //                         // chan.setInviteOnly(true); // Implement this method in Channel class
        //                         std::cout << "Setting invite-only mode for channel " << name << std::endl;
        //                     }
        //                     // else if (mode[i] == 't')
        //                     // {
        //                     //     // set topic restricted
        //                     //     // chan.setTopicRestricted(true); // Implement this method in Channel class
        //                     //     std::cout << "Setting topic-restricted mode for channel " << name << std::endl;
        //                     // }
        //                     // else if (mode[i] == 'k')
        //                     // {
        //                     //     if (holder.size() < 4)
        //                     //     {
        //                     //         std::cout << "Password not provided for +k mode\n";
        //                     //         return false;
        //                     //     }
        //                     //     std::string key = holder[3];
        //                     //     // chan.setKey(key); // Implement this method in Channel class
        //                     //     std::cout << "Setting key for channel " << name << std::endl;
        //                     // }
        //                     // else if (mode[i] == 'l')
        //                     // {
        //                     //     if (holder.size() < 4)
        //                     //     {
        //                     //         std::cout << "User limit not provided for +l mode\n";
        //                     //         return false;
        //                     //     }
        //                     //     size_t limit = std::stoul(holder[3]);
        //                     //     // chan.setUserLimit(limit); // Implement this method in Channel class
        //                     //     std::cout << "Setting user limit for channel " << name << std::endl;
        //                     // }
        //                 }
        //             }
        //         }
        //     }

        //     std::cout << holder[0] << std::endl;
        // }
}
