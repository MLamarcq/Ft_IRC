#include "channel.hpp"

channel::channel()
{
	this->_nbrClients = 0;
	this->_clientLimit = 100;
	this->_isInvite = false;
	this->_topicEstate = false;
	return ;
}

channel::channel(std::string channelName, int passWord)
{
	(void)channelName;
	(void)passWord;
}

channel::channel(const channel& copy)
{
	(void)copy;
}

channel & channel::operator=(const channel& copy)
{
	if (this!= &copy)
	{
		if (copy.getClientLimit())
			this->_clientLimit = copy.getClientLimit();
		if (copy.getNbrOfClients())
			this->_nbrClients = copy.getNbrOfClients();
		if (copy.getClientLimit())
			this->_name = copy.getName();
		if (copy.getClientLimit())
			this->_pswd = copy.getPassword();
		for (std::map<client *, bool>::iterator m_it = _listOfClients.begin(); m_it != _listOfClients.end(); ++m_it)
		{
			delete m_it->first;
		}
		this->_listOfClients.clear();
		std::map<client *, bool> map = copy.getListOfClients();
		std::map<client *, bool>::iterator it = map.begin();
		std::map<client *, bool>::iterator ite = map.begin();
		while (it != ite)
		{
			client *copy2 = copyClient(it->first);
			if (copy2)
			{
				this->_listOfClients[copy2] = it->second;
			}
			it++;
		}
	}
	(void)copy;
	return (*this);
}

channel::~channel()
{
	return ;
}

client	*channel::copyClient(client *original)
{
	if (!original)
	{
		return (NULL);
	}
	client *copy = new client;
	copy->setNickName(original->getNickName());
	copy->setRealName(original->getRealName());
	copy->setUserName(original->getUserName());
	copy->setHostName(original->getHostName());
	copy->setOperatorStatus(original->getOperatorStatus());
	copy->setWelcomeMessageSent(original->isWelcomeMessageSent());
	copy->setsocketFd(original->getsocketFd());
	copy->setPort(original->getPort());
	copy->setIp(original->getIp());
	copy->setNumberOfChannelJoined(original->getNumberOfChannelJoined());
	return (copy);
}

std::string	channel::getName(void) const
{
	return (this->_name);
}

std::string channel::getPassword(void) const
{
	return (this->_pswd);
}

std::string channel::getTopic(void) const
{
	return (this->_topic);
}

int			channel::getNbrOfClients(void) const
{
	return (this->_nbrClients);
}

int			channel::getClientLimit(void) const
{
	return (this->_clientLimit);
}

bool		channel::getIsInvite(void) const
{
	return (this->_isInvite);
}

bool		channel::getTopicEstate(void) const
{
	return (this->_topicEstate);
}

void	channel::increaseNbrCLient(void)
{
	this->_nbrClients++;
	return ;
}

std::list<client *>		channel::getWaitingList(void) const
{
	return (this->_waitingList);
}

std::map<client*, bool> channel::getListOfClients(void) const
{
	return (this->_listOfClients);
}

void	channel::setWaitingList(client *client1)
{
	if (!client1)
		return ;
	this->_waitingList.push_back(client1);
	return ;
}

int	channel::setName(std::string parameter)
{
	if (parameter.size() > 200)
	{
		return (0);
	}
	size_t space = parameter.find(' ');
	size_t coma = parameter.find(',');
	size_t sound = parameter.find('\a');
	if (space != std::string::npos || coma != std::string::npos || sound != std::string::npos)
	{
		this->_name = parameter;
		return (0);
	}
	size_t r = parameter.find('\r');
	if (r != std::string::npos)
	{
		parameter = parameter.substr(0, r);
	}
	// i = 0;
	// while (i < parameter.size())
	// {
	// 	i++;
	// }
	this->_name = parameter;
	return (1);
}
void	channel::setPassword(std::string password)
{
	// std::string password;
	// bool toggle = false;
	// while (toggle == false)
	// {
	// 	if (!std::getline(std::cin, password))
	// 	{
	// 		return (0);
	// 	}
	// 	if (password.empty())
	// 	{
	// 	}
	// 	if (isDigit(password) == 1)
	// 	{
	// 		this->_pswd = password;
	// 		toggle = true;
	// 	}
	// 	password.erase();
	// }
	this->_pswd = password;
	return ;
}

