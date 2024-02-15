/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlamarcq <mlamarcq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:33:59 by mlamarcq          #+#    #+#             */
/*   Updated: 2024/02/15 12:37:08 by mlamarcq         ###   ########.fr       */
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
	
	client * tempClient;
	tempClient = serv->findClientBySocket(fd);

	//std::string message = ":localhost ";
	//message.append("PONG ");
	//message.append("localhost ");
	//message.append(":localhost\r\n");
	std::string msg;
	if (tempClient)
		msg = tempClient->getNickName();
	else
		msg = "";
	
	return (PONG(msg));
}

std::string		command::QUIT(int fd, Server* serv)
{

	client * tempClient;
	close(fd);
	FD_CLR(fd, &(serv->M_struct->current_sockets));
	tempClient = serv->findClientBySocket(fd);
	//std::cout << "IN QUIT NICK IS " << tempClient->getNickName() << std::endl;
	std::string message = ":localhost 403 ";
	if (tempClient)
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
				std::string msg;
				if (clientTmp)
					msg = clientTmp->getNickName();
				else
					msg = "";
				return (ERR_NEEDMOREPARAMS(msg, chaine));
			}
			std::cout << "c2.4.1.4\n";
			//std::cout << "TABSPLIT[0] = " << tabSplit[0] << std::endl;
			if (clientTmp)
		    {
				if (serv->findClientByUserName(temp[0].append(clientTmp->getNickName())) != NULL)
				{
					std::cout << "USER ALREADY EXIST" << std::endl;
					return (ERR_ALREADYREGISTRED(temp[0]));
				}
			}
			if (clientTmp)
			{
				std::cout << "c2.4.1.5\n";
				clientTmp->setUserName(temp[0].append(clientTmp->getNickName()));
				std::cout << "c2.4.1.6\n";
				clientTmp->setMode(temp[1]);
				std::cout << "c2.4.1.7\n";
				clientTmp->setHostName(temp[2]);
				std::cout << "c2.4.1.8\n";
				clientTmp->setRealName(temp[3]);
			}
			

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
		if (clientTmp)
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
	std::map<std::string, std::vector<std::string > >::iterator it = serv->M_cmdMap.find("PASS");

    // Check if the key was found
    if (it != serv->M_cmdMap.end()) {
        temp = parsTemp(serv->M_cmdMap["PASS"]);
    } else {
        temp = parsTemp(serv->M_cmdMap["SSPA"]);
    }
	std::cout << "c20\n";
	
	//temp = serv->M_cmdMap["PASS"];	
	std::cout << "c21\n";
	clientTmp = serv->findClientBySocket(fd);
	std::cout << "c22\n";
	if (clientTmp && clientTmp->isWelcomeMessageSent())
	{
		std::cout << "PASS CLIENT ALREADY REGISTERED NO NEED PASS" << std::endl;
		return (ERR_ALREADYREGISTRED(clientTmp->getNickName()));
	}
	std::cout << "c23\n";
	
	if (temp[0].empty())
	{
		std::cout << "PASS EMPTY" << std::endl;
		return (ERR_NEEDMOREPARAMS(clientTmp->getNickName(), "PASS"));
	}
	std::cout << "c24\n";
	std::string clientPass = temp[0];
	//clientPass = clientPass.substr(0, clientPass.length() - 1);
	clientPass = temp[0];
	std::cout << "c25\n";
	if (clientPass != serv->M_pass_wd)
	{
		std::cout << "PASS WRONG PASS" << std::endl;
		return (ERR_PASSWDMISMATCH(clientTmp->getNickName()));
	}
	std::cout << "c26\n";
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
	if (serv->checkChannel() == false)
	{
		std::cout << "No channel in the list, it will be the first !" << std::endl;
		channel *new_one = new channel;
		//faire une fonction dans client pour delete, utiliser algo for each : delete (it);
		//ou utiliser QUIT
		check = new_one->setChannelFirstTime(client1, serv, temp);
		new_one->welcomeInChanMessage(client1);
		first_toggle = true;
	}
	if (first_toggle == false)
	{
		std::list<channel *> listOfChannels = serv->getListOfChannels();
		std::list<channel *>::iterator it = listOfChannels.begin();
		std::list<channel *>::iterator ite = listOfChannels.end();
		bool found = false;
		while (it != ite)
		{
				if ((*it)->getName().compare(temp[0]) == 0)
				{
					std::cout << "Channel found !" << std::endl;
					if ((*it)->getIsInvite() == true)
					{
						bool isWaiting = false;
						std::list<client *> cl_list = (*it)->getWaitingList();
						std::list<client *>::iterator cl_it = cl_list.begin();
						std::list<client *>::iterator cl_ite = cl_list.end();
						std::list<client *>::iterator cl_find;
						cl_find = std::find(cl_it, cl_ite, client1);
						if (cl_find != cl_list.end())
						{
							std::cout << "TROUVEEEE" << std::endl;
						// while (cl_it != cl_ite)
						// {
						// 	std::cout << "IN THE WAITING LIST :" << (*cl_it)->getNickName() << std::endl;
							// if ((*cl_it)->getsocketFd() == client1->getsocketFd())
							// {
							(*it)->getWaitingList().erase(cl_it);
							isWaiting = true;
							chan = temp[0];
							// }
							// cl_it++;
						}
						if (isWaiting == false)
						{
							serv->setChanName(temp[0]);
							check = 6;
						}
					}
					else
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
			check = new_one->setChannelFirstTime(client1, serv, temp);
			new_one->welcomeInChanMessage(client1);
		}
		else if (found == true && check == 0)
			check = serv->addClientToChannel(client1, temp);
	}
	(void)client1;
	(void)serv;
	return (check);
}


