/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imoumini <imoumini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:33:59 by mlamarcq          #+#    #+#             */
/*   Updated: 2024/02/08 17:35:17 by imoumini         ###   ########.fr       */
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

std::string		command::PING(int fd, Server *serv)
{
	(void)serv;
	(void)fd;
	client * tempClient;
	tempClient = serv->findClientBySocket(fd);

	//std::string message = ":localhost ";
	//message.append("PONG ");
	//message.append("localhost ");
	//message.append(":localhost\r\n");
	return (PONG(tempClient->getNickName()));
}

std::string		command::QUIT(int fd, Server* serv)
{

	client * tempClient;
	close(fd);
	FD_CLR(fd, &(serv->M_struct->current_sockets));
	tempClient = serv->findClientBySocket(fd);
	//std::cout << "IN QUIT NICK IS " << tempClient->getNickName() << std::endl;
	std::string message = ":localhost 403 ";
	message.append(tempClient->getNickName().c_str());
	message.append(":leaving :No such channel\r\n");
	if (tempClient != NULL)
		tempClient->goodBy();
	if (tempClient != NULL)
		serv->eraseClientFromList(tempClient->getNickName());

	// si le client appartient à un channel appeler la fonction
	// qui prévient tout le monde qu'il s'est barré
	return (message);
}


std::string		command::USER(int fd, Server* serv)
{
	std::vector<std::string> temp;
	std::string arg;
	client* clientTmp;

	clientTmp = serv->findClientBySocket(fd);
	// dejà j'envoi pas l'erreur mauvais args
	// je vois pas le contenu de USER voir pk
	//std::cout << "c1\n";
	std::cout << "c2.4.1.1\n";
	// std::cout << 
	if (serv->M_cmdMap.find("USER") != serv->M_cmdMap.end() || serv->M_cmdMap.find("userhost") != serv->M_cmdMap.end())
	{
		//std::cout << "c2\n";
		std::cout << "c2.4.1.2\n";
		std::map<std::string, std::vector<std::string> > :: iterator it = serv->M_cmdMap.find("USER");
		if (it != serv->M_cmdMap.end())
		{
			//std::cout << "c3\n";
			std::cout << "c2.4.1.3\n";
			temp = parsTemp(serv->M_cmdMap["USER"]);
			// char **tabSplit = ft_split(temp.c_str(), ' ');
			// int count = std::count(temp.begin(), temp.end(), ' ');
			if (temp.size() < 4)
			{
				std::string chaine;
  				for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); ++it)
					chaine += *it;
				return (ERR_NEEDMOREPARAMS(clientTmp->getNickName(), chaine));
			}
			std::cout << "c2.4.1.4\n";
			//std::cout << "TABSPLIT[0] = " << tabSplit[0] << std::endl;

		    if (serv->findClientByUserName(temp[0]) != NULL)
			{
				std::cout << "USER ALREADY EXIST" << std::endl;
				return (ERR_ALREADYREGISTRED(temp[0]));
			}
			std::cout << "c2.4.1.5\n";
			clientTmp->setUserName(temp[0].append(clientTmp->getNickName()));
			std::cout << "c2.4.1.6\n";
			clientTmp->setMode(temp[1]);
			std::cout << "c2.4.1.7\n";
			clientTmp->setHostName(temp[2]);
			std::cout << "c2.4.1.8\n";
			clientTmp->setRealName(temp[3]);
			

			 std::cout << "c2.4.1.9\n";
			//    if (serv->findClientByUserName(tabSplit[0]) != NULL)
		   //        return (ERR_ALREADYREGISTRED(tabSplit[0]));
			// std::string name = tabSplit[0];
			// name.append(clientTmp->getNickName());
			// clientTmp->setUserName(name);
			// clientTmp->setMode(tabSplit[1]);
			// clientTmp->setHostName(tabSplit[2]);
			// clientTmp->setRealName(tabSplit[3]);
			return ("nothing");
		}
		else
		{
			//std::cout << "c4\n";
			std::cout << "c2.4.1.6\n";
			temp = parsTemp(serv->M_cmdMap["userhost"]);
			// char **tabSplit = ft_split(temp.c_str(), ' ');
			// int count = std::count(temp.begin(), temp.end(), ' ');
			if (temp.size() < 4)
			{
				std::string chaine;
				for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); ++it)
					chaine += *it;
				return (ERR_NEEDMOREPARAMS(clientTmp->getNickName(), chaine));
			}
			for (size_t i = 0; i < temp.size(); i++)
				std::cout << "temp[" << i << "] = " << temp[i] << std::endl;
			std::cout << "c2.4.1.7\n";
			if (serv->findClientByUserName(temp[0].append(clientTmp->getNickName()) ) != NULL)
			{
				std::cout << "USER ALREADY EXIST" << std::endl;
				return (ERR_ALREADYREGISTRED(temp[0]));
			}
			std::cout << "c2.4.1.8\n";
			clientTmp->setUserName(temp[0].append(clientTmp->getNickName()));
			clientTmp->setMode(temp[1]);
			clientTmp->setHostName(temp[2]);
			clientTmp->setRealName(temp[3]);
			std::cout << "c2.4.1.9\n";
			return ("nothing");
		}
	}
	return ("BIZZARE");

}


