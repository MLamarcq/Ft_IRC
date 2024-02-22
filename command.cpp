/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlamarcq <mlamarcq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:33:59 by mlamarcq          #+#    #+#             */
/*   Updated: 2024/02/22 17:22:07 by mlamarcq         ###   ########.fr       */
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

	tempClient = serv->findClientBySocket(fd);
	std::list<channel *> ChanList = serv->getListOfChannels();
	std::list<channel *>::iterator it = ChanList.begin();
	std::list<channel *>::iterator ite = ChanList.end();
	while (it != ite)
	{
		std::map<client *, bool> ClMap = (*it)->getListOfClients();
		std::map<client *, bool>::iterator cit = ClMap.begin();
		std::map<client *, bool>::iterator cite = ClMap.end();
		while (cit != cite)
		{
			if (cit->first->getsocketFd() == fd)
			{
				this->PART_QUIT(tempClient, serv, (*it)->getName());
				break ;
			}
			cit++;
		}
		it++;
	}

	close(fd);
	FD_CLR(fd, &(serv->M_struct->current_sockets));
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
	if (serv->M_cmdMap.find("USER") != serv->M_cmdMap.end() || serv->M_cmdMap.find("userhost") != serv->M_cmdMap.end())
	{
		std::map<std::string, std::vector<std::string> > :: iterator it = serv->M_cmdMap.find("USER");
		if (it != serv->M_cmdMap.end())
		{
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
			if (clientTmp)
			{
				if (serv->findClientByUserName(temp[0].append(clientTmp->getNickName())) != NULL)
				{
					return (ERR_ALREADYREGISTRED(temp[0]));
				}
			}
			if (clientTmp)
			{
				clientTmp->setUserName(temp[0].append(clientTmp->getNickName()));
				clientTmp->setMode(temp[1]);
				clientTmp->setHostName(temp[2]);
				clientTmp->setRealName(temp[3]);
			}
			

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
			if (serv->findClientByUserName(temp[0].append(clientTmp->getNickName()) ) != NULL)
			{
				return (ERR_ALREADYREGISTRED(temp[0]));
			}
			clientTmp->setUserName(temp[0].append(clientTmp->getNickName()));
			clientTmp->setMode(temp[1]);
			clientTmp->setHostName(temp[2]);
			clientTmp->setRealName(temp[3]);
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
	// 
	//if (end == std::string::npos)
	//{
		//size_t lastcar = temp.find('\0');
		//arg = temp.substr(notSpace, lastcar - 1);
		arg =  temp[0];
		if (arg.empty())
		{
			return (ERR_NONICKNAMEGIVEN(arg));
		}
		if (serv->findClientByNickName(arg) != NULL)
		{
			return (ERR_NICKNAMEINUSE(arg, arg));
		}
		
		
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
	
	//temp = serv->M_cmdMap["PASS"];	
	clientTmp = serv->findClientBySocket(fd);
	if (clientTmp && clientTmp->isWelcomeMessageSent())
	{
		return (ERR_ALREADYREGISTRED(clientTmp->getNickName()));
	}
	
	if (temp[0].empty())
	{
		return (ERR_NEEDMOREPARAMS(clientTmp->getNickName(), "PASS"));
	}
	std::string clientPass = temp[0];
	//clientPass = clientPass.substr(0, clientPass.length() - 1);
	clientPass = temp[0];
	if (clientPass.compare(serv->M_pass_wd) != 0)
	{
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
	// 	size++;
	// }
	return (temp);
}


int		command::JOIN(client *client1, Server *serv)
{
	std::vector<std::string> temp;
	std::string message;
	bool first_toggle = false;
	temp = parsTemp(serv->M_cmdMap["JOIN"]);
	int check = 0;

	if (!client1 || !serv)
		return (0);
	if (temp.size() == 0)
		return (0);
	if (temp[0][0] != '#')
	{
		message = ERR_NEEDMOREPARAMS(client1->getNickName(), "JOIN");
		send(client1->getsocketFd(), message.c_str(), message.length(), 0);
		return (0);
	}
	if (serv->checkChannel() == false)
	{
		channel *new_one = new channel;
		//faire une fonction dans client pour delete, utiliser algo for each : delete (it);
		//ou utiliser 
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
					check = (*it)->handleIsInvite(client1, serv, temp);
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
		{
			// check = serv->addClientToChannel(client1, temp);
			check = (*it)->addClientToChannel(client1, serv, temp);
		}
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
	{
	 	// message = ERR_NEEDMOREPARAMS(client1->getNickName(), "MODE");
		// send(client1->getsocketFd(), message.c_str(), message.length(), 0);
		return (0);
	}
	while (i < temp.size())
	{
		i++;
	}
	std::list<channel *> listOfChannels = serv->getListOfChannels();
	std::list<channel *>::iterator it = listOfChannels.begin();
	std::list<channel *>::iterator ite = listOfChannels.end();
	if (*it)
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
		bool ope = true;
		if (temp[0][0] != '#')
		{
			message = NOSUCHCHANNEL(client1->getNickName(), client1->getUserName(), temp[0]);
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			return (0);
		}
		//virifier avant si il existe un - ou un + sinon le message est mal interprete
		if ((*it)->isInThechan(client1) == false)
		{
			message = NOTONCHANNEL(client1->getNickName(), client1->getUserName(), (*it)->getName());
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			return (0);
		}
		ope = (*it)->isOperatorInChan(client1);
		if (ope == true)
		{
			int	arg = whatArg(temp);
			int sign = whatSign(temp);
			serv->setChanName(temp[0]);
			switch (arg)
			{
				case 'i' :
				{
					bool checkIsInvite;
					if (sign == '-')
					{
						checkIsInvite = (*it)->getIsInvite();
						if (checkIsInvite == true)
						{
							(*it)->changeIsInviteOff(client1->getNickName(), client1->getUserName(), "-i");
						}
						break ;
					}
					else
					{
						checkIsInvite = (*it)->getIsInvite();
						if (checkIsInvite == false)
						{
							(*it)->changeIsInviteOn(client1->getNickName(), client1->getUserName(), "+i");
						}
						break ;
					}
				}
				case 't' :
				{
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
					if (temp.size() > 2)
					{
						if (sign == '+')
							(*it)->changePrivileges(client1, client1->getNickName(), client1->getUserName(), "+o", temp[2], 1);
						if (sign == '-')
							(*it)->changePrivileges(client1, client1->getNickName(), client1->getUserName(), "-o", temp[2], 2);
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
					
					if (sign == '+')
					{
						if (temp.size() > 2)
						{
							if ((*it)->getClientLimit() == 100 && temp.size() > 2)
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
		else
		{
			check = 482;
			serv->setChanName(temp[0]);
		}
	}
	else
	{
		bool is_client = false;
		std::list<client *> client_list = serv->getServClientList();
		std::list<client *>::iterator list_it = client_list.begin();
		std::list<client *>::iterator list_ite = client_list.end();
		while (list_it != list_ite)
		{
			if ((*list_it)->getNickName().compare(temp[0]) == 0)
			{
				is_client = true;
				break ;
			}
			list_it++;
		}
		if (is_client == false)
		{
			message = NOSUCHCHANNEL(client1->getNickName(), client1->getUserName(), temp[0]);
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
		}
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
	
	if (temp.size() == 0)
	{
		message = ERR_NEEDMOREPARAMS(client1->getNickName(), "TOPIC");
		send(client1->getsocketFd(), message.c_str(), message.length(), 0);
		return (0);
	}
	while (i < temp.size())
	{
		i++;
	}
	std::list<channel *> list = serv->getListOfChannels();
	std::list<channel *>::iterator it = list.begin();
	std::list<channel *>::iterator ite = list.end();
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
		if (temp.size() < 2)
		{
			// message = 
			return (0);
		}
		else
		{
			bool res = false;
			std::map<client *, bool> tmp = (*it)->getListOfClients();
			std::map<client *, bool>::iterator m_it = tmp.begin();
			std::map<client *, bool>::iterator m_ite = tmp.end();
			while (m_it != m_ite)
			{
				if (m_it->first->getsocketFd() == client1->getsocketFd())
				{
					res = true;
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
					(*it)->setTopic(s_topic);
					message = RPL_TOPIC(client1->getNickName(), (*it)->getName(), (*it)->getTopic());
					(*it)->sendToAllChan(message);
					break ;
				}
				m_it++;
			}
			if (res == false)
			{
				message = NOTONCHANNEL(client1->getNickName(), client1->getUserName(), (*it)->getName());
				send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			}
		}
	}
	else
	{
		message = NOSUCHCHANNEL(client1->getNickName(), client1->getUserName(), temp[0]);
		send(client1->getsocketFd(), message.c_str(), message.length(), 0);
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
		return (0);
	}
	while (i < temp.size())
	{
		i++;
	}
	while (it != ite)
	{
		std::string diez = "#";
		if (temp[1][0] != '#')
		{
			temp[1].insert(0, diez);
		}
		if ((*it)->getName().compare(temp[1]) == 0)
		{
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
								while (cl_it != cl_ite)
								{
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
	if (m_found != serv->M_cmdMap.end())
		temp = parsTemp(serv->M_cmdMap["PRIVMSG"]);
	else
		temp = parsTemp(serv->M_cmdMap["VMSGPRI"]);
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
		std::map<client *, bool> cList = (*it)->getListOfClients();
		std::map<client *, bool>::iterator found_client = cList.find(client1);
		if (found_client == (cList.end()))
		{
			message = NOTONCHANNEL(client1->getNickName(), client1->getUserName(), (*it)->getName());
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			return (0);
		}
		while (i < temp.size())
		{
			message.append(temp[i]);
			message.append(" ");
			i++;
		}
		found = message.find(":");
		if (found != std::string::npos)
			message.erase(found, 1);
		to_send = PRIVMSG_CHAN(client1->getNickName(), client1->getUserName(), (*it)->getName(), message);
		(*it)->sendPrivMsg(client1, to_send);
	}
	else
	{
		size_t j = 1;
		// while (j < temp.size())
		// {
		// 	j++;
		// }
		bool exist = false;
		client *dest;
		std::list<client *> cList = serv->getServClientList();
		std::list<client *>::iterator cit = cList.begin();
		std::list<client *>::iterator cite = cList.end();
		for (; cit != cite; cit++)
		{
			if ((*cit)->getNickName().compare(temp[0]) == 0)
			{
				exist = true;
				dest = (*cit);
				break ;
			}
		}
		while (j < temp.size())
		{
			message.append(temp[j]);
			message.append(" ");
			j++;
		}
		found = message.find(":");
		if (found != std::string::npos)
			message.erase(found, 1);
		if (exist == false)
		{
			// message = NOSUCHUSER(client1->getNickName(), client1->getUserName(), temp[0]);
			message = ERR_NOSUCHNICK(client1->getNickName(), temp[0]);
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			return (0);
		}
		// to_send = PRIVMSG_USER(client1->getNickName(), client1->getUserName(), dest->getNickName(), message);
		to_send = PRIVMSG_CHAN	(client1->getNickName(), client1->getUserName(), dest->getNickName(), message);
		send(dest->getsocketFd(), to_send.c_str(), to_send.length(), 0);
	}
	(void)client1;
	(void)serv;
	return (check);
}

int	command::PART(client *client1, Server *serv)
{
	bool toggle = false;
	// bool empty = false;
	std::string message, reason;
	size_t i = 1;
	std::vector<std::string> temp;
	std::map<std::string, std::vector<std::string > >::iterator m_found = serv->M_cmdMap.find("PART");
	if (m_found != serv->M_cmdMap.end())
		temp = parsTemp(serv->M_cmdMap["PART"]);
	else
		temp = parsTemp(serv->M_cmdMap["RTPA"]);
	if (temp.size() == 0)
		return (0);
	while (i < temp.size())
	{
		reason.append(temp[i]);
		reason.append(" ");
		i++;
	}
	std::list<channel *> chanList = serv->getListOfChannels();
	std::list<channel *>::iterator it = chanList.begin();
	std::list<channel *>::iterator ite = chanList.end();
	i = 1;
	while (it != ite)
	{
		if ((*it)->getName().compare(temp[0]) == 0)
		{
			toggle = true;
			break ;
		}
		i++;
		it++;
	}
	if (*it && toggle == true)
	{
		bool inChan = false;
		std::map<client *, bool> clMap = (*it)->getListOfClients();
		// std::map<client *, bool> clMap = (*it)->getListOfClients();
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
		(*it)->eraseCLientFromChan(client1, reason);
		std::map<client *, bool> cl_map = (*it)->getListOfClients();
		std::map<client *, bool>::iterator cite = cl_map.end();
		for (std::map<client *, bool>::iterator cit = cl_map.begin(); cit != cite; cit++)
		{
		}
		// }
		if ((*it)->getListOfClients().empty())
		{
			// empty = true;
			if (temp.size() == 2)
				message = XKICK(client1->getNickName(), client1->getUserName(), (*it)->getName(), client1->getNickName(), reason);
			else
				message = XKICK(client1->getNickName(), client1->getUserName(), (*it)->getName(), client1->getNickName(), "");
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			message.erase();
			if (temp.size() == 2)
				message = RPL_PART(client1->getNickName(), (*it)->getName(), reason);
			else
				message = RPL_PART(client1->getNickName(), (*it)->getName(), "");
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			//faire une fonction qui delete le cannal dans serv
			// serv->eraseChannelFromList((*it));
			// return (0);
		}
		else
		{
			if (temp.size() >= 2)
				message = PART_CHAN(client1->getNickName(), client1->getUserName(), (*it)->getName(), reason);
			else
				message = PART_CHAN(client1->getNickName(), client1->getUserName(), (*it)->getName(), "");
			(*it)->sendPrivMsg(client1, message);
			message.erase();
			if (temp.size() == 2)
				message = XKICK(client1->getNickName(), client1->getUserName(), (*it)->getName(), client1->getNickName(), reason);
			else
				message = XKICK(client1->getNickName(), client1->getUserName(), (*it)->getName(), client1->getNickName(), "");
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			message.erase();
			if (temp.size() == 2)
				message = RPL_PART(client1->getNickName(), (*it)->getName(), reason);
			else
				message = RPL_PART(client1->getNickName(), (*it)->getName(), "");
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
		}
		// if (empty == true)
		// 	serv->eraseChannelFromList((*it));
	}
	else
	{
		message = NOSUCHCHANNEL(client1->getNickName(), client1->getUserName(), temp[0]);
		send(client1->getsocketFd(), message.c_str(), message.length(), 0);
	}
	return (0);
}

int	command::PART_QUIT(client *client1, Server *serv, std::string chan)
{
	bool toggle = false;
	// bool empty = false;
	std::string message, reason;
	size_t i = 1;
	std::vector<std::string> temp;
	// std::map<std::string, std::vector<std::string > >::iterator m_found = serv->M_cmdMap.find("PART");
	if (chan.empty() == 0)
	{
		temp.push_back(chan);
	}
	if (temp.size() == 0)
		return (0);
	while (i < temp.size())
	{
		reason.append(temp[i]);
		reason.append(" ");
		i++;
	}
	std::list<channel *> chanList = serv->getListOfChannels();
	std::list<channel *>::iterator it = chanList.begin();
	std::list<channel *>::iterator ite = chanList.end();
	i = 1;
	while (it != ite)
	{
		if ((*it)->getName().compare(temp[0]) == 0)
		{
			toggle = true;
			break ;
		}
		i++;
		it++;
	}
	if (*it && toggle == true)
	{
		bool inChan = false;
		std::map<client *, bool> clMap = (*it)->getListOfClients();
		// std::map<client *, bool> clMap = (*it)->getListOfClients();
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
		(*it)->eraseCLientFromChan(client1, reason);
		std::map<client *, bool> cl_map = (*it)->getListOfClients();
		std::map<client *, bool>::iterator cite = cl_map.end();
		for (std::map<client *, bool>::iterator cit = cl_map.begin(); cit != cite; cit++)
		{
		}
		// }
		if ((*it)->getListOfClients().empty())
		{
			// empty = true;
			if (temp.size() == 2)
				message = XKICK(client1->getNickName(), client1->getUserName(), (*it)->getName(), client1->getNickName(), reason);
			else
				message = XKICK(client1->getNickName(), client1->getUserName(), (*it)->getName(), client1->getNickName(), "");
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			message.erase();
			if (temp.size() == 2)
				message = RPL_PART(client1->getNickName(), (*it)->getName(), reason);
			else
				message = RPL_PART(client1->getNickName(), (*it)->getName(), "");
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			//faire une fonction qui delete le cannal dans serv
			// serv->eraseChannelFromList((*it));
			// return (0);
		}
		else
		{
			if (temp.size() >= 2)
				message = PART_CHAN(client1->getNickName(), client1->getUserName(), (*it)->getName(), reason);
			else
				message = PART_CHAN(client1->getNickName(), client1->getUserName(), (*it)->getName(), "");
			(*it)->sendPrivMsg(client1, message);
			message.erase();
			if (temp.size() == 2)
				message = XKICK(client1->getNickName(), client1->getUserName(), (*it)->getName(), client1->getNickName(), reason);
			else
				message = XKICK(client1->getNickName(), client1->getUserName(), (*it)->getName(), client1->getNickName(), "");
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			message.erase();
			if (temp.size() == 2)
				message = RPL_PART(client1->getNickName(), (*it)->getName(), reason);
			else
				message = RPL_PART(client1->getNickName(), (*it)->getName(), "");
			send(client1->getsocketFd(), message.c_str(), message.length(), 0);
		}
		// if (empty == true)
		// 	serv->eraseChannelFromList((*it));
	}
	else
	{
		message = NOSUCHCHANNEL(client1->getNickName(), client1->getUserName(), temp[0]);
		send(client1->getsocketFd(), message.c_str(), message.length(), 0);
	}
	return (0);
}

int	command::KICK(int fd, Server* serv)
{
	size_t								i = 0;
	size_t								i_target = 0;
	std::string							message;
	std::string							content;
	std::vector<std::string>			temp = parsTemp(serv->M_cmdMap["KICK"]);
	client*								client_target;
	client*								client_emit = serv->findClientBySocket(fd);
	std::map<client *, bool>::iterator	target_rm;
	bool								emit_found = false;
	bool								target_found = false;
	bool								InChan = false;
	bool								ChanExist = false;

	while (i < temp.size())
	{
		i++;
	}

	content = temp[2];
	i = 1;
	while (i < temp.size())
	{
		content += temp[i];
		content.append(" ");
		i++;
	}

	if (temp.size() < 1)
	{
		message = ERR_NEEDMOREPARAMS(client_emit->getNickName(), "KICK");
		return (send(client_emit->getsocketFd(), message.c_str(), message.length() ,0));
	}
	client_target = serv->findClientByNickName(temp[1]);
	if (client_target) //&& client_emit->getOperatorStatus() == true)
	{
		std::list<channel *>::iterator it = serv->M_listOfChannels.begin();
		for (; it != serv->M_listOfChannels.end(); it++)
		{
			// check if both user are in the same channel
			if ((*it)->getName().compare(temp[0]) == 0)
			{
				ChanExist = true;
				break ;
			}
		}
		if (ChanExist == false)
		{
			message = ERR_NOSUCHNICK(client_emit->getNickName(), temp[0]);
			(send(client_emit->getsocketFd(), message.c_str(), message.length(), MSG_NOSIGNAL));
		}
		if (*it && ChanExist == true)
		{
			for (std::map<client *, bool>::iterator ite = (*it)->_listOfClients.begin(); ite != (*it)->_listOfClients.end(); ite++)
			{
				if ((*ite).first->getNickName() == client_emit->getNickName())
				{
					InChan = true;
					if (ite->second == true)
					{
						emit_found = true;
						break ;
					}
				}
			}
			if (InChan == false)
			{
				if (client_emit->getOperatorStatus() == false)
				{
					message = ERR_KICKPRIVSNEEDED(client_emit->getNickName(), client_emit->getUserName());
					send(client_emit->getsocketFd(), message.c_str(), message.length(), MSG_NOSIGNAL);
					return (0);
				}
				message = NOTONCHANNEL(client_emit->getNickName(), client_emit->getUserName(), temp[0]);
				send(client_emit->getsocketFd(), message.c_str(), message.length(), MSG_NOSIGNAL);
				return (0);
			}
			if (emit_found == false)
			{
				message = ERR_CHANOPRIVSNEEDED((*it)->getName());
				send(client_emit->getsocketFd(), message.c_str(), message.length(), 0);
				return (0);
			}
			for (std::map<client *, bool>::iterator ite = (*it)->_listOfClients.begin(); ite != (*it)->_listOfClients.end(); ite++)
			{
				if ((*ite).first->getNickName().compare(client_target->getNickName()) && emit_found)
				{
					target_rm = ite;
					target_found = true;
				}
				i_target++;
			}
			if (target_found)
			{
				(*it)->eraseCLientFromChan(client_target, "reason");
				message = XKICK(client_emit->getNickName(), client_emit->getUserName(), (*it)->getName(), client_target->getNickName(), content);
				send(client_target->getsocketFd(), message.c_str(), message.length() ,0);
				message.erase();
				message = KICK_CHAN(client_emit->getNickName(), client_emit->getUserName(), (*it)->getName(), client_target->getNickName(), content);
				(*it)->sendToAllChan(message);
				message.erase();
				message = YOU_KICK(client_emit->getNickName(), client_emit->getUserName(), (*it)->getName(), client_target->getNickName(), content);
				send(client_target->getsocketFd(), message.c_str(), message.length(), 0);
				return (0);
			}
		}
	}
	else
	{
		message = ERR_NOSUCHNICK(client_emit->getNickName(), temp[1]);
		return (send(client_emit->getsocketFd(), message.c_str(), message.length(), MSG_NOSIGNAL));
	}
	(void)emit_found;
	(void)client_emit;
	(void)message;
	(void)fd;
	(void)serv;
	return (0);
}

int	command::OPER(int fd, Server* serv)
{
	size_t						i = 0;
	client*						client_target;
	client*						client_emit = serv->findClientBySocket(fd);
	std::string					message;
	std::vector<std::string>	temp;

	temp = parsTemp(serv->M_cmdMap["OPER"]);
	while (i < temp.size())
	{
		i++;
	}

	if (temp.size() != 2)
	{
		message = ERR_NEEDMOREPARAMS(client_emit->getNickName(), "OPER");
		return (send(client_emit->getsocketFd(), message.c_str(), message.length() ,0));
	}
	if (temp.size() == 2)
	{
		if (!temp[0].empty())
		{
			client_target = serv->findClientByNickName(temp[0]);
			if (!client_target)
			{
				message = ERR_NOSUCHNICK(client_emit->getNickName(), temp[0]);
				return (send(client_emit->getsocketFd(), message.c_str(), message.length(), 0));
			}
			else
			{
				if (temp[1] == "abc")
				{
					client_target->setOperatorStatus(true);
					bool res = false;
					std::list<channel *> ChanList = serv->getListOfChannels();
					std::list<channel *>::iterator it = ChanList.begin();
					std::list<channel *>::iterator ite = ChanList.end();
					while (it != ite)
					{
						if ((*it)->isInChan(client_emit) == true)
						{
							(*it)->setOperators(client_emit);
							res = true;
							break ;
						}
						it++;
					}
					if (client_target->getNickName().compare(client_emit->getNickName()) == 0)
					{
						if (res == true)
						{
							message = ISNOWOPEFROMCHAN(client_emit->getNickName(), client_emit->getUserName(), (*it)->getName(), client_emit->getNickName() );
							(*it)->sendToAllChan(message);
							return (send(client_emit->getsocketFd(), message.c_str(), message.length() , 0));
						}
						message = ISNOWOPE(client_emit->getNickName(), client_emit->getUserName());
						return (send(client_emit->getsocketFd(), message.c_str(), message.length() ,0));
					}
					else
					{
						if (res == true)
						{
							message = ISNOWOPEFROMCHAN(client_emit->getNickName(), client_emit->getUserName(), (*it)->getName(), client_target->getNickName() );
							(*it)->sendToAllChan(message);
							return (send(client_emit->getsocketFd(), message.c_str(), message.length() , 0));
						}
						message = ISNOWOPE_BY_USER(client_emit->getNickName(), client_emit->getUserName());
						return (send(client_target->getsocketFd(), message.c_str(), message.length() ,0));
					}
				}
				else
				{
					message = ERR_BADOPERKEY(client_emit->getNickName(), client_emit->getUserName());
					return (send(client_emit->getsocketFd(), message.c_str(), message.length() ,0));

					// message = ERR_PASSWDMISMATCH(client_target->getNickName());
					// return (send(client_target->getsocketFd(), message.c_str(), message.length() ,0));
				}
			}
		}
	}
	(void)fd;
	(void)i;
	(void)temp;
	return (0);
}

int	command::WALLOPS(int fd, Server* serv)
{
	size_t						i = 0;
	std::string					message, to_send;
	std::string					content;
	client*						client_emit = serv->findClientBySocket(fd);
	std::vector<std::string>	temp = parsTemp(serv->M_cmdMap["wallops"]);
	client						*me = serv->findClientBySocket(fd);

	// ERR_NEEDMOREPARAMS

	if (temp.size() < 1)
	{
		message = ERR_NEEDMOREPARAMS(client_emit->getNickName(), "KICK");
		return (send(client_emit->getsocketFd(), message.c_str(), message.length() ,0));
	}

	content = temp[0];
	i = 1;
	while (i < temp.size())
	{
		content += temp[i];
		i++;
	}
	if (me->getOperatorStatus())
	{
		for (std::list<client *>::iterator it = serv->listOfClients.begin(); it != serv->listOfClients.end(); it++)
		{
			if ((*it)->getOperatorStatus() && (*it)->getsocketFd() != fd)
			{
				message = content; // je n'ai trouve aucun rpl valable
				to_send = PRIVMSG_WALLOPS(client_emit->getNickName(), client_emit->getUserName(), message);
				send((*it)->getsocketFd(), to_send.c_str(), to_send.length(), 0);
			}
		}
	}
	else
	{
		message = ERR_WALLOPSPRIVSNEEDED(client_emit->getNickName(), client_emit->getUserName());
		send(client_emit->getsocketFd(), message.c_str(), message.length(), 0);
	}
	(void)fd;
	(void)serv;
	return (0);
}

int	command::KILL(int fd, Server* serv)
{
	size_t						i = 0;
	std::string					message;
	std::string					content;
	std::vector<std::string>	temp;
	int							fd_target = 0;
	client*						client_emit = serv->findClientBySocket(fd);
	client*						client_target;

	temp = parsTemp(serv->M_cmdMap["kill"]);
	while (i < temp.size())
	{
		i++;
	}

	content = temp[0];
	i = 1;
	while (i < temp.size())
	{
		content += temp[i];
		i++;
	}

	if (i == 1 || i == 2)
	{
		client_target = serv->findClientByNickName(temp[0]);
		if (!client_target)
		{
			message = ERR_NOSUCHNICK(client_emit->getNickName(), temp[0]);
			return (send(client_emit->getsocketFd(), message.c_str(), message.length(), 0));
		}
		else if (client_emit->getOperatorStatus() == false)
		{
			message = ERR_NOPRIVILEGES(client_emit->getNickName());
			return (send(client_emit->getsocketFd(), message.c_str(), message.length(), 0));
		}
		else
		{
			if (i == 1)
				message = XQUIT(client_target->getNickName(), client_emit->getNickName(), "");
			else
				message = XQUIT(client_target->getNickName(), client_emit->getNickName(), content);
			fd_target = client_target->getsocketFd();
			this->QUIT(fd_target, serv);
			return (send(client_emit->getsocketFd(), message.c_str(), message.length(), 0));
		}
	}
	else
	{
		message = ERR_NEEDMOREPARAMS(client_emit->getNickName(), "kill");
		return (send(client_emit->getsocketFd(), message.c_str(), message.length(), 0));
	}
	(void)fd_target;
	(void)fd;
	(void)serv;
	return (0);
	// ERR_NOPRIVILEGES
	// ERR_NOSUCHNICK
	// ERR_NEEDMOREPARAMS

	// ERR_CANTKILLSERVER
}

int	command::NOTICE(int fd, Server* serv)
{
	size_t						i = 0;
	std::string					message;
	std::string					content;
	std::vector<std::string>	temp;
	client*						client_target;
	client*						client_emit = serv->findClientBySocket(fd);

	temp = parsTemp(serv->M_cmdMap["NOTICE"]);
	while (i < temp.size())
	{
		i++;
	}

	if (temp.size() < 2)
	{
		message = ERR_NEEDMOREPARAMS(client_emit->getNickName(), "NOTICE");
		return (send(client_emit->getsocketFd(), message.c_str(), message.length() ,0));
	}

	i = 1;
	while (i < temp.size())
	{
		content.append(temp[i]);
		content.append(" ");
		i++;
	}

	client_target = serv->findClientByNickName(temp[0]);
	if (client_target)
	{
		for (std::list<client *>::iterator ite = serv->listOfClients.begin(); ite != serv->listOfClients.end(); ite++)
		{
			if ((*ite)->getNickName().compare(client_target->getNickName()) == 0)
			{
				message = XNOTICE(client_emit->getNickName(), client_emit->getNickName(), client_target->getUserName(), content);
				return (send(client_target->getsocketFd(), message.c_str(), message.length() ,0));
			}
		}
	}
	message = ERR_NOSUCHNICK(client_emit->getNickName(), temp[0]);
	send(client_emit->getsocketFd(), message.c_str(), message.length(), 0);
	(void)fd;
	(void)serv;
	return (0);
	// ERR_NORECIPIENT
	// ERR_CANNOTSENDTOCHAN
	// ERR_NOTOPLEVEL
	// ERR_WILDTOPLEVEL
	// ERR_TOOMANYTARGETS
	// ERR_NOSUCHNICK
	// RPL_AWAY
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
			break ;
		}
		// case 4 :
		// {
		// 	//Penser a envoyer a tous les clients du channel
		// 	chan = serv->getChanName();
		// 	message = NO_INVITE(client1->getNickName(), chan);
		// 	return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		// }
		// case 5 :
		// {
		// 	chan = serv->getChanName();
		// 	message = YES_INVITE(client1->getNickName(), chan);
		// 	return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		// }command::INVITE(client *client1, Server *serv)
		case 6 :
		{
			chan = serv->getChanName();
			message = ERR_INVITEONLYCHAN(client1->getNickName(), chan);
			return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		}
		// case 7 :
		// {
		// 	chan = serv->getChanName();
		// 	message = TOPIC_IS_OFF(client1->getNickName(), chan);
		// 	return (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
		// }
		case 403 :
		{
			chan = serv->getChanName();
			message = ERR_NOSUCHCHANNEL(client1->getNickName(), chan);
			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
			break ;
		}
		case 461 :
		{
			chan = serv->getChanName();
			message = ERR_NEEDMOREPARAMS(client1->getNickName(), cmd);
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
			message = ERR_UNKNOWNMODE(client1->getNickName(), chan);
			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
			// return (0);
		}
		case 475 :
		{
			chan = serv->getChanName();
			message = ERR_BADCHANNELKEY(client1->getNickName(), chan);
			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
			//break ;
		}
		case 479 :
		{
			chan = serv->getChanName();
			message = ERR_BADCHANNAME(client1->getNickName(), chan);
			return (send(client1->getsocketFd(), message.c_str(), message.length() ,0));
		}
		case 482 :
		{
			chan = serv->getChanName();
			message = ERR_CHANOPRIVSNEEDED(chan);
			int rtn = (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
			return (rtn);
			//break ;
		}
		default :
		{
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
// std::string		WALLOPS();
// std::string		bot();