int		command::MODE(client *client1, Server *serv)
{
	std::vector<std::string> temp;
	temp = parsTemp(serv->M_cmdMap["MODE"]);
	bool toggle = false;
	std::string recup, message;
	int check = 0;
	size_t i = 0;
	if (temp.size() <= 1)
		return (0);
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
		bool ope = true;
		std::cout << "DEDANS" << std::endl;
		
		//virifier avant si il existe un - ou un + sinon le message est mal interprete
		
		// channel *chan = (*it);
		
		//Ici on verifiait la taille de l'argument, mais, si l'on se refere a dalNet, pas necessaire
		
		// if (temp[1].size() > 2)
		// {
		// 	ope = false;
		// 	check = 472;
		// 	serv->setChanName(temp[0]);
		// 	// std::cout << "CharERR = " << this->M_charErr << std::endl;
		// 	std::cout << "temp[1] = " << temp[1] << std::endl;
		// 	std::string err = temp[1];
		// 	serv->setCharErr(err);
		// 	// std::cout << "WRONG FIRST ARG" << std::endl;
		// 	// message = ERR_NEEDMOREPARAMS(client1->getNickName(), (*it)->getName());
		// 	// return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		// }
		// std::cout << "chan name = " << chan->getName() << std::endl;
		// std::cout << "chan password = " << chan->getPassword() << std::endl;
		
		std::map<client *, bool> map2 = (*it)->getListOfClients();
		std::map<client *, bool>::iterator m_it = map2.begin();
		std::map<client *, bool>::iterator m_ite = map2.end();
		while (m_it != m_ite)
		{
			if (m_it->first->getsocketFd() == client1->getsocketFd())
			{
				if (m_it->second == false)
				{
					check = 482;
					serv->setChanName(temp[0]);
					ope = false;
					break ;
				}
				else
				{
					std::cout << "Client has privileges !" << std::endl;
				}
			}
			m_it++;
		}
		if (ope == true)
		{
			int	arg = whatArg(temp);
			int sign = whatSign(temp);
			std::cout << "ARG = " << arg << std::endl;
			serv->setChanName(temp[0]);
			switch (arg)
			{
				case 'i' :
				{
					bool checkIsInvite;
					std::cout << "C'est un i !" << std::endl;
					if (sign == '-')
					{
						checkIsInvite = (*it)->getIsInvite();
						if (checkIsInvite == true)
						{
							(*it)->changeIsInviteOff(client1->getNickName(), client1->getUserName(), "-i");
							// check = 4;
						}
						break ;
					}
					else
					{
						checkIsInvite = (*it)->getIsInvite();
						if (checkIsInvite == false)
						{
							(*it)->changeIsInviteOn(client1->getNickName(), client1->getUserName(), "+i");
							// check = 5;
						}
						break ;
					}
					//break ;
				}
				case 't' :
				{
					std::cout << "C'est un t !" << std::endl;
					if (sign == '+')
					{
						(*it)->setTopicEstate(client1->getNickName(), client1->getUserName(), "+t", 1);
					}
					if (sign == '-')
					{
						(*it)->setTopicEstate(client1->getNickName(), client1->getUserName(), "-t", 2);
					}
					break ;
				}
				case 'k' :
				{
					std::cout << "C'est un k !" << std::endl;
					
					if (sign == '+')
					{
						if (temp.size() > 2)
						{
							if ((*it)->getPassword().empty() && temp.size() > 2)
							{
								(*it)->setNewPassWord(temp[2], client1->getNickName(), client1->getUserName(), "+k");
							}
						}
						else
						{
							char new_char = arg;
							message = MODE_NEEDMOREPARAMS(client1->getNickName(), client1->getUserName(), (*it)->getName(), new_char);
							send(client1->getsocketFd(), message.c_str(), message.length(), 0);
						}
					}
					if (sign == '-')
					{
						if ((*it)->getPassword().empty() == 0)
						{
							(*it)->clearPassWord(client1->getNickName(), client1->getUserName(), "-k");
						}
					}
					break ;
				}
				case 'o' :
				{
					std::cout << "C'est un o !" << std::endl;
					if (temp.size() > 2)
					{
						if (sign == '+')
							(*it)->changePrivileges(client1->getNickName(), client1->getUserName(), "+o", temp[2], 1);
						if (sign == '-')
							(*it)->changePrivileges(client1->getNickName(), client1->getUserName(), "-o", temp[2], 2);
					}
					else
					{
						char new_char = arg;
						message = MODE_NEEDMOREPARAMS(client1->getNickName(), client1->getUserName(), (*it)->getName(), new_char);
						send(client1->getsocketFd(), message.c_str(), message.length(), 0);
					}
					break ;
				}
				case 'l' :
				{
					std::cout << "C'est un l !" << std::endl;
					
					if (sign == '+')
					{
						if (temp.size() > 2)
						{
							if ((*it)->getClientLimit() == 0 && temp.size() > 2)
							{
								(*it)->setClientLimit(client1->getNickName(), client1->getUserName(), "+l", temp[2]);
							}
						}
						else
						{
							char new_char = arg;
							message = MODE_NEEDMOREPARAMS(client1->getNickName(), client1->getUserName(), (*it)->getName(), new_char);
							send(client1->getsocketFd(), message.c_str(), message.length(), 0);
						}
					}
					if (sign == '-')
					{
						if ((*it)->getClientLimit() != 0)
						{
							(*it)->eraseClientLimit(client1->getNickName(), client1->getUserName(), "-l");
						}
					}
					break ;
				}
				default :
				{
					char new_char = arg;
					message = MODE_UNKNOW_MODE(client1->getNickName(), client1->getUserName(), (*it)->getName(), new_char);
					(*it)->sendToAllChan(message);
					break ;
				}
			}
		}
		// if (temp[1].size() > 2)
		// 	std::cout << "WRONG FIRST ARG" << std::endl;
		// else
		// 	std::cout << "GOOD FIRST ARG" << std::endl;
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
	(void)client1;
	return (check);
}

