#include "client.hpp"

client::client() : nickName("Default"), realName("Default"),userName("Default"),
                    hostName("Default"),isOperator(false),welcomeMessageSent(false),
                    numberChannelJoined(0) 
{

}

client::client(std::string hostNameParam, int fd) : nickName("Default"), realName("Default"),userName("Default"),
                    hostName(hostNameParam),isOperator(false),welcomeMessageSent(false),
                    socketFd(fd), numberChannelJoined(0)  
{

}

client::client(const client& copy) : nickName(copy.nickName), realName(copy.realName),userName(copy.realName),
                    hostName(copy.hostName),isOperator(copy.isOperator),welcomeMessageSent(copy.welcomeMessageSent),
                    socketFd(copy.socketFd), port(copy.port), ip(copy.ip),numberChannelJoined(copy.numberChannelJoined)  
{

}

client& client::operator=(const client& copy)
{
    (void)copy;
    return (*this);
}

client::~client()
{

}

std::string client::getNickName()
{
    return (this->nickName);
}
std::string client::getRealName()
{
    return (this->realName);
}
std::string client::getUserName()
{
    return (this->userName);
}
std::string client::getHostName()
{
    return (this->hostName);
}
bool        client::getOperatorStatus()
{
    return (this->isOperator);
}
bool        client::isWelcomeMessageSent()
{
    return (this->welcomeMessageSent);
}
int         client::getsocketFd()
{
    return (this->socketFd);
}
int         client::getPort()
{
    return (this->port);
}
char *         client::getIp()
{
    return (this->ip);
}
int         client::getNumberOfChannelJoined()
{
    return (numberChannelJoined);
}

std::string client::getMode()
{
    return (this->mode);
}
//setterclient::
void        client::setNickName(std::string str)
{
    this->nickName = str;
}
void        client::setRealName(std::string str)
{
    this->realName = str;
}
void        client::setUserName(std::string str)
{
    this->userName = str;
}
void        client::setHostName(std::string str)
{
    this->hostName = str;
}
void        client::setOperatorStatus(bool status)
{
    this->isOperator = status;
}
void        client::setWelcomeMessageSent(bool status)
{
    this->welcomeMessageSent = status;
}
void        client::setsocketFd(int fd)
{
    this->socketFd = fd;
}
void        client::setPort(int port)
{
    this->port = port;
}
void        client::setIp(char *ip)
{
    this->ip = ip;
}
void        client::setNumberOfChannelJoined(int nb)
{
    this->numberChannelJoined = nb;
}

void client::setMode(std::string mode)
{
    this->mode = mode;
}

// utils

void client::fillStrParam(std::string str, client* clientPtr)
{
    char **tabSplit;
    char **splitNickname;
    tabSplit = ft_split(str.c_str(), ' ');
    //std::cout << "FILL StrParam\n";
    int i = 0;
    int argUser = 1;
    while(tabSplit[i])
    {
        std::string temp;
        temp = tabSplit[i];
        if (tabSplit[i] && temp.find("NICK") != std::string::npos)
        {
            i++;
            if (tabSplit[i])
            {
                splitNickname = ft_split(tabSplit[i], '\r');
                clientPtr->setNickName(copyString(splitNickname[0]));
            }
            i++;
            while (tabSplit[i])
            {
                if (argUser == 1)
                    clientPtr->setUserName(tabSplit[i]);
                if (argUser == 2)
                    clientPtr->setMode(tabSplit[i]);
                if (argUser == 3)
                    clientPtr->setHostName(tabSplit[i]);
                if (argUser == 4)
                    clientPtr->setRealName(tabSplit[i]);
                i++;
                argUser++;
            }
        }
        if (tabSplit[i])
            i++; 
    }
}

void client::hello()
{
    std::cout	<< GREEN
	<< "client : " << this->getNickName()
    << "\t now active on "
	<< "socket fd: " << this->getsocketFd()
	<< "\t ip: " << this->getIp()
	<< "\t port: " << this->getPort()
	<< "\thostname: " << this->getPort()
	<< END << std::endl;
}

void client::goodBy()
{
    std::cout	<< GREEN
	<< "client : " << this->getNickName()
    << "\t is disconnected and no longer on  "
	<< "\t socket fd: " << this->getsocketFd()
	<< "\t ip: " << this->getIp()
	<< "\t port: " << this->getPort()
	<< "\thostname: " << this->getPort()
	<< END << std::endl;
}




