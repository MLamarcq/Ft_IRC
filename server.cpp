#include "server.hpp"

Server::Server(void)
{
	return ;
}

Server::Server(std::string port, std::string pass_wd)
{
	if (port.compare("6667") != 0 && port.compare("6697") != 0)
		throw(WrongPortException());
	this->M_port = port;
	this->M_pass_wd = pass_wd;
	this->M_struct = NULL;
	this->M_working = true;
	init_struct();
	fill_commands_vector();
	return ;
}

Server::Server(Server const &src)
{
	*this = src;
	return ;
}

Server & Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->M_port = rhs.getPort();
		this->M_pass_wd = rhs.getPass_Wd();
		Copy_Struct(rhs);
	}
	return (*this);
}

Server::~Server(void)
{
	if (this->M_struct)
		delete this->M_struct;
	if (!this->listOfClients.empty())
	{
		std::list<client *> clList = this->listOfClients;
		std::list<client *>::iterator it = clList.begin();
		std::list<client *>::iterator ite = clList.end();
		
		while (it != ite)
		{
			delete (*it);
			// clList.erase(it);
			it++;
		}
	}
	if (!this->M_listOfChannels.empty())
	{
		std::list<channel *> chList = this->M_listOfChannels;
		std::list<channel *>::iterator ch_it = chList.begin();
		std::list<channel *>::iterator ch_ite = chList.end();
		
		while (ch_it != ch_ite)
		{
			delete (*ch_it);
			// clList.erase(it);
			ch_it++;
		}
	}
	//penser a delete les channel + clients
	return ;
}
bool containsAlphanumeric(const std::vector<std::string>& strVector) {
    for (std::vector<std::string>::const_iterator it = strVector.begin(); it != strVector.end(); ++it) {
        const std::string& str = *it;

        for (std::string::const_iterator chIt = str.begin(); chIt != str.end(); ++chIt) {
            char ch = *chIt;

            if (std::isalnum(static_cast<unsigned char>(ch))) {
                return true;  // Return true as soon as an alphanumeric character is found
            }
        }
    }

    return false; // Return false if no alphanumeric character is found in any string
}
std::string	intTostring(int number)
{
	std::string result;
	char digitChar;
	
	while (number <= 0)
	{
		std::srand(0);
		number = std::rand();
	}
	while (number > 0) 
	{
		digitChar = '0' + (number % 10);
		result.push_back(digitChar);
		number /= 10;
	}
	if (result.size() > 4)
		result = result.substr(0, 4);
	return (result);
}

std::string const	getTime()
{
	time_t	t(time(NULL));
	std::string	res(ctime(&t));
	res.erase(res.end() - 1);
	
	return (res);
}

std::string Server::getPort(void) const
{
	return (this->M_port);
}

std::string Server::getPass_Wd(void) const
{
	return (this->M_pass_wd);
}
std::string Server::getChanName(void) const
{
	return (this->M_chanName);
}

void		Server::setChanName(std::string name)
{
	this->M_chanName = name;
	return ;
}

std::list<client *>	Server::getServClientList(void) const
{
	return (this->listOfClients);
}
// std::vector<std::vector<std::string> > Server::getCmdArgs(void) const
// {
// 	return (this->M_args);
// }

void	Server::init_struct(void)
{
	this->M_struct = new t_serv;
	this->M_struct->serveurSockFd = 0;
	this->M_struct->clientSockFd = 0;
	bzero(&this->M_struct->sockStructServ, sizeof(this->M_struct->sockStructServ));
	this->M_struct->sockStructServ.sin_family = AF_INET;
	this->M_struct->sockStructServ.sin_addr.s_addr = INADDR_ANY;
	this->M_struct->sockStructServ.sin_port = htons(std::atoi(this->M_port.c_str()));
	FD_ZERO(&this->M_struct->current_sockets);
	FD_ZERO(&this->M_struct->temp_sockets);
	return ;
}