int		command::TOPIC(client *client1, Server *serv)
{
	std::vector<std::string> temp;
	bool toggle = false;
	temp = parsTemp(serv->M_cmdMap["TOPIC"]);
	size_t i = 0;
	int check = 0;
	std::string message, s_topic;
	std::list<channel *> list = serv->getListOfChannels();
	std::list<channel *>::iterator it = list.begin();
	std::list<channel *>::iterator ite = list.end();
	
	if (temp.size() == 0)
	{
		std::cout << "NO ARG" << std::endl;
		return (0);
	}
	while (i < temp.size())
	{
		std::cout << "temp[" << i << "] = " << temp[i] << std::endl;
		i++;
	}
	while (it != ite)
	{
		std::cout << "ON PASSE PAR LA" << std::endl;
		if ((*it)->getName().compare(temp[0]) == 0)
		{
			toggle = true;
			break ;
		}
		it++;
	}
	if (*it && toggle == true)
	{
		std::cout << "ON RENTRE DANS TOPIC" << std::endl;
		if (temp.size() < 2)
		{
			std::cout << "On rentre ici !" << std::endl;
			return (0);
		}
		else
		{
			std::cout << "ON RENTRE DANS TOPIC" << std::endl;
			std::map<client *, bool> tmp = (*it)->getListOfClients();
			std::map<client *, bool>::iterator m_it = tmp.begin();
			std::map<client *, bool>::iterator m_ite = tmp.end();
			while (m_it != m_ite)
			{
				if (m_it->first->getsocketFd() == client1->getsocketFd())
				{
					if ((*it)->getTopicEstate() == false && m_it->second == false)
					{
						check = 482;
						serv->setChanName((*it)->getName());
						break ;
					}
					i = 1;
					while (i < temp.size())
					{
						s_topic = s_topic.append(temp[i]) + ' ';
						i++;
					}
					std::cout << "MESSAGE ENVOYE" << std::endl;
					(*it)->setTopic(s_topic);
					message = RPL_TOPIC(client1->getNickName(), (*it)->getName(), (*it)->getTopic());
					(*it)->sendToAllChan(message);
					break ;
				}
				m_it++;
			}
		}
		std::cout << "THE TOPIC IS " << (*it)->getTopic() << std::endl;
	}
	(void)toggle;
	(void)client1;
	return (check);
}