// std::string		command::NICK(int fd, Server* serv)
// {

// 	//voit pk loic a pas été changé par toto
// 	std::string temp;
// 	std::string arg;
// 	client* clientTmp;
// 	temp = serv->M_cmdMap["NICK"];
// 	size_t notSpace = temp.find_first_not_of(' ');
// 	size_t end = temp.find(' ', notSpace);
// 	std::cout << "c2.1.1.1\n";
// 	if (end == std::string::npos)
// 	{
// 		std::cout << "c2.1.1.2\n";
// 		size_t lastcar = temp.find('\0');
// 		arg = temp.substr(notSpace, lastcar - 1);
// 		//std::cout << "ARG IS " << arg << std::endl;
// 		if (serv->findClientByNickName(arg) != NULL)
// 			return (ERR_NICKNAMEINUSE(arg, arg));
// 		std::cout << "c2.1.1.3\n";
// 		if (arg.empty())
// 			return (ERR_NONICKNAMEGIVEN(arg));
// 		std::cout << "c2.1.1.4\n";
// 		/*
// 		for (unsigned int i = 0; i < arg.length(); ++i)
// 		{
// 			if (!std::isalnum(arg[i]) && arg[i] != '-' && arg[i] != '_')
// 				return(ERR_ERRONEUSNICKNAME(arg, arg));
// 		}
// 		*/
// 		clientTmp = serv->findClientBySocket(fd);
// 		clientTmp->setNickName(arg);
// 		return ("nothing");
// 	}
// 	else
// 	{
// 		arg = temp.substr(notSpace, end);
// 		if (serv->findClientByNickName(arg) == NULL)
// 			return (ERR_NICKNAMEINUSE(arg, arg));
// 		if (arg.empty())
// 			return (ERR_NONICKNAMEGIVEN(arg));
// 		/*
// 		for (unsigned int i = 0; i < arg.length(); ++i)
// 		{
// 			if (!std::isalnum(arg[i]) && arg[i] != '-' && arg[i] != '_')
// 				return(ERR_ERRONEUSNICKNAME(arg, arg));
// 		}
// 		*/
// 		clientTmp = serv->findClientBySocket(fd);
// 		clientTmp->setNickName(arg);
		
// 		return ("nothing");
// 	}

// }

std::string		command::NICK(int fd, Server* serv)
{
	//voit pk loic a pas été changé par toto

	//std::string temp;
	std::vector<std::string> temp;
	std::string arg;
	client* clientTmp;
	temp = parsTemp(serv->M_cmdMap["NICK"]);
	//size_t notSpace = temp.find_first_not_of(' ');
	//size_t end = temp.find(' ', notSpace);
	std::cout << "c2.1.1.1\n";
	// 
	//if (end == std::string::npos)
	//{
		std::cout << "c2.1.1.2\n";
		//size_t lastcar = temp.find('\0');
		//arg = temp.substr(notSpace, lastcar - 1);
		//std::cout << "ARG IS " << arg << std::endl;
		arg =  temp[0];
		if (arg.empty())
		{
			std::cout << "NICK EMPTY" << std::endl;
			return (ERR_NONICKNAMEGIVEN(arg));
		}
		std::cout << "c2.1.1.3\n";
		if (serv->findClientByNickName(arg) != NULL)
		{
			std::cout << "NICK ALREADY IN USE" << std::endl;
			return (ERR_NICKNAMEINUSE(arg, arg));
		}
		
		
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
	//}
	
	//else
	//{
	//	arg = temp.substr(notSpace, end);
	//	if (serv->findClientByNickName(arg) == NULL)
	//		return (ERR_NICKNAMEINUSE(arg, arg));
	//	if (arg.empty())
	//		return (ERR_NONICKNAMEGIVEN(arg));
		/*
		for (unsigned int i = 0; i < arg.length(); ++i)
		{
			if (!std::isalnum(arg[i]) && arg[i] != '-' && arg[i] != '_')
				return(ERR_ERRONEUSNICKNAME(arg, arg));
		}
		*/
	//	clientTmp = serv->findClientBySocket(fd);
	//	clientTmp->setNickName(arg)	
	//	return ("nothing");
	//}
}