void	Server::fill_commands_vector(void)
{
	// faut que j'ajoute les 
	this->M_commands.push_back("NICK");
	this->M_commands.push_back("PASS");
	this->M_commands.push_back("SSPA");
	this->M_commands.push_back("USER");
	this->M_commands.push_back("MODE");
	this->M_commands.push_back("PING");
	this->M_commands.push_back("QUIT");
	this->M_commands.push_back("OPER");
	this->M_commands.push_back("JOIN");
	this->M_commands.push_back("PART");
	this->M_commands.push_back("RTPA");
	this->M_commands.push_back("kill");
	this->M_commands.push_back("llki");
	this->M_commands.push_back("NOTICE");
	this->M_commands.push_back("TOPIC");
	this->M_commands.push_back("KICK");
	this->M_commands.push_back("PRIVMSG");
	this->M_commands.push_back("VMSGPRI");
	this->M_commands.push_back("wallops");
	this->M_commands.push_back("userhost");
	this->M_commands.push_back("INVITE");
	return ;
}

void	Server::Copy_Struct(Server const &rhs)
{
	if (this->M_struct)
		delete this->M_struct;
	this->M_struct = new t_serv;
	this->M_struct->serveurSockFd = rhs.M_struct->serveurSockFd;
	this->M_struct->clientSockFd = rhs.M_struct->clientSockFd;
	this->M_struct->sockStructServ.sin_family = rhs.M_struct->sockStructServ.sin_family;
	this->M_struct->sockStructServ.sin_addr.s_addr = rhs.M_struct->sockStructServ.sin_addr.s_addr;
	this->M_struct->sockStructServ.sin_port = rhs.M_struct->sockStructServ.sin_port;
	this->M_struct->current_sockets = rhs.M_struct->current_sockets;
	return ;
}

void	Server::Setup_Socket(void)
{
	this->M_struct->serveurSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->M_struct->serveurSockFd < 0)
	{
		throw(WrongSocketFdEexception());
	}
	int option = 1;
	setsockopt(this->M_struct->serveurSockFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (bind(this->M_struct->serveurSockFd, reinterpret_cast<struct sockaddr*>(&this->M_struct->sockStructServ), sizeof(this->M_struct->sockStructServ)) < 0)
	{
		throw(ErrorBindingException());
	}
	if (listen(this->M_struct->serveurSockFd, 100) < 0)
	{
		throw(ErrorListenException());
	}
	addNewSocketToThePool(this->M_struct->serveurSockFd);
	return ;
}

void	Server::addNewSocketToThePool(int new_socket) const
{
	FD_SET(new_socket, &this->M_struct->current_sockets);
	return ;
}


int		Server::i_send_message(int clientSockFd, std::string message) const
{
	return (send(clientSockFd, message.c_str(), message.length() ,0));
}


void	Server::i_accept_connexion(void)
{
	socklen_t clientLen;

	clientLen = sizeof(this->M_struct->sockStructClient);
	this->M_struct->clientSockFd = accept(this->M_struct->serveurSockFd, reinterpret_cast<struct sockaddr*>(&this->M_struct->sockStructClient), &clientLen);
	if (this->M_struct->clientSockFd < 0)
	{
		throw (WrongClientSocketFdException());
	}
	this->i_handle_first_connexion();
	return ;
}