int	command::INVITE(client *client1, Server *serv)
{
	std::string message, chan;
	std::vector<std::string> temp;
	bool toggle = false;
	bool chanExist = false;
	size_t i = 0;
	int check = 0;
	temp = parsTemp(serv->M_cmdMap["INVITE"]);
	std::list<channel *> chanList = serv->getListOfChannels();
	std::list<channel *>::iterator it = chanList.begin();
	std::list<channel *>::iterator ite = chanList.end();
	
	if (temp.size() <= 1)
	{
		std::cout << "NO ARG" << std::endl;
		return (0);
	}
	while (i < temp.size())
	{
		std::cout << "temp[" << i << "] = " << temp[i] << std::endl;
		i++;
	}
	while (it != ite)
	{
		std::cout << "ON PASSE PAR LA" << std::endl;
		std::string diez = "#";
		std::cout << "STRING AVANT = " << temp[1] << std::endl;
		if (temp[1][0] != '#')
		{
			temp[1].insert(0, diez);
		}
		std::cout << "STRING APRES = " << temp[1] << std::endl;
		if ((*it)->getName().compare(temp[1]) == 0)
		{
			std::cout << "SUCCESS !" << std::endl;
			chanExist = true;
			std::map<client *, bool> clients = (*it)->getListOfClients();
			std::map<client *, bool>::iterator m_it = clients.begin();
			std::map<client *, bool>::iterator m_ite = clients.end();
			while (m_it != m_ite)
			{
				if (client1->getsocketFd() == m_it->first->getsocketFd())
				{
					toggle = true;
					break ;
				}
				m_it++;
			}
			if (toggle == false)
			{
				message = NOTONCHANNEL(client1->getNickName(), client1->getUserName(), (*it)->getName());
				std::cout << "NOTONCHANNEL = " << message << std::endl;
				send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			}
			else
			{
				if (m_it->second == false)
					check = 482;
				else
				{
					bool stop = false;
					bool exist = false;
					std::list<client *> servClientList = serv->getServClientList();
					std::list<client *>::iterator l_it = servClientList.begin();
					std::list<client *>::iterator l_ite = servClientList.end();
					for(; l_it != l_ite; l_it++)
					{
						if ((*l_it)->getNickName().compare(temp[0]) == 0)
						{
							exist = true;
							std::map<client *, bool>::iterator c_it = clients.begin();
							std::map<client *, bool>::iterator c_ite = clients.end();
							while (c_it != c_ite)
							{
								std::cout << "Client nickname in channel : " << c_it->first->getNickName() << std::endl;
								if (c_it->first->getNickName().compare(temp[0]) == 0)
								{
									message = ERR_USERONCHANNEL(client1->getNickName(), client1->getUserName(), (*it)->getName(), temp[0]);
									send(client1->getsocketFd(), message.c_str(), message.length(), 0);
									stop = true;
									break ;
								}
								c_it++;
							}
							if (stop == false)
							{
								(*it)->setWaitingList(*l_it);
								std::list<client *> cl_list = (*it)->getWaitingList();
								std::list<client *>::iterator cl_it = cl_list.begin();
								std::list<client *>::iterator cl_ite = cl_list.end();
								std::cout << "IN THE WAITING LIST : " << (*cl_it)->getNickName() << std::endl;
								while (cl_it != cl_ite)
								{
									std::cout << "IN THE WAITING LIST : " << (*cl_it)->getNickName() << std::endl;
									cl_it++;
								}
								message = INVITE_ON_CHAN(client1->getNickName(), client1->getUserName(), (*it)->getName(), temp[0]);
								send((*l_it)->getsocketFd(), message.c_str(), message.length(), 0);
								stop = true;
							}
						}
						if (stop == true)
							break ;
					}
					if (exist == false)
					{
						message = ERR_NOSUCHNICK(client1->getNickName(), temp[0]);
						std::cout << "NOSUCHNICK = " << message << std::endl;
						send(client1->getsocketFd(), message.c_str(), message.length(), 0);
					}
				}
			}
			break ;
		}
		it++;
	}
	if (chanExist == false)
	{
		std::cout << "NO SUCH CHANNEL" << std::endl;
		if (temp.size() > 1)
			chan = temp[1];
		message = NOSUCHCHANNEL(client1->getNickName(), client1->getUserName(), chan);
		send(client1->getsocketFd(), message.c_str(), message.length(), 0);
	}
	(void)toggle;
	(void)client1;
	return (check);
}