std::string		command::PASS(int fd, Server* serv)
{
	std::vector<std::string >temp;
	std::string arg;
	client* clientTmp;
	temp = serv->M_cmdMap["PASS"];
	clientTmp = serv->findClientBySocket(fd);
	if (clientTmp->isWelcomeMessageSent())
	{
		std::cout << "PASS CLIENT ALREADY REGISTERED NO NEED PASS" << std::endl;
		return (ERR_ALREADYREGISTRED(clientTmp->getNickName()));
	}
	if (temp[0].empty())
	{
		std::cout << "PASS EMPTY" << std::endl;
		return (ERR_NEEDMOREPARAMS(clientTmp->getNickName(), "PASS"));
	}
	std::string clientPass = temp[0];
	clientPass = clientPass.substr(0, clientPass.length() - 1);
	if (clientPass != serv->M_pass_wd)
	{
		std::cout << "PASS WRONG PASS" << std::endl;
		return (ERR_PASSWDMISMATCH(clientTmp->getNickName()));
	}
	return ("nothing");
}

std::vector<std::string>	command::parsTemp(std::vector<std::string> temp)
{
	std::vector<std::string>::iterator it = temp.begin();
	std::vector<std::string>::iterator ite = temp.end();
	size_t i = 0;
	while (it != ite)
	{
		if ((*it).find('\r') != std::string::npos)
		{
			i = (*it).find('\r');
			(*it) = (*it).substr(0, i);
		}
		it++;
	}
	// size_t size = 0;
	// while (size < temp.size())
	// {
	// 	std::cout << "temp[" << size << "] = " << temp[size] << std::endl;
	// 	size++;
	// }
	return (temp);
}