void	Server::i_handle_first_connexion(void)
{
	

	char buff[513];
	std::string nickname;
	std::string realName;
	std::string userName;
	std::string hostName;
	
	client * clientPtr;
	std::string convert;
	int			personalNumber;
	std::string name;
	personalNumber = std::rand();
	convert = intTostring(personalNumber);
	name = "NotSetYet";
	name.append("#");
	name.append(convert);
	clientPtr = this->createClient();
	clientPtr->setNickName("NotSetYet");
	clientPtr->setsocketFd(this->M_struct->clientSockFd);
	clientPtr->setIp(inet_ntoa(this->M_struct->sockStructClient.sin_addr));
	clientPtr->setPort(ntohs(this->M_struct->sockStructClient.sin_port));
	//ajouter le client dans la liste
	this->listOfClients.push_back(clientPtr);
	
	int countCommand = 0;
	if (this->M_cmdMap.empty())
	{
		if (requestParsing(this->M_struct->clientSockFd) == 0)
		{
			//this->M_cmdMap.clear();
			//close(i);
			//FD_CLR(i, &(this->M_struct->current_sockets));
			return ;
		}
	}
	std::map<std::string, std::vector <std::string> >::iterator m_it = this->M_cmdMap.begin();
	std::map<std::string, std::vector <std::string> >::iterator m_ite = this->M_cmdMap.end();
	while (m_it != m_ite)
	{
		if (m_it->first.find("NICK") != std::string::npos)
			countCommand++;
		if (m_it->first.find("USER") != std::string::npos)
			countCommand++;
		if ((m_it->first.find("PASS") != std::string::npos) || (m_it->first.find("SSPA") != std::string::npos))
			countCommand++;
		m_it++;
	}
	if (countCommand != 3)
	{
		//this->M_requestVector.clear();
		//this->M_cmdMap.clear();
		if (clientPtr)
			this->eraseClientFromList(clientPtr->getNickName());
		return ;
	}
	std::string returnValue = chooseAndExecuteAction(this->M_struct->clientSockFd);
	if (returnValue == "WRONG PASS" || returnValue == "WRONG USER" || returnValue == "WRONG NICK")
	{
		//this->M_requestVector.clear();
		this->M_cmdMap.clear();
		if (clientPtr)
			this->eraseClientFromList(clientPtr->getNickName());
		return ;
	}
	//this->M_requestVector.clear();
	this->M_cmdMap.clear();
	//send welcome message
	sendWelcomeMessage(clientPtr);
	
	memset(buff, 0, 513);


}

int	Server::requestParsing(int ClientFd)
{
	char	buff[513];
	int		reader;
	size_t	find_r = 0;
	size_t	find_n = 0;
	int		count = 0;
	reader = read(ClientFd, buff, 512);
	if (reader == -1)
	{
		return (0);
	}
	if (reader == 0)
	{
		std::string message = commandObj->QUIT(ClientFd, this);
		i_send_message(ClientFd,message);
		return (0);
	}
	
	buff[reader] = '\0';
	std::string tmp(buff);
	size_t size  = tmp.size();
	while (find_n < size - 1)
	{
		/*find_r = tmp.find('\r', find_n + 1);*/
		find_n = tmp.find('\n', find_n + 1);
		if (find_r == std::string::npos /*|| find_n == std::string::npos*/)
		{
			return (0);
		}
		count++;
	}
	if (fillVectorRequest(count, tmp) == 0)
		return (0);
	if (fillCmdMap() == 0)
		return (0);
	// putRequestArgInVector();
	return (1);
}

client*	Server::findClientByUserName(std::string clientUserName)
{
	std::string temp;
	for (std::list<client *>::iterator it = this->listOfClients.begin(); it != this->listOfClients.end(); it++) {
		temp = (*it)->getUserName();
		//if (temp.find(clientUserName) != std::string::npos)
		//	return (*it);
		if (temp.compare(clientUserName) == 0)
		{
			return (*it);
		}
	}
	return (NULL);
}


void				Server::sendWelcomeMessage(client* clientPtr)
{
	// i_send_message(this->M_struct->clientSockFd,":localhost 001 " + clientPtr->getNickName() + " :Welcome to the Internet Relay Network " + clientPtr->getNickName()+"!" + clientPtr->getUserName() + "@localhost\r\n");
	// i_send_message(this->M_struct->clientSockFd,":localhost 002 "+ clientPtr->getNickName() +  " :Your host is localhost, running version 1.0\r\n");
	// i_send_message(this->M_struct->clientSockFd,":localhost 003 "+ clientPtr->getNickName() + " :This server was created 01/01/24\r\n");
	// i_send_message(this->M_struct->clientSockFd,":localhost 004 " + clientPtr->getNickName() + " localhost 1.0\r\n");
	i_send_message(this->M_struct->clientSockFd, RPL_WELCOME(clientPtr->getNickName(), clientPtr->getUserName(), clientPtr->getHostName()));
	i_send_message(this->M_struct->clientSockFd, RPL_YOURHOST(clientPtr->getNickName()));
	i_send_message(this->M_struct->clientSockFd, RPL_CREATED(clientPtr->getNickName(), getTime()));
	i_send_message(this->M_struct->clientSockFd,RPL_MYINFO(clientPtr->getNickName()));
	clientPtr->setWelcomeMessageSent(true);
	clientPtr->hello();
}

