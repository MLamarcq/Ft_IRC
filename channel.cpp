#include "channel.hpp"

channel::channel() :
    name("default"),
    pswd("1234")
{
}

channel::channel(std::string channelName)
{
    (void)channelName;
}

channel::channel(const channel& copy)
{
    (void)copy;
}

channel & channel::operator=(const channel& copy)
{
    (void)copy;
    return (*this);
}

channel::~channel()
{

}

