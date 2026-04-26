
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include  "../Server/Client.hpp"

class Client;

class Channel
{
private:
    std::string _name;
    std::string _topic;
    std::string _key;


    std::set<int> _invited; // invated client "fds"
    std::map<int, Client*> _members;
    std::set<Client*> _operators;
    std::vector<Client*> _banned;



    bool _inviteOnly; // +i
    bool _topicRestricted; // +t
    size_t _userLimit; // +l 
    
    std::string _topicOwner;
    time_t _topicSetTime ;

public:
// make a defulaut constractor
    Channel(const std::string& name);
    ~Channel();
// make a  parameteras 
    Channel(std::string key ,const std::string& name);

    std::string getTopicOwner() const;
    time_t getTopicSetTime() const;
    void setTopicOwner(std::string name);
    void setTopicSetTime(time_t time);

    // Basic info
    const std::string& getName() const;
    const std::string& getTopic() const;

    // Membership
    void addClient(Client* c);
    void removeClient(Client* c);
    bool hasClient(Client* c);

    bool hasKey() const;
    std::string getKey() const;
    bool isInviteOnly() const;
    bool isTopicRestricted() const;
    void setInviteOnly(bool inviteOnly);
    bool getInviteOnly() const;
    std::map<int, Client*> getMembers() const;
    bool hasUserLimit() const;
    size_t getUserLimit() const;
    void  setKey(std::string key);
    void setTopic(std::string key);
    std::map<int, Client*> getmembers ();
    std::set<Client*> getoperators() const;
    void addOperator(Client* client);
    void removeOperator(Client* client);
    void addInvited(Client* client);
    void removeInvited(Client* client);
    bool isInvited(const Client& client) const;
    void setUserLimit(size_t limit);
    bool isBanned(const Client& client) const;
    bool isOperator(const Client& client) const;
    void broadcastMsg(const std::string& msg, const std::map<int, Client*>& members);

    // bool addOperator(const Client& client) const;
};

bool validName(std::string name, Client *client);
#endif