int	Server::fillVectorRequest(int count, std::string tmp)
{
	size_t token = 0;
	int i = 0;
	while (i < count)
	{
		std::string string_copy = tmp;
		std::string temp;
		token = string_copy.find('\n');
		if (token == std::string::npos)
		{
		
			temp = string_copy;
			this->M_requestVector.push_back(temp);
			temp.erase();
			i++;

		}
		else
		{
			temp = string_copy.substr(0, token);
			this->M_requestVector.push_back(temp);
			temp.erase();
			tmp = tmp.substr(token + 1, tmp.size());
			i++;
		}
	}
	return (1);
}


int	Server::fillCmdMap(void)
{
	std::vector<std::string>::iterator it = this->M_requestVector.begin();
	std::vector<std::string>::iterator ite = this->M_requestVector.end();
	while (it != ite)
	{
		it++;
	}
	if (this->M_requestVector.empty())
	{
		return (1);
	}
	std::string first;
	std::string second;
	std::vector<std::string> temp1;
	std::vector<std::string> temp2;
	it = this->M_requestVector.begin();
	ite = this->M_requestVector.end();
	std::map< std::string, std::vector<std::string> >::iterator mi_it;
	
	while (it != ite)
	{
		size_t space = it->find(' ');
		if (space == std::string::npos)
		{
			first = it->c_str();
			//temp = split_string_v2(second, ' ');
			//temp1[0] = "";
			temp1.push_back("");
			temp1[0] = "";
			mi_it = this->M_cmdMap.find(first);
			if ((mi_it == this->M_cmdMap.end()) || first == "")
			{
				if (first != "")
				{
					this->M_cmdMap[first] = temp1;
					int count = temp1.size();
					int i = 0;
					while (i < count)
					{
						i++;
					}
				}
				else
				{
					temp1.push_back("");
					temp1[0] = second;
					this->M_cmdMap[first] = temp1;
				}
			}
			it++;
			//return (0);
		}
		else 
		{
			first = it->substr(0, space);
			second = it->substr(space + 1, it->size());

			mi_it = this->M_cmdMap.find(first);
			if ((mi_it == this->M_cmdMap.end()) || first == "")
			{
				if (first != "")
				{
					temp2 = split_string_v2(second, ' ');
					int count = temp2.size();
					int i = 0;
					while (i < count)
					{
						i++;
					}
					this->M_cmdMap[first] = temp2;
				}
				else
				{
					temp1.push_back("");
					temp1[0] = second;
					this->M_cmdMap[first] = temp1;
				}
			}
			it++;
		}
		
	}

	std::map<std::string, std::vector<std::string> >::iterator m_it = this->M_cmdMap.begin();
	std::map<std::string, std::vector<std::string> >::iterator m_ite = this->M_cmdMap.end();
	while (m_it != m_ite)
	{
		for (size_t i = 0; i < m_it->second.size(); ++i)
		{
		}
		m_it++;
	}

	m_it = this->M_cmdMap.begin();
	m_ite = this->M_cmdMap.end();

	std::map<std::string, std::vector<std::string> >::iterator m_it2 = this->M_cmdMap.begin();
	std::map<std::string, std::vector<std::string> >::iterator m_ite2 = this->M_cmdMap.end();
	
	while (m_it != m_ite)
	{
		if (m_it->first == "" && (m_it->second[0].find("DONE") == std::string::npos))
		{
			std::string tempKey;
			std::string tempValue;
			tempValue = m_it->second[0];
			while (m_it2 != m_ite2)
			{
				m_ite2--;
				if (!containsAlphanumeric(m_ite2->second))
				{
					tempKey.append(m_ite2->first);
					m_ite2->second.push_back("DONE");
				}
				
			}
			std::vector<std::string> vectTemp;
			std::vector<std::string> vectTemp2;
			vectTemp.push_back(tempValue);
			vectTemp2.push_back("DONE");
			//this->M_cmdMap[tempKey];
			this->M_cmdMap[tempKey] = vectTemp;
			this->M_cmdMap[m_it->first] = vectTemp2;
			
		}
		m_it++;
	}
	// gérer plusieurs arguments => non chois assumé ya écrit couper command pas argument et ya pas tous les gens qui l'ont fait
	// gérer à la fisrt connexion
	// gérer quit
	m_it = this->M_cmdMap.begin();
	m_ite = this->M_cmdMap.end();

	while (m_it != m_ite)
	{
		for (size_t i = 0; i < m_it->second.size(); ++i)
		{
		}
		m_it++;
	}
	
	this->M_requestVector.clear();
	return (1);
}

