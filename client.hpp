#ifndef CLIENT_HPP
#define CLIENT_HPP


#include "server.hpp"
class channel;
class client {

    public:
        client();
        client(std::string hostname, int fd);
        client(const client& copy);
        client& operator=(const client& copy);
        ~client();

        //getter
        std::string getNickName();
        std::string getRealName();
        std::string getUserName();
        std::string getHostName();
        std::string getMode();
        bool        getOperatorStatus();
        bool        isWelcomeMessageSent();
        int         getsocketFd();
        int         getPort();
        char        *getIp();
        int         getNumberOfChannelJoined();


        //setter
        void        setNickName(std::string str);
        void        setRealName(std::string str);
        void        setUserName(std::string str);
        void        setHostName(std::string str);
        void        setOperatorStatus(bool status);
        void        setWelcomeMessageSent(bool status);
        void        setsocketFd(int fd);
        void        setPort(int port);
        void        setIp(char *ip);
        void        setNumberOfChannelJoined(int nb);
        void        setMode(std::string mode);

        //utiles
        void fillStrParam(std::string str, client* clientPtr);
        void hello();
        void goodBy();
    protected:
        std::string nickName;
        std::string realName;
        std::string userName;
        std::string hostName;
        std::string mode;
        bool isOperator;
        bool welcomeMessageSent;
        int socketFd;
        int port;
        char *ip;
        int numberChannelJoined;
        std::list<channel *> listOfChannels;

};
#endif