#include "channel.hpp"

channel::channel() : _name("default"), _pswd("1234")
{
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
	(void)copy;
	return (*this);
}

channel::~channel()
{
	return ;
}

std::string	channel::getName(void) const
{
	return (this->_name);
}

void	channel::setName(std::string parameter)
{
	this->_name = parameter;
	return ;
}
int	channel::setPassword(void)
{
	std::string password;
	bool toggle = false;
	std::cout << "Please choose a password for this channel (4 digits): ";
	while (toggle == false)
	{
		if (!std::getline(std::cin, password))
		{
			std::cout << "End of file called. Channel setup failed. Cancel all actions" << std::endl;
			return (0);
		}
		if (password.empty())
		{
			std::cout << "Empty password. Please ty again : ";
		}
		if (isDigit(password) == 1)
		{
			this->_pswd = password;
			// std::cout << "Password for " << this->_name << "set. It is : " << this->_pswd << std::endl;
			toggle = true;
		}
		password.erase();
	}
	// std::cout << "Le nom du channel est : " << this->_name << std::endl;
	// std::cout << "Le mot de passe est : " << this->_pswd << std::endl;
	return (1);
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
	if (enterPassword() == 0)
		return ;
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