std::vector<std::string> split_string_v2(const std::string& s, char delimiter)
{
	std::vector<std::string> temp;
	std::stringstream ss(s);
	std::string token;
	
	while (std::getline(ss, token, delimiter))
	{
		temp.push_back(token);
	}
	
	return temp;
}



std::string	Server::chooseAndExecuteAction(int clientFd)
{
	std::map<std::string, std::vector <std::string> >::iterator m_it = this->M_cmdMap.begin();
	std::map<std::string, std::vector <std::string> >::iterator m_ite = this->M_cmdMap.end();
	bool	toggle = false;
	std::string returnValue;
	returnValue = "nothing launch";
	for (; m_it != m_ite; m_it++)
	{
		int i = 1;
		toggle = false;
		// std::string message = "On recoit bien le message";
		// if (send(clientFd, message.c_str(), message.length(), 0) < 0)
		std::vector<std::string>::iterator it = this->M_commands.begin();
		std::vector<std::string>::iterator ite = this->M_commands.end();
		for (; it != ite; it++)
		{
			//std::map<std::string, std::string>::iterator it_found = this->M_cmdMap.find(*(it));
			
			//if (it_found != this->M_cmdMap.end())
			if (m_it->first.find(*it) != std::string::npos)
			{

				toggle = true;
				//on lance la fonction switch, on lui passe i
				returnValue = executeCmd(i, clientFd);
				if (returnValue == "WRONG PASS")
					return ("WRONG PASS");
				if (returnValue == "WRONG NICK")
					return ("WRONG NICK");
				if (returnValue == "WRONG USER")
					return ("WRONG USER");
				
			}
			if (toggle == true)
				break ;
			i++;
		}
	}
	return (returnValue) ;
}

