

#include "parsing.hpp"

std::string rot13(std::string str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = ((str[i] - 'A' + 13) % 26) + 'A';
        }
        else if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = ((str[i] - 'a' + 13) % 26) + 'a';
        }
    }
    return str;
}

void Parsing::boot(Client &client, std::string msg)
{
    if (msg.empty()) 
    {
        client.sendMsg(":BOOTBOT PRIVMSG " + client.getNick()
            + " :Usage: BOOT <TEXT>");
        return;
    }

    std::string encoded = rot13(msg);

    client.sendMsg(":BOOTBOT PRIVMSG " + client.getNick()
        + " :" + encoded);
}

// void Parsing::boot(Client &client, std::string msg)
// {
//     if (msg.empty())
//     {
//         client.sendMsg(": Empty message | BOOT <message>");
//         return ;
//     }
//     std::string cmd;
//     std::stringstream ss(msg);
//     ss >> cmd;
//     msg.erase(msg.begin(), msg.begin() + (cmd.length()));
//     std::string prompt = "You are an IRC bot. Reply in ONE short sentence only. User: " + msg;

//     std::string command =
//             "curl -s http://localhost:11434/api/generate "
//             "-H \"Content-Type: application/json\" "
//             "-d '{\"model\":\"gemma:2b\",\"prompt\":\"" + prompt + "\",\"stream\":false}' "
//             "| jq -r '.response'";
//     pid_t pid = fork();

//     if (pid < 0) {
//         client.sendMsg(": Fork failed");
//         return ;
//     }
//     else if (pid == 0) {
//         FILE* pipe = popen(command.c_str(), "r");
//         if (!pipe)
//         {
//             client.sendMsg(": popen failed!");
//             exit(1);
//         }

//         std::string result;
//         char buffer[256];

//         while (fgets(buffer, sizeof(buffer), pipe) != NULL)
//             result += buffer;

//         pclose(pipe);
//         client.sendMsg("[AI] : " + result);
//     }
//     else {
//         wait(NULL);
//     }
// }