int		command::PRIVMSG(client *client1, Server *serv)
{
	std::vector<std::string> temp;
	std::string message, to_send;
	// temp = parsTemp(serv->M_cmdMap["PRIVMSG"]);
	std::map<std::string, std::vector<std::string > >::iterator m_found = serv->M_cmdMap.find("PRIVMSG");
	if (m_found != serv->M_cmdMap.end()) {
        temp = parsTemp(serv->M_cmdMap["PRIVMSG"]);
    } else {
        temp = parsTemp(serv->M_cmdMap["VMSGPRI"]);
    }
	int check = 0;
	size_t i = 1;
	size_t found = 0;
	bool toggle = false;
	if (temp.size() < 2)
	{
		message = NEEDMOREPARAMS(client1->getNickName(), client1->getUserName(), "PRIVMSG");
		send(client1->getsocketFd(), message.c_str(), message.length(), 0);
		return (0);
	}
	std::list<channel *> chanList = serv->getListOfChannels();
	std::list<channel *>::iterator it = chanList.begin();
	std::list<channel *>::iterator ite = chanList.end();
	while(it != ite)
	{
		if ((*it)->getName().compare(temp[0]) == 0)
		{
			toggle = true;
			break ;
		}
		it++;
	}
	if (*it && toggle == true)
	{
		while (i < temp.size())
		{
			std::cout << "temp[" << i << "] = " << temp[i] << std::endl;
			message.append(temp[i]);
			message.append(" ");
			i++;
		}
		std::cout << "MESSAGE 1 = " << message << std::endl;
		found = message.find(":");
		if (found != std::string::npos)
			message.erase(found, 1);
		std::cout << "MESSAGE 2 = " << message << std::endl;
		to_send = PRIVMSG_CHAN(client1->getNickName(), client1->getUserName(), (*it)->getName(), message);
		(*it)->sendPrivMsg(client1, to_send);
	}
	(void)client1;
	(void)serv;
	return (check);
}

int	command::PART(client *client1, Server *serv)
{
	bool toggle = false;
	std::string message;
	std::vector<std::string> temp;
	std::map<std::string, std::vector<std::string > >::iterator m_found = serv->M_cmdMap.find("PART");
	if (m_found != serv->M_cmdMap.end()) {
        temp = parsTemp(serv->M_cmdMap["PART"]);
    } else {
        temp = parsTemp(serv->M_cmdMap["RTPA"]);
    }
	if (temp.size() == 0)
		return (0);
	std::list<channel *> chanList = serv->getListOfChannels();
	std::list<channel *>::iterator it = chanList.begin();
	std::list<channel *>::iterator ite = chanList.end();
	while (it != ite)
	{
		if ((*it)->getName().compare(temp[0]) == 0)
		{
			toggle = true;
			break ;
		}
		it++;
	}
	if (*it && toggle == true)
	{
		bool inChan = false;
		std::map<client *, bool> clMap = (*it)->getListOfClients();
		std::map<client *, bool>::iterator c_it = clMap.begin();
		std::map<client *, bool>::iterator c_ite = clMap.end();
		while (c_it != c_ite)
		{
			if (c_it->first->getsocketFd() == client1->getsocketFd())
			{
				inChan = true;
				break ;
			}
			c_it++;
		}
		if (inChan == false)
		{
			message = NOTONCHANNEL(client1->getNickName(), client1->getUserName(), (*it)->getName());
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			return (0);
		}
		(*it)->getListOfClients().erase(c_it);
		if ((*it)->getListOfClients().empty())
		{
			serv->eraseChannelFromList((*it));
			if (temp.size() == 2)
				message = PART_CHAN(client1->getNickName(), client1->getUserName(), (*it)->getName(), temp[1]);
			else
				message = PART_CHAN(client1->getNickName(), client1->getUserName(), (*it)->getName(), "");
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			return (0);
		}

		std::cout << "SUCCESS IN PART !" << std::endl;
	}
	else
	{
		std::cout << "NO SUCH CHANNEL PART" << std::endl;
		message = NOSUCHCHANNEL(client1->getNickName(), client1->getUserName(), temp[0]);
		send(client1->getsocketFd(), message.c_str(), message.length(), 0);
	}
	return (0);
}