void	channel::setListofClient(client *client1)
{
	if (!client1)
	{
		return ;
	}
	if (client1->getOperatorStatus() == true)
		this->_listOfClients[client1] = true;
	else
		this->_listOfClients[client1] = false;
	return ;
}

void	channel::setTopic(std::string topic)
{
	if (topic.empty())
	{
		return ;
	}
	if (this->_topic.empty() == 0)
		this->_topic.erase();
	this->_topic = topic;
	return ;
}

void	channel::setOperators(client *client1)
{
	if (!client1)
	{
		return ;
	}
	this->_listOfClients[client1] = true;
	return ;
}

void	channel::addClientToTheChannel(client *client1)
{
	if (!client1)
	{
		return ;
	}
	if (client1->getOperatorStatus() == true)
		this->_listOfClients[client1] = true;
	else
		this->_listOfClients[client1] = false;
	return ;
}

bool	channel::isInChan(client *client1)
{
	std::map<client *, bool>::iterator found = this->_listOfClients.find(client1);
	if (found != this->_listOfClients.end())
		return (true);
	return (false);
}

// void	channel::printMap(void) const
// {
// 	std::map<client *, bool>::const_iterator it = this->_listOfClients.begin();
// 	std::map<client *, bool>::const_iterator ite = this->_listOfClients.end();
// 	while (it != ite)
// 	{
// 		if (it->first->getNickName().size() > 10)
// 		else
// 		if (it->second == true)
// 		else
// 		it++;
// 	}
// 	return ;
// }

int	channel::setChannelFirstTime(client *client1, Server *serv, std::vector<std::string> temp)
{
	if (this->setName(temp[0]) == 0)
	{
		serv->setChanName(this->getName());
		// std::string message = ERR_NOSUCHCHANNEL(client1->getNickName(), this->getName());
		
		return (479);
	}
	// if (temp.size() == 2)
	// 	this->setPassword(temp[1]);
	this->setListofClient(client1);
	// this->setOperators(client1);
	serv->setNewChannel(this);
	this->increaseNbrCLient();
	return (0);
}

void		channel::changeIsInviteOn(std::string name, std::string username, std::string mode)
{
	std::string message;
	
	if (name.empty())
	{
		return ;
	}
	if (username.empty())
	{
		return ;
	}
	if (this->_isInvite == false)
		this->_isInvite = true;
	message = MODE_CHANNEL_YES_INVITE(name, username, this->getName(), mode);
	this->sendToAllChan(message);
	return ;
}

void		channel::changeIsInviteOff(std::string name, std::string username, std::string mode)
{
	std::string message;
	
	if (name.empty())
	{
		return ;
	}
	if (username.empty())
	{
		return ;
	}
	if (this->_isInvite == true)
		this->_isInvite = false;
	message = MODE_CHANNEL_NO_INVITE(name, username, this->getName(), mode);
	this->sendToAllChan(message);
	return ;
}

void	channel::setNewPassWord(std::string pass, std::string name, std::string username, std::string mode)
{
	std::string message;

	if (pass.empty())
	{
		return ;
	}
	if (this->_pswd.empty() == 0)
	{
		return ;
	}
	this->_pswd = pass;
	//message = RPL_PASSWORD_CHANGED(name, this->getName(), pass);
	message = MODE_CHANNEL_NEWMDP(name, username, this->getName(), mode, pass);
	this->sendToAllChan(message);
	(void)username;
	(void)name;
	(void)pass;
	return ;
}

