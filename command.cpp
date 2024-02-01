/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlamarcq <mlamarcq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:33:59 by mlamarcq          #+#    #+#             */
/*   Updated: 2024/02/01 12:12:15 by mlamarcq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

// std::string		PASS();
// std::string		NICK();
// std::string		USER();
// std::string		PING();
// std::string		PONG();
// std::string		OPER();

command::command()
{

}
command::command(const command& copy)
{
    (void)copy;
}
command& command::operator=(const command& copy)
{
    (void) copy;
    return (*this);
}
command::~command()
{

}

std::string		command::QUIT(int fd, Server* serv)
{

	client * tempClient;
	close(fd);
	FD_CLR(fd, &(serv->M_struct->current_sockets));
	tempClient = serv->findClientBySocket(fd);
    //std::cout << "IN QUIT NICK IS " << tempClient->getNickName() << std::endl;
	if (tempClient != NULL)
		tempClient->goodBy();
	if (tempClient != NULL)
		serv->eraseClientFromList(tempClient->getNickName());

    // si le client appartient à un channel appeler la fonction
    // qui prévient tout le monde qu'il s'est barré
    return (" ");
}


std::string		command::USER(int fd, Server* serv)
{
    std::string temp;
    std::string arg;
    client* clientTmp;

    clientTmp = serv->findClientBySocket(fd);
    // dejà j'envoi pas l'erreur mauvais args
    // je vois pas le contenu de USER voir pk
    //std::cout << "c1\n";
    std::cout << "c2.4.1.1\n";
    if (serv->M_cmdMap.find("USER") != serv->M_cmdMap.end() || serv->M_cmdMap.find("userhost") != serv->M_cmdMap.end())
    {
        //std::cout << "c2\n";
        std::cout << "c2.4.1.2\n";
        if (serv->M_cmdMap.find("USER") != serv->M_cmdMap.end())
        {
            //std::cout << "c3\n";
            std::cout << "c2.4.1.3\n";
            temp = serv->M_cmdMap["USER"];
            char **tabSplit = ft_split(temp.c_str(), ' ');
            int count = std::count(temp.begin(), temp.end(), ' ');
            if (count < 3)
                return (ERR_NEEDMOREPARAMS(clientTmp->getNickName(), temp));
            std::cout << "c2.4.1.4\n";
            std::cout << "TABSPLIT[0] = " << tabSplit[0] << std::endl;
            if (serv->findClientByUserName(tabSplit[0]) != NULL)
                return (ERR_ALREADYREGISTRED(tabSplit[0]));
            std::cout << "c2.4.1.5\n";
            std::string name = tabSplit[0];
            name.append(clientTmp->getNickName());
            clientTmp->setUserName(name);
            clientTmp->setMode(tabSplit[1]);
            clientTmp->setHostName(tabSplit[2]);
            clientTmp->setRealName(tabSplit[3]);
            return ("nothing");
        }
        else
        {
            //std::cout << "c4\n";
            std::cout << "c2.4.1.6\n";
            temp = serv->M_cmdMap["userhost"];
            char **tabSplit = ft_split(temp.c_str(), ' ');
            int count = std::count(temp.begin(), temp.end(), ' ');
            if (count < 3)
                return (ERR_NEEDMOREPARAMS(clientTmp->getNickName(), temp));
            std::cout << "c2.4.1.7\n";
            if (serv->findClientByUserName(tabSplit[0]) != NULL)
                return (ERR_ALREADYREGISTRED(tabSplit[0]));
            std::cout << "c2.4.1.8\n";
            clientTmp->setUserName(tabSplit[0]);
            clientTmp->setMode(tabSplit[1]);
            clientTmp->setHostName(tabSplit[2]);
            clientTmp->setRealName(tabSplit[3]);
            return ("nothing");
        }
    }
    return ("BIZZARE");

}


std::string		command::NICK(int fd, Server* serv)
{

    //voit pk loic a pas été changé par toto
    std::string temp;
    std::string arg;
    client* clientTmp;
    temp = serv->M_cmdMap["NICK"];
    size_t notSpace = temp.find_first_not_of(' ');
    size_t end = temp.find(' ', notSpace);
    std::cout << "c2.1.1.1\n";
    if (end == std::string::npos)
    {
        std::cout << "c2.1.1.2\n";
        size_t lastcar = temp.find('\0');
        arg = temp.substr(notSpace, lastcar - 1);
        //std::cout << "ARG IS " << arg << std::endl;
        if (serv->findClientByNickName(arg) != NULL)
            return (ERR_NICKNAMEINUSE(arg, arg));
        std::cout << "c2.1.1.3\n";
        if (arg.empty())
            return (ERR_NONICKNAMEGIVEN(arg));
        std::cout << "c2.1.1.4\n";
        /*
        for (unsigned int i = 0; i < arg.length(); ++i)
		{
            if (!std::isalnum(arg[i]) && arg[i] != '-' && arg[i] != '_')
			    return(ERR_ERRONEUSNICKNAME(arg, arg));
        }
        */
        clientTmp = serv->findClientBySocket(fd);
        clientTmp->setNickName(arg);
        return ("nothing");
    }
    else
    {
        arg = temp.substr(notSpace, end);
        if (serv->findClientByNickName(arg) == NULL)
            return (ERR_NICKNAMEINUSE(arg, arg));
        if (arg.empty())
            return (ERR_NONICKNAMEGIVEN(arg));
        /*
        for (unsigned int i = 0; i < arg.length(); ++i)
		{
            if (!std::isalnum(arg[i]) && arg[i] != '-' && arg[i] != '_')
			    return(ERR_ERRONEUSNICKNAME(arg, arg));
        }
        */
        clientTmp = serv->findClientBySocket(fd);
        clientTmp->setNickName(arg);
        
        return ("nothing");
    }

}

std::string		command::PASS(int fd, Server* serv)
{
    std::string temp;
    std::string arg;
    client* clientTmp;
    temp = serv->M_cmdMap["PASS"];
    clientTmp = serv->findClientBySocket(fd);
    if (clientTmp->isWelcomeMessageSent())
        return (ERR_ALREADYREGISTRED(clientTmp->getNickName()));
    if (temp.empty())
        return (ERR_NEEDMOREPARAMS(clientTmp->getNickName(), "PASS"));
    if (temp.find(serv->M_pass_wd) == std::string::npos)
        return (ERR_PASSWDMISMATCH(clientTmp->getNickName()));
    return ("nothing");
}
// std::string		JOIN();
// std::string		PART();
// std::string		TOPIC();
// std::string		KICK();
// std::string		MODE();
// std::string		PRIVMSG();
// std::string		NOTICE();
// std::string		KILL();
// std::string		WALLOPS();
// std::string		bot();
