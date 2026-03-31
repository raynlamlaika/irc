#include "Client.hpp"



int Client::getFd() const
{
    return _fd;
}

int Client::numberOfChannels() const 
{
    return numberOfChannelsJoined;
}

void Client::setname(std::string nameHolder)
{
    name = nameHolder;
}

Client::Client()
{
    // _addr.sin_port = 133;
    // numberOfChannelsJoined = 0;
    // std::cout << "[+] Client connected fd[" << _fd << "]\n";
}

Client::Client(int fd, const sockaddr_in &addr, std::string password) 
    : _fd(fd), _addr(addr), numberOfChannelsJoined(0), name(""), nick(""), pass(false), auth(false), password(password)
{
    //_addr.sin_port = 133;
    std::cout << "Client connected fd[" << _fd << "]\n";
}

Client::~Client()
{
    std::cout << "Client disconnected fd[" << _fd << "]\n";
    close(_fd);
}
std::string Client::getName() const
{
    return name;
}

int Client::receive(char *buffer, size_t size)
{
    return recv(_fd, buffer, size, 0);
}

void Client::sendMsg(const std::string &msg)
{
    send(_fd, msg.c_str(), msg.size(), 0);
}


void Client::incrementChannels()
{
    numberOfChannelsJoined++;
}

void Client::decrementChannels()
{
    numberOfChannelsJoined--;
}

std::string Client::readLine()
{
    std::string content;

    while (true)
    {
        char buffer[1024];
        int byte = this->receive(buffer, sizeof(buffer));

        if (byte <= 0)
            return "";

        content.append(buffer, byte);

        if (!content.empty() && content[content.length() - 1] == '\n')
            break;
    }

    if (!content.empty() && content[content.length() - 1] == '\n')
        content.erase(content.length() - 1);

    return content;
}

bool Client::getAuth()
{
    return auth;
}

bool Client::getPass()
{
    return pass;
}

void Client::setpass()
{
    pass = true;
}

void Client::setNick(std::string &value)
{
    nick = value;
}

void Client::get_informatoin()
{
    while (!auth)
    {
        std::string line = readLine();

        if (line.empty())
        {
            std::cout << "Client disconnected\n";
            return;
        }

        std::cout << "Received: " << line << std::endl;

        handleCommand(line);
    }
    this->sendMsg("Auth complete\n\r");
}

void Client::handleCommand(const std::string &line)
{
    std::istringstream iss(line);
    std::string cmd, value;

    iss >> cmd >> value;

    if (cmd == "PASS")
    {
        if (value == password)
        {
            pass = true;
            std::cout << "PASS OK\n";
        }
        else
            this->sendMsg(":Password incorrect\n\r");
            // std::cout << "Wrong PASS\n";
    }
    else if (cmd == "NICK")
    {
        if (!pass)
        {
            this->sendMsg(":You have not registered");
            // std::cout << "Need PASS first\n";
            return;
        }
        nick = value;
        std::cout << "Nick: " << nick << "\n";
    }
    else if (cmd == "USER")
    {
        if (!pass)
        {
            std::cout << "Need PASS + NICK\n";
            return;
        }
        name = value;
        std::cout << "User: " << name << "\n";
    }
    else
    {
        this->sendMsg(":You have not registered");
    }

    if (pass && !nick.empty() && !name.empty())
    {
        auth = true;
    }
}