void	channel::clearPassWord(std::string name, std::string username, std::string mode)
{
	std::string message;

	if (this->_pswd.empty())
	{
		return ;
	}
	this->_pswd.erase();
	message = MODE_CHANNEL_CLEARMDP(name, username, this->getName(), mode);
	this->sendToAllChan(message);
	(void)username;
	(void)name;
	return ;
}


void	channel::changePrivileges(client * client_emit, std::string name, std::string username, std::string mode, std::string client1, int code)
{
	std::string message;
	bool present = false;

	if (client1.empty())
	{
		return ;
	}
	if (name.empty())
	{
		return ;
	}
	if (username.empty())
	{
		return ;
	}
	std::map<client *, bool>::iterator it = this->_listOfClients.begin();
	std::map<client *, bool>::iterator ite = this->_listOfClients.end();
	// if (it == this->_listOfClients.end())
	// {
	// 	return ;
	// }
	while (it != ite)
	{
		if (it->first->getNickName().compare(client1) == 0)
		{
			present = true;
			break ;
		}
		it++;
	}
	if (present == false)
	{
		message = CLIENT_NOTONCHANNEL(client_emit->getNickName(), client_emit->getUserName(), this->_name, client1, mode);
		send(client_emit->getsocketFd(), message.c_str(), message.length(), 0);
		return ;
	}
	if ((it)->first->getOperatorStatus() == true)
	{
		message = WRONG_USER_MODE(client_emit->getNickName(), client_emit->getUserName(), this->_name, client1, mode);
		send(client_emit->getsocketFd(), message.c_str(), message.length(), 0);
		return ;
	}
	if (code == 1)
	{
		if (it->second == false)
		{
			it->second = true;
			message = MODE_CHANNEL_NOWOP(name, username, this->getName(), mode, client1);
			this->sendToAllChan(message);
		}
	}
	else if (code == 2)
	{
		if (it->second == true)
		{
			it->second = false;
			message = MODE_CHANNEL_NOMOREOP(name, username, this->getName(), mode, client1);
			this->sendToAllChan(message);
		}
	}
	return ;
}

void	channel::setClientLimit(std::string name, std::string username, std::string mode, std::string nb)
{
	int check = 0;
	int res = 0;
	std::string message;

	if (nb.empty())
	{
		return ;
	}
	if (name.empty())
	{
		return ;
	}
	if (username.empty())
	{
		return ;
	}
	check = isDigit(nb);
	if (check == 0)
	{
		return ;
	}
	res = std::atoi(nb.c_str());
	this->_clientLimit = res;
	message = MODE_CHANNEL_CLIENTLIMIT(name, username, this->getName(), mode, nb);
	this->sendToAllChan(message); 
	return ;
}

void	channel::eraseClientLimit(std::string name, std::string username, std::string mode)
{
	std::string message;

	if (name.empty())
	{
		return ;
	}
	if (username.empty())
	{
		return ;
	}
	this->_clientLimit = 100;
	message = MODE_CHANNEL_NOCLIENTLIMIT(name, username, this->getName(), mode);
	this->sendToAllChan(message);
	return ;
}

void	channel::setTopicEstate(std::string name, std::string username, std::string mode, int code)
{
	std::string message;

	if (name.empty())
	{
		return ;
	}
	if (username.empty())
	{
		return ;
	}
	if (code == 1)
	{
		if (this->_topicEstate == true)
		{
			this->_topicEstate = false;
			message = MODE_TOPIC_OFF(name, username, this->getName(), mode);
			this->sendToAllChan(message);
		}
	}
	if (code == 2)
	{
		if (this->_topicEstate == false)
		{
			this->_topicEstate = true;
			message = MODE_TOPIC_ON(name, username, this->getName(), mode);
			this->sendToAllChan(message);
		}
	}
	return ;
}