std::string	Server::executeCmd(int i, int clientFd)
{
	switch (i)
	{
		case 1 :
		{
			client* clientPtr;
			std::string msg;
			std::string oldNick;

			// :haha!imrane@localhost NICK bob
			// peut être il va manquer le code 00x
			clientPtr = this->findClientBySocket(clientFd);
			oldNick = clientPtr->getNickName();
			std::string message = commandObj->NICK(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG NICK");
			}
			else
			{
				msg.append(":");
				msg.append(oldNick);
				msg.append("!");
				msg.append(clientPtr->getUserName());
				msg.append("@");
				msg.append(clientPtr->getHostName());
				msg.append(" ");
				//msg_nick + "NICK " + args + "\r\n"
				msg.append("NICK ");
				msg.append(clientPtr->getNickName());
				msg.append("\r\n");
				i_send_message(clientFd, msg);
			}
			break ;
		}
		case 2 :
		{
			std::string message = commandObj->PASS(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG PASS");
			}
			break ;
		}
		case 3 :
		{
			std::string message = commandObj->PASS(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG PASS");
			}
			break ;
		}
		case 4 :
		{
			client* clientTmp;

			clientTmp = this->findClientBySocket(clientFd);
			std::string message = commandObj->USER(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG USER");
			}
			
			
			break ;
		}
		case 5 :
		{
			client *client1 = this->findClientBySocket(clientFd);
			//int sendReturn = commandObj->MODE(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "MODE");
			if (sendReturn == -1)
			{
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				return ("Error mode");
			}
			break ;
		}
		case 6 :
		{
			std::string message = commandObj->PING(clientFd, this);
			i_send_message(clientFd,message);
			return ("PING");
			break ;
		}
		case 7 :
		{
			std::string message = commandObj->QUIT(clientFd, this);
			i_send_message(clientFd,message);
			break ;
		}
		case 8 :
		{
			commandObj->OPER(clientFd, this);
			break ;
		}
		case 9 :
		{
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "JOIN");
			if (sendReturn == -1)
			{
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				return ("Error Joinin channel");
			}
			// if (message.find("nothing") == std::string::npos)
			// {
			// 	i_send_message(clientFd,message);
			// 	return ("WRONG USER");
			// }
			break ;
		}
		case 10 :
		{
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "PART");
			if (sendReturn == -1)
			{
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				return ("Error Joinin channel");
			}
			break ;
		}
		case 11 :
		{
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "PART");
			if (sendReturn == -1)
			{
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				return ("Error Joinin channel");
			}
			break ;
		}
		case 12 :
		{
			commandObj->KILL(clientFd, this);
			break ;
		}
		case 13 :
		{
			commandObj->KILL(clientFd, this);
			break ;
		}
		case 14 :
		{
			commandObj->NOTICE(clientFd, this);
			break ;
		}
		case 15 :
		{
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "TOPIC");
			if (sendReturn == -1)
			{
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				return ("Error Joinin channel");
			}
			break ;
		}
		case 16 :
		{
			commandObj->KICK(clientFd, this);
			break ;
		}
		case 17 :
		{
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "PRIVMSG");
			if (sendReturn == -1)
			{
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				return ("Error Joinin channel");
			}
			break ;
		}
		case 18 :
		{
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "PRIVMSG");
			if (sendReturn == -1)
			{
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				return ("Error Joinin channel");
			}
			break ;
		}
		case 19 :
		{
			commandObj->WALLOPS(clientFd, this);
			break ;
		}
		case 20 :
		{
			client* clientTmp;

			clientTmp = this->findClientBySocket(clientFd);
			std::string message = commandObj->USER(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG USER");
			}
			
			
			break ;
		}
		case 21 :
		{
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "INVITE");
			if (sendReturn == -1)
			{
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				return ("Error Joinin channel");
			}
			break ;
		}
		default :
		{
			//std::string message = "421  was not coded =)\r\n";
			//i_send_message(clientFd,message);
			break ;
		}
	}
	return ("nothing");
}

std::list<channel *> Server::getListOfChannels(void) const
{
	return (this->M_listOfChannels);
}

void	Server::setNewChannel(channel *chan)
{
	if(!chan)
	{
		return ;
	}
	this->M_listOfChannels.push_back(chan);
	std::list<channel *>::iterator it = this->M_listOfChannels.begin();
	std::list<channel *>::iterator ite = this->M_listOfChannels.end();
	while (it != ite)
	{
		it++;
	}
	return ;
}

