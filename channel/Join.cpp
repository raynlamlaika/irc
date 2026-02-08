#include "parsing.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>


std::map<std::string, Channel>& Parsing::Getchannel()
{
    return _channels;
}

void Parsing::add_Channel(const Channel& channel)
{
    const std::string& name = channel.getName();

    if (_channels.find(name) != _channels.end())
        return;

    _channels.insert(std::make_pair(name, channel));
}

std::vector<std::string> parceCammandJoin(std::string line)
{
    // separate the names and keys to loop out of them

    std::vector<std::string> holder;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word)
    {
        // std::cout << word << std::endl;
        holder.push_back(word);
    }
    if (holder.size() >= 1)
    {
        std::string command = holder[0];
        if (command  != "JOIN")
        {
            std::cout  << "not the join cammand\n";
            return holder;
        }
    }
    if (holder.size() >= 2)
        std::string names = holder[1];
    if (holder.size() >= 3)
        std::string names = holder[2];
    return holder;
}

std::vector<std::string> Parsing::HelperSplit(std::string line, char del)
{
    std::vector<std::string> result;
    std::string token;
    std::string::size_type start = 0;
    std::string::size_type pos;

    while ((pos = line.find(del, start)) != std::string::npos)
    {
        std::string token = line.substr(start, pos - start);
        // Trim whitespace from token
        size_t token_start = token.find_first_not_of(" \t\n\r");
        if (token_start != std::string::npos) {
            size_t token_end = token.find_last_not_of(" \t\n\r");
            token = token.substr(token_start, token_end - token_start + 1);
        } else {
            token = ""; // Token is all whitespace
        }
        result.push_back(token);
        start = pos + 1;
    }
    
    // Handle last token
    std::string last_token = line.substr(start);
    size_t token_start = last_token.find_first_not_of(" \t\n\r");
    if (token_start != std::string::npos) {
        size_t token_end = last_token.find_last_not_of(" \t\n\r");
        last_token = last_token.substr(token_start, token_end - token_start + 1);
    } else {
        last_token = ""; // Token is all whitespace
    }
    result.push_back(last_token);

    return result;
}

std::map<std::string, std::string> key_name(std::vector<std::string> line)
{
    std::vector<std::string> names;
    std::vector<std::string> keys;
    std::map<std::string, std::string> holder;
    if (line.size() > 1 && !line[1].empty()) {
        names = Parsing::HelperSplit(line[1], ',');
    }
    if (line.size() > 2 && !line[2].empty()) {
        keys = Parsing::HelperSplit(line[2], ',');
    }

    for (size_t i = 0; i < names.size(); ++i) {
        if (i < keys.size() && !keys[i].empty())
            holder.insert(std::make_pair(names[i], keys[i]));
        else
            holder.insert(std::make_pair(names[i], ""));
    }
    return holder;
}

void Parsing::join(Client &client, std::string line)
{
    std::map<std::string, Channel>& chs = Getchannel();

    // Parse the line
    std::vector<std::string> parsed = parceCammandJoin(line);
    if (parsed.size() < 2) {
        std::cout << "JOIN command missing channel names\n";
        return;
    }

    std::map<std::string, std::string> NamesKeys = key_name(parsed);

    for (auto it = NamesKeys.begin(); it != NamesKeys.end(); ++it) {
        const std::string& channelName = it->first;
        const std::string& key = it->second;
        auto chIt = chs.find(channelName);
        if (chIt == chs.end())
        {
            // Channel does not exist, create it
            Channel newChannel(channelName);
            if (!key.empty())
                newChannel.setKey(key);
            newChannel.addClient(client);
            add_Channel(newChannel);
        }
        else
        {
            Channel& channel = chIt->second;
            std::cout << "Joining channel: " << channelName << (key.empty() ? "" : " with key: " + key) << "\n";
            std::cout << "Channel " << channelName << " has key: " << (channel.hasKey() ? channel.getKey() : "none") << "\n";
            if (channel.hasKey())
            {
                if (!key.empty()) {
                    if (channel.getKey() == key)
                    {
                        channel.addClient(client);
                    }
                    else {
                        std::cout << "Incorrect key for channel: " << channelName << " with key: " << key << " this is the correct key: " << channel.getKey() << "\n";
                    }
                } else {
                    std::cout << "Key required for channel: " << channelName << "\n";
                }
            } else {
                channel.addClient(client);
            }
        }
    }

    std::cout << "\t\t\t------this is all of the channels------\t\t\t\n";
    for (auto it1 = chs.begin(); it1 != chs.end(); ++it1) {
        std::cout << "Channel : " << it1->first << ": " << it1->second.getName() << std::endl;
    }
}

