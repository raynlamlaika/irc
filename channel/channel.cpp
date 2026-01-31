#include "channel.hpp"



Channel::Channel(const std::string& name) : _name(name), _key("")
{
    // if ther is not keys 
    
    _topic = "";
    _inviteOnly =  false;
    _topicRestricted =  false;
    _userLimit = 0;
}

Channel::~Channel()
{
    
}
Channel::Channel(std::string key, const std::string& name) : _name(name), _key(key) 
{
    
    _topic = "";
    _inviteOnly =  false;
    _topicRestricted =  false;
    _userLimit = 0;
}


/*--- ----*/


    // Membership
void Channel::addClient(Client& c)
{
    // MAY check if the c is null or have no value later

    // check if the client already in the channel
    // _members[c.getFd()] = &c;
    // check if there is  in key needed
    std::map<int, Client*>::iterator it = _members.find(c.getFd());
    // user not exesit i the channel
    if (it == _members.end())
        _members[c.getFd()] = &c; // add the user
    else
        std::cout << " the client allready exist\n";
    
}

void Channel::removeClient(Client& c)
{
    // check if the client already in the channel
    std::map<int, Client*>::iterator it = _members.find(c.getFd());

    // user not exesit i the channel
    if (it != _members.end())
        _members.erase(c.getFd()); // remove the user
    else
        std::cout << " the client not exist\n";
}


bool Channel::hasClient(Client& c)
{
    return _members.find(c.getFd()) != _members.end();
}

/*---    ---*/

bool Channel::hasKey() const
{
    if (this->_key != "")
        return false;
    return true;
}

bool Channel::isInviteOnly() const
{
    return(this->_inviteOnly);
}

bool Channel::isTopicRestricted() const
{
    return (this->_topicRestricted);
}

bool Channel::hasUserLimit() const
{
    return this->_userLimit;
}

const std::string& Channel::getName() const
{
    return (this->_name);
}

const std::string& Channel::getTopic() const
{
    return (this->_topic);
}

std::string Channel::getKey() const
{
    return (this->_key);
}

void  Channel::setKey(std::string key)
{
    this->_key = key;
}