int	command::handleCmd(client *client1, Server *serv, std::string cmd)
{
	int check = -1;
	std::string chan, message, err;
	if (cmd.compare("JOIN") == 0)
		check = this->JOIN(client1, serv);
	else if (cmd.compare("MODE") == 0)
		check = this->MODE(client1, serv);
	else if (cmd.compare("TOPIC") == 0)
		check = this->TOPIC(client1, serv);
	else if (cmd.compare("INVITE") == 0)
		check = this->INVITE(client1, serv);
	else if (cmd.compare("PRIVMSG") == 0)
		check = this->PRIVMSG(client1, serv);
	else if (cmd.compare("PART") == 0)
		check = this->PART(client1, serv);
	switch (check)
	{
		case 0 :
		{
			std::cout << "COMMAND SUCCESS" << std::endl;
			break ;
		}
		// case 4 :
		// {
		// 	//Penser a envoyer a tous les clients du channel
		// 	chan = serv->getChanName();
		// 	message = NO_INVITE(client1->getNickName(), chan);
		// 	std::cout << "MESSAGE = " << message << std::endl;
		// 	return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		// }
		// case 5 :
		// {
		// 	chan = serv->getChanName();
		// 	message = YES_INVITE(client1->getNickName(), chan);
		// 	std::cout << "MESSAGE = " << message << std::endl;
		// 	return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		// }command::INVITE(client *client1, Server *serv)
		case 6 :
		{
			chan = serv->getChanName();
			message = ERR_INVITEONLYCHAN(client1->getNickName(), chan);
			std::cout << "MESSAGE = " << message << std::endl;
			return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		}
		// case 7 :
		// {
		// 	chan = serv->getChanName();
		// 	message = TOPIC_IS_OFF(client1->getNickName(), chan);
		// 	std::cout << "MESSAGE = " << message << std::endl;
		// 	return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		// }
		case 403 :
		{
			chan = serv->getChanName();
			std::cout << "ERROR 403" << std::endl;
			message = ERR_NOSUCHCHANNEL(client1->getNickName(), chan);
			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
			break ;
		}
		case 461 :
		{
			chan = serv->getChanName();
			message = ERR_NEEDMOREPARAMS(client1->getNickName(), cmd);
			std::cout << "MESSAGE = " << message << std::endl;
			return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		}
		case 471 :
		{
			chan = serv->getChanName();
			message = ERR_CHANNELISFULL(client1->getNickName(), chan);
			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
			//break ;
		}
		case 472 :
		{
			chan = serv->getChanName();
			std::cout << "chan = " << chan << std::endl;
			message = ERR_UNKNOWNMODE(client1->getNickName(), chan);
			std::cout << "MESSAGE = " << message << std::endl;
			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
			// return (0);
		}
		case 475 :
		{
			chan = serv->getChanName();
			std::cout << "ERROR 475" << std::endl;
			message = ERR_BADCHANNELKEY(client1->getNickName(), chan);
			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
			//break ;
		}
		case 482 :
		{
			chan = serv->getChanName();
			std::cout << "NOM DU CHANNEL 482 = " << chan << std::endl;
			std::cout << "client1->getNickName(): " << client1->getNickName() << std::endl;
			std::cout << "client1->getsocketFd(): " << client1->getsocketFd() << std::endl;
			message = ERR_CHANOPRIVSNEEDED(chan);
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
	return (check);
}


// std::string		JOIN();
// std::string		PART();
// std::string		TOPIC();
// std::string		KICK();

// std::string		PRIVMSG();
// std::string		NOTICE();
// std::string		KILL();
// std::string		WALLOPS();
// std::string		bot();