int		command::JOIN(client *client1, Server *serv)
{
	std::vector<std::string> temp;
	std::string chan, message;
	bool first_toggle = false;
	temp = parsTemp(serv->M_cmdMap["JOIN"]);
	int check = 0;
	// if (temp.empty())
	// {
	// 	std::cout << "To join a channel, please enter it's name" << std::endl;
	// 	return (" ");
	// }
	// if (client1->getBan() == true)
	// 	return ();
	// for (size_t li = 0; li < temp.size(); li++)
	// 	std::cout << "temp[" << li << "] = " << temp[li] << std::endl;
	// std::cout << "temp.size() = " << temp.size() << std::endl;
	// if (temp.size() > 2)
	// {
	// 	std::string chaine;
	// 	for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); ++it)
	// 		chaine += *it;
	// 	return (ERR_NEEDMOREPARAMS(client1->getNickName(), chaine));
	// }
	if (serv->checkChannel() == false)
	{
		std::cout << "No channel in the list, it will be the first !" << std::endl;
		channel *new_one = new channel;
		//faire une fonction dans client pour delete, utiliser algo for each : delete (it);
		//ou utiliser QUIT
		check = new_one->setChannelFirstTime(client1, serv, new_one, temp);
		first_toggle = true;
		// std::cout << "Channel name = " << new_one->getName() << std::endl; //"d'une taille de : " << new_one->getName().size() << std::endl;
		// std::cout << "Channel password = " << new_one->getPassword() << std::endl;
		//return (0);
	}
	if (first_toggle == false)
	{
		// // std::cout << "On passe la" << std::endl;
		std::list<channel *> listOfChannels = serv->getListOfChannels();
		std::list<channel *>::iterator it = listOfChannels.begin();
		std::list<channel *>::iterator ite = listOfChannels.end();
		bool found = false;
		while (it != ite)
		{
				// std::cout << "Parameter = " << parameter << std::endl;
				// std::cout << "Channel name = " << (*it)->getName() << std::endl; //" d'une taille de : " << (*it)->getName().size() << std::endl;
				// std::cout << "temp[0] = " << temp[0] << std::endl; //" d'une taille de : " << temp.size() << std::endl;
				// std::cout << "res du compare : " << (*it)->getName().compare(temp[0]) << std::endl;
				if ((*it)->getName().compare(temp[0]) == 0)
				{
					std::cout << "Channel found !" << std::endl;
					chan = temp[0];
					found = true;
					break ;
				}
				it++;
			}
		if (found == false)
		{
			channel *new_one = new channel;
			//faire une fonction dans client pour delete, utiliser algo for each : delete (it);
			//ou utiliser QUIT
			// if (new_one->setName(temp[0]) == 0)
			// {
			// 	std::string message = ERR_NOSUCHCHANNEL(client1->getNickName(), new_one->getName());
			// 	std::cout << "le message = " << message << std::endl;
			// 	std::cout << "ERROR CHANNEL" << std::endl;
			// 	std::cout << "La socket du client = " << client1->getsocketFd() << std::endl;
			// 	return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
			// }
			// if (temp.size() == 2)
			// 	new_one->setPassword(temp[1]);
			// new_one->setListofClient(client1);
			// new_one->setOperators(client1);
			// serv->setNewChannel(new_one);
			// new_one->increaseNbrCLient();
			check = new_one->setChannelFirstTime(client1, serv, new_one, temp);
			// std::cout << "Channel name = " << new_one->getName() << std::endl;
			// std::cout << "Channel password = " << new_one->getPassword() << std::endl;
			//return (0);
		}
		else
			check = serv->addClientToChannel(client1, temp);
	}
	switch (check)
	{
		case 0 :
		{
			std::cout << "Welcome to the channel !" << std::endl;
			break ;
		}
		case 403 :
		{
			chan = serv->getChanName();
			std::cout << "NOM DU CHANNEL 403 = " << chan << std::endl;
			std::cout << "client1->getNickName(): " << client1->getNickName() << std::endl;
			std::cout << "client1->getsocketFd(): " << client1->getsocketFd() << std::endl;
			std::string message = ERR_NOSUCHCHANNEL(client1->getNickName(), chan);
			int rtn = (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
			std::cout << rtn << std::endl;
			return (rtn);
			//break ;
		}
		case 471 :
		{
			chan = serv->getChanName();
			std::cout << "NOM DU CHANNEL 471 = " << chan << std::endl;
			std::cout << "client1->getNickName(): " << client1->getNickName() << std::endl;
			std::cout << "client1->getsocketFd(): " << client1->getsocketFd() << std::endl;
			std::string message = ERR_CHANNELISFULL(client1->getNickName(), chan);
			int rtn = (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
			std::cout << rtn << std::endl;
			return (rtn);
			//break ;
		}
		case 475 :
		{
			chan = serv->getChanName();
			std::cout << "NOM DU CHANNEL 475 = " << chan << std::endl;
			std::cout << "client1->getNickName(): " << client1->getNickName() << std::endl;
			std::cout << "client1->getsocketFd(): " << client1->getsocketFd() << std::endl;
			std::string message = ERR_BADCHANNELKEY(client1->getNickName(), chan);
			int rtn = (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
			std::cout << rtn << std::endl;
			return (rtn);
			//break ;
		}
		default :
		{
			std::cout << "ERROR" << std::endl;
			break ;
		}
	}
	// // std::cout << "Parameter in JOIN = " << parameter << std::endl;
	// // std::cout << "Client1 nickname in JOIN : " << client1->getNickName() << std::endl;
	// // std::cout << "Client1 FD in JOIN : " << client1->getsocketFd() << std::endl;
	(void)client1;
	(void)serv;
	// (void) found;
	return (0);
}

int		command::MODE(client *client1, Server *serv)
{
	std::vector<std::string> temp;
	temp = parsTemp(serv->M_cmdMap["MODE"]);
	bool toggle = false;
	std::string recup, message;
	int check = 0;
	size_t i = 0;
	
	while (i < temp.size())
	{
		std::cout << "temp[" << i << "] = " << temp[i] << std::endl;
		i++;
	}
	std::list<channel *> listOfChannels = serv->getListOfChannels();
	std::list<channel *>::iterator it = listOfChannels.begin();
	std::list<channel *>::iterator ite = listOfChannels.end();
	std::cout << "ICI?" << std::endl;
	if (*it)
		std::cout << "it = " << (*it)->getName() << std::endl;
	while (it != ite)
	{
		if ((*it)->getName().compare(temp[0]) == 0)
		{
			std::cout << "Channel found !" << std::endl;
			toggle = true;
			break ;
		}
		it++;
	}
	if (*it && toggle == true)
	{
		bool tunnel = true;
		//virifier avant si il existe un - ou un + sinon le message est mal interprete
		// channel *chan = (*it);
		if (temp[1].size() > 2)
		{
			tunnel = false;
			check = 461;
			// std::cout << "WRONG FIRST ARG" << std::endl;
			// message = ERR_NEEDMOREPARAMS(client1->getNickName(), (*it)->getName());
			// return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		}
		// std::cout << "chan name = " << chan->getName() << std::endl;
		// std::cout << "chan password = " << chan->getPassword() << std::endl;
		std::map<client *, bool> map2 = (*it)->getListOfClients();
		std::map<client *, bool>::iterator m_it = map2.begin();
		std::map<client *, bool>::iterator m_ite = map2.end();
		while (m_it != m_ite && tunnel == true)
		{
			if (m_it->first->getsocketFd() == client1->getsocketFd())
			{
				if (m_it->second == false)
				{
					check = 482;
					break ;
				}
				else
				{
					std::cout << "Client has privileges !" << std::endl;
				}
			}
			m_it++;
		}
		if (temp[1].size() > 2)
			std::cout << "WRONG FIRST ARG" << std::endl;
		else
			std::cout << "GOOD FIRST ARG" << std::endl;
		// while (m_it != m_ite)
		// {
		// 	std::cout << "La map : [" << m_it->first->getNickName() << "] = " << m_it->second << std::endl;
		// 	std::cout << "Autre info client : fd = " << m_it->first->getsocketFd() << std::endl;
		// 	m_it++;
		// }
		// std::cout << "On sort de la boucle" << std::endl;
		// std::map<client *, bool>::iterator d_it = map2.begin();
		// std::map<client *, bool>::iterator d_ite = map2.end();
		// while (d_it != d_ite)
		// {
		// 	delete d_it->first;
		// 	d_it++;
		// }
		// chan->getListOfClients().clear();
		// std::cout << "La map se clear bien" << std::endl;
	}
	else
		check = 100;
	switch (check)
	{
		case 0 :
		{
			std::cout << "Welcome to the channel !" << std::endl;
			break ;
		}
		case 461 :
		{
			message = ERR_NEEDMOREPARAMS(client1->getNickName(), (*it)->getName());
			return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		}
		case 482 :
		{
			std::cout << "NOM DU CHANNEL 482 = " << (*it)->getName() << std::endl;
			std::cout << "client1->getNickName(): " << client1->getNickName() << std::endl;
			std::cout << "client1->getsocketFd(): " << client1->getsocketFd() << std::endl;
			message = ERR_CHANOPRIVSNEEDED(temp[0]);
			std::cout << "Message = " << message << std::endl;
			int rtn = (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
			std::cout << rtn << std::endl;
			return (rtn);
			//break ;
		}
		default :
		{
			std::cout << "ERROR" << std::endl;
			break ;
		}
	}
	(void)client1;
	return (0);
}


// int	command::handleCmd(client *client1, Server *serv, std::string cmd)
// {
// 	int check = -1;
// 	std::string chan, message;
// 	if (cmd.compare("JOIN") == 0)
// 		check = this->JOIN(client1, serv);
// 	switch (check)
// 	{
// 		case 0 :
// 		{
// 			std::cout << "Welcome to the channel !" << std::endl;
// 			break ;
// 		}
// 		case 403 :
// 		{
// 			chan = serv->getChanName();
// 			std::cout << "ERROR 403" << std::endl;
// 			message = ERR_NOSUCHCHANNEL(client1->getNickName(), chan);
// 			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
// 			break ;
// 		}
// 		case 471 :
// 		{
// 			chan = serv->getChanName();
// 			message = ERR_CHANNELISFULL(client1->getNickName(), chan);
// 			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
// 			//break ;
// 		}
// 		case 475 :
// 		{
// 			chan = serv->getChanName();
// 			std::cout << "ERROR 475" << std::endl;
// 			message = ERR_BADCHANNELKEY(client1->getNickName(), chan);
// 			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
// 			//break ;
// 		}
// 		default :
// 		{
// 			std::cout << "ERROR" << std::endl;
// 			break ;
// 		}
// 	}
// 	return (check);
// }

// std::string		JOIN();
// std::string		PART();
// std::string		TOPIC();
// std::string		KICK();

// std::string		PRIVMSG();
// std::string		NOTICE();
// std::string		KILL();
// std::string		WALLOPS();
// std::string		bot();