int	channel::handleIsInvite( client *client1, Server *serv, std::vector<std::string> temp)
{
	int check = 0;
	if (client1->getOperatorStatus() == true)
		return (0);
	if (this->getIsInvite() == true)
	{
		bool isWaiting = false;
		std::list<client *> cl_list = this->_waitingList;
		std::list<client *>::iterator cl_it = cl_list.begin();
		std::list<client *>::iterator cl_ite = cl_list.end();
		std::list<client *>::iterator cl_find;
		cl_find = std::find(cl_it, cl_ite, client1);
		if (cl_find != cl_list.end())
		{
			while (cl_it != cl_ite)
			{
				if ((*cl_it)->getsocketFd() == client1->getsocketFd())
				{
					// cl_it = this->_waitingList.erase(cl_it);
					// cl_list.erase(cl_it);
					cl_it = cl_list.erase(cl_it);
					isWaiting = true;
					break ;
				}
				cl_it++;
			}
			this->_waitingList = cl_list;
			if (this->_waitingList.empty() == 0)
			{
				std::list<client *> cl_list_2 = this->_waitingList;
				std::list<client *>::iterator cl_it_2 = cl_list_2.begin();
				std::list<client *>::iterator cl_ite_2 = cl_list_2.end();
				while (cl_it_2 != cl_ite_2)
				{
					cl_it_2++;
				}
			}
			else
			{
			}
		}
		std::list<client *>::iterator new_it = this->_waitingList.begin();
		std::list<client *>::iterator new_ite = this->_waitingList.end();
		for(; new_it != new_ite; new_it++)
		if (isWaiting == false)
		{
			serv->setChanName(temp[0]);
			check = 6;
		}
	}
	return (check);
}

int	channel::addClientToChannel(client *client1, Server *serv, std::vector<std::string> temp)
{
	if (!client1)
	{
		return (1);
	}
	// if (parameter.empty())
	// {
	// 	return ;
	// }
	std::string message;
	// std::list<channel *>::iterator it = this->M_listOfChannels.begin();
	// std::list<channel *>::iterator ite = this->M_listOfChannels.end();
	// while (it != ite)
	// {
	if (this->getName().compare(temp[0]) == 0)
	{
		if (this->getNbrOfClients() >= this->getClientLimit()) //poser la question aux gars
		{
			serv->setChanName(this->getName());
			return (471);
		}
		if (this->getPassword().empty() == 0 && temp.size() >= 2)
		{
			if (this->getPassword().compare(temp[1]) == 0)
			{
				this->addClientToTheChannel(client1);
				this->increaseNbrCLient();
				this->welcomeInChanMessage(client1);
				if (this->getTopic().empty() == 0)
				{
					message = RPL_TOPIC(client1->getNickName(), this->getName(), this->getTopic());
					send(client1->getsocketFd(), message.c_str(), message.length(), 0);
				}
				else
				{
					message = RPL_NOTOPIC(client1->getNickName(), this->getName());
					send(client1->getsocketFd(), message.c_str(), message.length(), 0);
				}
				message.erase();
				message = JOIN_IN_CHAN(client1->getNickName(), client1->getUserName(), this->_name);
				this->sendPrivMsg(client1, message);
				return (0);
			}
			else
			{
				serv->setChanName(this->getName());
				return (475);
			}
		}
		else if (this->getPassword().empty() && temp.size() < 2)
		{
			this->addClientToTheChannel(client1);
			this->increaseNbrCLient();
			this->welcomeInChanMessage(client1);
			if (this->getTopic().empty() == 0)
			{
				message = RPL_TOPIC(client1->getNickName(), this->getName(), this->getTopic());
				send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			}
			else
			{
				message = RPL_NOTOPIC(client1->getNickName(), this->getName());
				send(client1->getsocketFd(), message.c_str(), message.length(), 0);
			}
			message.erase();
			message = JOIN_IN_CHAN(client1->getNickName(), client1->getUserName(), this->_name);
			this->sendPrivMsg(client1, message);
			return (0);
		}
		else
		{
			serv->setChanName(this->getName());
			return (475);
		}
		// break ;
	}
	// 	it++;
	// }
	return (0);
}



