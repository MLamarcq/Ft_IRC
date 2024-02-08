#include "channel.hpp"

channel::channel()
{
	this->_nbrClients = 0;
	this->_clientLimit = 2;
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
		std::cout << "Client doesn't exist" << std::endl;
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

int			channel::getNbrOfClients(void) const
{
	return (this->_nbrClients);
}

int			channel::getClientLimit(void) const
{
	return (this->_clientLimit);
}

void	channel::increaseNbrCLient(void)
{
	this->_nbrClients++;
	return ;
}

std::map<client*, bool> channel::getListOfClients(void) const
{
	return (this->_listOfClients);
}

int	channel::setName(std::string parameter)
{
	std::cout << "parameter = " << parameter << std::endl;
	// std::cout << "On regarde chaque caractere quand pas de mdp avant" << std::endl;
	// size_t i = 0;
	// while (i < parameter.size())
	// {
	// 	std::cout << "parameter[" << i << "] = " << parameter[i] << std::endl;
	// 	i++;
	// }
	if (parameter.size() > 200)
	{
		std::cout << "+ de 200" << std::endl;
		return (0);
	}
	size_t space = parameter.find(' ');
	size_t coma = parameter.find(',');
	std::cout << "coma = " << coma << std::endl;
	size_t sound = parameter.find('\a');
	if (space != std::string::npos || coma != std::string::npos || sound != std::string::npos)
	{
		std::cout << "ici" << std::endl;
		this->_name = parameter;
		return (0);
	}
	size_t r = parameter.find('\r');
	if (r != std::string::npos)
	{
		std::cout << " trouve !" << std::endl;
		parameter = parameter.substr(0, r);
	}
	// std::cout << "On regarde chaque caractere quand pas de mdp apres" << std::endl;
	// i = 0;
	// while (i < parameter.size())
	// {
	// 	std::cout << "parameter[" << i << "] = " << parameter[i] << std::endl;
	// 	i++;
	// }
	this->_name = parameter;
	return (1);
}
void	channel::setPassword(std::string password)
{
	// std::string password;
	// bool toggle = false;
	// std::cout << "Please choose a password for this channel (4 digits): ";
	// while (toggle == false)
	// {
	// 	if (!std::getline(std::cin, password))
	// 	{
	// 		std::cout << "End of file called. Channel setup failed. Cancel all actions" << std::endl;
	// 		return (0);
	// 	}
	// 	if (password.empty())
	// 	{
	// 		std::cout << "Empty password. Please ty again : ";
	// 	}
	// 	if (isDigit(password) == 1)
	// 	{
	// 		this->_pswd = password;
	// 		// std::cout << "Password for " << this->_name << "set. It is : " << this->_pswd << std::endl;
	// 		toggle = true;
	// 	}
	// 	password.erase();
	// }
	// // std::cout << "Le nom du channel est : " << this->_name << std::endl;
	// // std::cout << "Le mot de passe est : " << this->_pswd << std::endl;
	this->_pswd = password;
	return ;
}

void	channel::setListofClient(client *client1)
{
	if (!client1)
	{
		std::cout << "Error, client does not exist" << std::endl;
		return ;
	}
	this->_listOfClients[client1] = false;
	return ;
}

void	channel::setOperators(client *client1)
{
	if (!client1)
	{
		std::cout << "Error, client does not exist" << std::endl;
		return ;
	}
	this->_listOfClients[client1] = true;
	return ;
}

int		channel::enterPassword(void) const
{
	//std::cout << "Mot de passe = " << this->_pswd << std::endl;
	int i = 1;
	std::cout << "You are trying to reach the channel : " << this->_name << std::endl;
	std::cout << "Please enter password. (" << i + 1 << " tries left) : ";
	bool toggle = false;
	std::string input;
	while (toggle == false)
	{
		if (!std::getline(std::cin, input))
		{
			std::cout << "Enf of file called. Operation cancelled" << std::endl;
			return (0);
		}
		if (input.compare(this->_pswd) == 0)
		{
			toggle = true;
			std::cout << "Welcome to " << this->_name << " !" << std::endl;
		}
		else if (i != 0)
		{
			i--;
			std::cout << "Wrong password. Please try again (" << i + 1 << " try left) : ";
		}
		else
		{
			std::cout << "Connexion to channel " << this->_name << " failed. Back to main menu." << std::endl;
			return (0);
		}
	}
	return (1);
}

void	channel::addClientToTheChannel(client *client1)
{
	if (!client1)
	{
		std::cout << "Client doesn't exist" << std::endl;
		return ;
	}
	// if (enterPassword() == 0)
	// 	return ;
	this->_listOfClients[client1] = false;
	return ;
}

void	channel::printMap(void) const
{
	std::map<client *, bool>::const_iterator it = this->_listOfClients.begin();
	std::map<client *, bool>::const_iterator ite = this->_listOfClients.end();
	// std::cout << "le nom du client : " << it->first->getNickName() << std::endl;
	// std::cout << "Le nom du channel = " << this->_name << std::endl;
	std::cout << std::internal << std::setw(10) << "Channel | ";
	std::cout << std::internal << std::setw(10) << "Client name | ";
	std::cout << std::internal << std::setw(10) << "Is operator |" << std::endl;
	while (it != ite)
	{
		std::cout << this->_name << " | ";
		// std::cout << "Le nom du channel = " << this->_name << std::endl;
		if (it->first->getNickName().size() > 10)
			std::cout << std::setw(9) << std::internal << it->first->getNickName().substr(0, 9) << "." << "| ";
		else
			std::cout << std::setw(10) << std::internal << it->first->getNickName().substr(0, 10) << " | ";
		if (it->second == true)
			std::cout << std::setw(10) << std::internal << "Yes |" << std::endl;
		else
			std::cout << std::setw(10) << std::internal << "No |" << std::endl;
		it++;
	}
	// std::cout << "Le nom du channel = " << this->_name << std::endl;
	// std::cout << std::setw(10) << std::internal << "Yes |" << std::endl;
	// std::cout << std::setw(10) << std::internal << "No |" << std::endl;
	return ;
}

int	channel::setChannelFirstTime(client *client1, Server *serv, channel *new_one, std::vector<std::string> temp)
{
	if (new_one->setName(temp[0]) == 0)
	{
		serv->setChanName(new_one->getName());
		std::cout << "ERROR CHANNEL" << std::endl;
		std::cout << "COMMAND JOIN SOCKET FD = " << client1->getsocketFd() << std::endl;
		// std::string message = ERR_NOSUCHCHANNEL(client1->getNickName(), new_one->getName());
		// std::cout << "le message = " << message << std::endl;
		return (403);
	}
	if (temp.size() == 2)
		new_one->setPassword(temp[1]);
	new_one->setListofClient(client1);
	new_one->setOperators(client1);
	serv->setNewChannel(new_one);
	new_one->increaseNbrCLient();
	return (0);
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
			std::cout << "Wrong password. Only Digit accepted. Please try again : ";
			return (0);
		}
	}
	if (i != 4)
	{
		std::cout << "Too much digit. Please try again : ";
		return (0);
	}
	return (1);
}