// int	Server::addClientToChannel(client *client1, std::vector<std::string> temp)
// {
// 	if (!client1)
// 	{
// 		return (1);
// 	}
// 	// if (parameter.empty())
// 	// {
// 	// 	return ;
// 	// }
// 	std::string message;
// 	std::list<channel *>::iterator it = this->M_listOfChannels.begin();
// 	std::list<channel *>::iterator ite = this->M_listOfChannels.end();
// 	while (it != ite)
// 	{
// 		if ((*it)->getName().compare(temp[0]) == 0)
// 		{
// 			if ((*it)->getNbrOfClients() >= (*it)->getClientLimit()) //poser la question aux gars
// 			{
// 				this->setChanName((*it)->getName());
// 				return (471);
// 			}
// 			if ((*it)->getPassword().empty() == 0 && temp.size() >= 2)
// 			{
// 				if ((*it)->getPassword().compare(temp[1]) == 0)
// 				{
// 					(*it)->addClientToTheChannel(client1);
// 					(*it)->increaseNbrCLient();
// 					(*it)->printMap();
// 					(*it)->welcomeInChanMessage(client1);
// 					if ((*it)->getTopic().empty() == 0)
// 					{
// 						message = RPL_TOPIC(client1->getNickName(), (*it)->getName(), (*it)->getTopic());
// 						send(client1->getsocketFd(), message.c_str(), message.length(), 0);
// 					}
// 					return (0);
// 				}
// 				else
// 				{
// 					this->setChanName((*it)->getName());
// 					return (475);
// 				}
// 			}
// 			else if ((*it)->getPassword().empty() && temp.size() < 2)
// 			{
// 				(*it)->addClientToTheChannel(client1);
// 				(*it)->increaseNbrCLient();
// 				(*it)->printMap();
// 				(*it)->welcomeInChanMessage(client1);
// 				if ((*it)->getTopic().empty() == 0)
// 				{
// 					message = RPL_TOPIC(client1->getNickName(), (*it)->getName(), (*it)->getTopic());
// 					send(client1->getsocketFd(), message.c_str(), message.length(), 0);
// 				}
// 				return (0);
// 			}
// 			else
// 			{
// 				this->setChanName((*it)->getName());
// 				return (475);
// 			}
// 			break ;
// 		}
// 		it++;
// 	}
// 	return (0);
// }
//USER fonctionne pas dans fist connexion car il US prend l'arg precedent et ER pareil
// faut faire en sorte que quand ya justre command je prend pas arg precedent

bool	Server::checkChannel(void) const
{
	if (this->M_listOfChannels.empty())
		return (false);
	return (true);
}

void	Server::i_handle_request(int i)
{
	if (requestParsing(i) == 0)
	{
		//this->M_requestVector.clear();
		this->M_cmdMap.clear();
		close(i);
		FD_CLR(i, &(this->M_struct->current_sockets));
		return ;
	}
	if (!this->findClientBySocket(i) && this->M_cmdMap.size() > 1 )
	{
		int countCommand = 0;
		std::map<std::string, std::vector <std::string> >::iterator m_it = this->M_cmdMap.begin();
		std::map<std::string, std::vector <std::string> >::iterator m_ite = this->M_cmdMap.end();
		while (m_it != m_ite)
		{
			if (m_it->first.find("NICK") != std::string::npos)
				countCommand++;
			if (m_it->first.find("USER") != std::string::npos)
				countCommand++;
			if ((m_it->first.find("PASS") != std::string::npos) || (m_it->first.find("SSPA") != std::string::npos))
				countCommand++;
			m_it++;
		}
		
		if (countCommand == 3)
		{
			i_handle_first_connexion();
			return ;
		}
	}
	if (this->findClientBySocket(i))
	{
		std::string returnValue;
		returnValue = chooseAndExecuteAction(i);
		if (!(returnValue == "nothing launch" /*|| returnValue == "PING")*/))
		{
			this->M_cmdMap.clear();
		}
	}
}
void	Server::eraseChannelFromList(channel *chan)
{
	std::list<channel *>::iterator found;
	found = std::find(this->M_listOfChannels.begin(), this->M_listOfChannels.end(), chan);
	if (found != this->M_listOfChannels.end())
	{
		this->M_listOfChannels.erase(found);
		delete (*found);
	}
	return;
}

int		Server::isAlpha(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

void	Server::closeSocket(void)
{
	std::list<client *> clList = this->listOfClients;
	std::list<client *>::iterator it = clList.begin();
	std::list<client *>::iterator ite = clList.end();
	while (it != ite)
	{
		if ((*it)->getsocketFd() > 2 && (*it)->getsocketFd() < 1024)
			close((*it)->getsocketFd());
		it++;
	}
	if (this->M_struct->serveurSockFd)
		close(this->M_struct->serveurSockFd);
	return ;
}

void	Server::mainProgram(void)
{
	while (this->M_working == true)
	{
		getSignal(1);
		this->M_struct->temp_sockets = this->M_struct->current_sockets;
		std::cout << "Waiting..." << std::endl;
		if (select(FD_SETSIZE, &this->M_struct->temp_sockets, NULL, NULL, NULL) < 0)
		{
			throw(SelectFunctionErrorException());
		}
		for (int i=0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &this->M_struct->temp_sockets))
			{
				if (i == this->M_struct->serveurSockFd)
				{
					i_accept_connexion();
					addNewSocketToThePool(this->M_struct->clientSockFd);
				}
				else
				{
					i_handle_request(i);
				}
			}
		}
	}
	return ;
}