bool	channel::isInThechan(client *client1) const
{
	bool res = false;
	std::map<client *, bool>::const_iterator found = this->_listOfClients.find(client1);
	std::map<client *, bool>::const_iterator ite = this->_listOfClients.end();

	if (found != ite)
		res = true;
	return (res);
}

bool	channel::isOperatorInChan(client *client1) const
{
	bool res = false;
	std::map<client *, bool>::const_iterator found = this->_listOfClients.find(client1);
	// std::map<client *, bool>::iterator ite = this->_listOfClients.end();

	if (found->second == true)
		res = true;
	return (res);
}

void	channel::sendToAllChan(std::string message)
{
	std::map<client *, bool>::iterator it = this->_listOfClients.begin();
	std::map<client *, bool>::iterator ite = this->_listOfClients.end();
	while (it != ite)
	{
		send(it->first->getsocketFd(), message.c_str(), message.length(), 0);
		it++;
	}
	return ;
}

void	channel::sendPrivMsg(client *client1, std::string message)
{
	int i = 0;
	std::map<client *, bool> m_client = this->_listOfClients;
	std::map<client *, bool>::iterator it = m_client.begin();
	std::map<client *, bool>::iterator ite = m_client.end();
	while (it != ite)
	{
		if (it != ite && it->first->getsocketFd() == client1->getsocketFd())
			it++;
		if (it != ite)
			send(it->first->getsocketFd(), message.c_str(), message.length(), 0);
		if (it != ite)
			it++;
		i++;
	}
	return ;
}

int	channel::isDigit(std::string str) const
{
	int i = 0;
	while (str[i])
	{
		if ((str[i] >= '0' && str[i] <= '9'))
			i++;
		else
		{
			return (0);
		}
	}
	// if (i != 4)
	// {
	// 	return (0);
	// }
	return (1);
}

void	channel::welcomeInChanMessage(client *client1)
{
	std::string message, clientList;
	int toSend;

	std::map<client *, bool>::iterator it = this->_listOfClients.begin();
	std::map<client *, bool>::iterator ite = this->_listOfClients.end();
	while(it != ite)
	{
		if (it->second == true)
			clientList = clientList + "@";
		clientList = clientList + it->first->getNickName() + " ";
		it++;
	}
	message = RPL_NAMREPLY(client1->getNickName(), this->getName(), clientList);
	toSend = (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
	if (toSend < 0)
	{
		return ;
	}
	message.erase();
	message = RPL_ENDOFNAMES(client1->getNickName(), this->getName());
	toSend = (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
	if (toSend < 0)
	{
		return ;
	}
	// message.erase();
	// message = WELCOME_CHAN(client1->getNickName(), client1->getUserName(), this->_name);
	// toSend = (send(client1->getsocketFd(), message.c_str(), message.length(), 0));
	// if (toSend < 0)
	// {
	// 	return ;
	// }
	return ;
}

void	channel::eraseCLientFromChan(client *client1, std::string reason)
{
	std::string message, tmp;
	std::map<client *, bool>::iterator found_it = this->_listOfClients.find(client1);
	std::map<client *, bool>::iterator it = this->_listOfClients.begin();
	std::map<client *, bool>::iterator ite = this->_listOfClients.end();
	

	while (it != ite)
	{
		it++;
	}

	if (found_it != ite)
	{
		this->_listOfClients.erase(client1);
	}
	std::map<client *, bool>::iterator cit = this->_listOfClients.begin();
	std::map<client *, bool>::iterator cite = this->_listOfClients.end();
	while (cit != cite)
	{
		cit++;
	}
	// if (reason.empty())
	// {
	// 	tmp = "";
	// 	message = KICK(client1->getNickName(), client1->getUserName(), this->getName(), client1->getNickName(), tmp);
	// 	send(client1->getsocketFd(), message.c_str(), message.length(), 0);
	// }
	// else
	// {
	// 	message = KICK(client1->getNickName(), client1->getUserName(), this->getName(), client1->getNickName(), reason);
	// 	send(client1->getsocketFd(), message.c_str(), message.length(), 0);
	// }
	(void)reason;
	return ;
}