void	Server::getSignal(int index)
{
	struct sigaction sa;
	if (index == 1)
	{
		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = &Server::handle_sigint_static;
		serverInstance = const_cast<Server*>(this);
		if (sigaction(SIGINT, &sa, 0) == -1)
		{
			perror("Error setting up SIGINT handler");
			return ;
		}
	}
	else if (index == 2)
	{
		memset(&sa, 0, sizeof(sa));
		 sa.sa_handler = &Server::handle_sigint_static;
        serverInstance = this; 
		if (sigaction(SIGPIPE, &sa, 0) == -1)
		{
			perror("Error setting up SIGPIPE handler");
			return ;
		}
	}
	return ;
}

void Server::handle_sigint_static(int signal)
{
	if (serverInstance)
		serverInstance->handle_sigint(signal);
}


void Server::handle_sigpipe_static(int signal, Server* serverInstance)
{
	if (serverInstance)
		serverInstance->handle_sigpipe(signal);
}


void Server::handle_sigpipe(int signal)
{
	if (signal == SIGPIPE)
	{
	}
	return ;
}

void	Server::handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		this->M_working = false;
		closeSocket();
	}
	return ;
}

client* Server::createClient()
{
  client* ptr;
  ptr = new client();
  return (ptr);
}

client *Server::findClientBySocket(int clientSocketFd)
{
	for (std::list<client *>::iterator it = this->listOfClients.begin(); it != this->listOfClients.end(); it++) {
		if ((*it)->getsocketFd() == clientSocketFd)
			return (*it);
	}
	return (NULL);
}

client *Server::findClientByNickName(std::string clientNickname)
{
	std::string temp;
	for (std::list<client *>::iterator it = this->listOfClients.begin(); it != this->listOfClients.end(); it++) {
		temp = (*it)->getNickName();
		if (temp.compare(clientNickname) == 0)
		{
			return (*it);
		}
	}
	return (NULL);
}

void	Server::eraseClientFromList(std::string clientNickname)
{
	std::string temp;
	for (std::list<client *>::iterator it = this->listOfClients.begin(); it != this->listOfClients.end(); it++)
	{
		if (it != this->listOfClients.end())
		{
			temp = (*it)->getNickName();
			if (temp.compare(clientNickname) == 0)
			{
				delete (*it);
				listOfClients.erase(it);
				return ;
			}
		}
			
	}
}

const char *Server::WrongPortException::what() const throw()
{
	return ("Wrong port used. Please report to a port allowed by the IRC server. Port needed : 6667 OR 6697");
}

const char *Server::WrongPasswordException::what() const throw()
{
	return ("Wrong password. Please retry with a valid one");
}

const char *Server::WrongSocketFdEexception::what() const throw()
{
	perror("FUNCTION REAL ERROR here is why error socke t");
	return ("Error : Socket Fd failed.");
}

const char *Server::ErrorBindingException::what() const throw()
{
	perror("FUNCTION REAL ERROR here is why error binding ");
	return ("Error : Binding socket failed.");
}

const char *Server::ErrorListenException::what() const throw()
{
	perror("FUNCTION REAL ERROR here is why error listen ");
	return ("Error : Set socket in listen mode failed.");
}

const char *Server::WrongClientSocketFdException::what() const throw()
{
	perror("FUNCTION REAL ERROR here is why error socket ");
	return ("Client fd error. Can't connect with server");
}

const char *Server::SelectFunctionErrorException::what() const throw()
{
	perror("FUNCTION REAL ERROR here is why error select ");
	return ("An error occured with select function");
}


