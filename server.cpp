#include "server.hpp"

Server::Server(void)
{
	return ;
}

Server::Server(std::string port, std::string pass_wd)
{
	if (port.compare("6667") != 0)
		throw(WrongPortException());
	this->M_port = port;
	if (pass_wd.compare("0000") != 0)
		throw(WrongPasswordException());
	this->M_pass_wd = pass_wd;
	this->M_struct = NULL;
	this->M_working = true;
	// this->serverInstance = NULL;
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
	this->M_commands.push_back("KILL");
	this->M_commands.push_back("LLKI");
	this->M_commands.push_back("NOTICE");
	this->M_commands.push_back("TOPIC");
	this->M_commands.push_back("KICK");
	this->M_commands.push_back("PRIVMSG");
	this->M_commands.push_back("VMSGPRI");
	this->M_commands.push_back("WALLOPS");
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
	std::cout << "waiting for a request..." << std::endl;
	this->M_struct->clientSockFd = accept(this->M_struct->serveurSockFd, reinterpret_cast<struct sockaddr*>(&this->M_struct->sockStructClient), &clientLen);
	std::cout << "New connection , socket fd is " << this->M_struct->clientSockFd << "ip is : " << inet_ntoa(this->M_struct->sockStructClient.sin_addr) << " , port : " << ntohs(this->M_struct->sockStructClient.sin_port) << std::endl;
	if (this->M_struct->clientSockFd < 0)
	{
		throw (WrongClientSocketFdException());
	}
	std::cout << "I have accepted your request !" << std::endl;
	this->i_handle_first_connexion();
	return ;
}

void	Server::i_handle_first_connexion(void)
{
	
	std::cout << "IM IN FIRST CONNEXION\n";

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
	std::cout << "FIRST CONNEXION SOCKET FD = " << clientPtr->getsocketFd() << std::endl;
	clientPtr->setIp(inet_ntoa(this->M_struct->sockStructClient.sin_addr));
	clientPtr->setPort(ntohs(this->M_struct->sockStructClient.sin_port));
	//ajouter le client dans la liste
	this->listOfClients.push_back(clientPtr);
	
	std::cout << "c1\n";
	int countCommand = 0;
	if (this->M_cmdMap.empty())
	{
		if (requestParsing(this->M_struct->clientSockFd) == 0)
		{
			//this->M_cmdMap.clear();
			//std::cout << PURPLE << "MAP IS CLEAR\n" << END;
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
	std::cout << "c2\n";
	if (countCommand != 3)
	{
		std::cout << "c2.1\n";
		std::cout << RED << "PAS NICK PASS & USER" << END <<std::endl;
		//this->M_requestVector.clear();
		//this->M_cmdMap.clear();
	
		this->eraseClientFromList(clientPtr->getNickName());
		return ;
	}
	std::string returnValue = chooseAndExecuteAction(this->M_struct->clientSockFd);
	if (returnValue == "WRONG PASS" || returnValue == "WRONG USER" || returnValue == "WRONG NICK")
	{
		std::cout << "c3\n";
		std::cout << returnValue << std::endl;
		//this->M_requestVector.clear();
		this->M_cmdMap.clear();
		this->eraseClientFromList(clientPtr->getNickName());
		return ;
	}
	//this->M_requestVector.clear();
	this->M_cmdMap.clear();
	//send welcome message
	sendWelcomeMessage(clientPtr);
	
	memset(buff, 0, 513);
	std::cout << "c4\n";


}

int	Server::requestParsing(int ClientFd)
{
	char	buff[513];
	int		reader;
	size_t	find_r = 0;
	size_t	find_n = 0;
	int		count = 0;
	std::cout << BLUE2 << "--------------IM IN REQUEST PARSING----------------\n" << END;
	std::cout << "c1.1\n";
	reader = read(ClientFd, buff, 512);
	std::cout << "read value is " << reader << std::endl;
	if (reader == -1)
	{
		std::cout << "Error reading. Please check client socket." << std::endl;
		return (0);
	}
	if (reader == 0)
	{
		std::string message = commandObj->QUIT(ClientFd, this);
		i_send_message(ClientFd,message);
		std::cout << "NOTHING TO READ\n";
		return (0);
	}
	
	std::cout << "c1.2\n";
	buff[reader] = '\0';
	std::cout << "BUFF IS : " << buff << std::endl;
	std::string tmp(buff);
	std::cout << "tmp = " << tmp << std::endl;
	size_t size  = tmp.size();
	// std::cout << "Request size = " << size << std::endl;
	while (find_n < size - 1)
	{
		/*find_r = tmp.find('\r', find_n + 1);*/
		find_n = tmp.find('\n', find_n + 1);
		if (find_r == std::string::npos /*|| find_n == std::string::npos*/)
		{
			std::cout << "IN REQUEST PARSING\n";
			std::cout << "Wrong request format. Please report to IRC's request format" << std::endl;
			return (0);
		}
		count++;
	}
	std::cout << "c1.3\n";
	std::cout << "find_r = " << find_r << std::endl;
	std::cout << "find_n = " << find_n << std::endl;
	std::cout << "count = " << count << std::endl;
	std::cout << "Tout s'est bien passe" << std::endl;
	if (fillVectorRequest(count, tmp) == 0)
		return (0);
	std::cout << "c1.4\n";
	if (fillCmdMap() == 0)
		return (0);
	// putRequestArgInVector();
	std::cout << "c1.5\n";
	return (1);
}

client*	Server::findClientByUserName(std::string clientUserName)
{
	std::string temp;
	for (std::list<client *>::iterator it = this->listOfClients.begin(); it != this->listOfClients.end(); it++) {
		temp = (*it)->getUserName();
		//if (temp.find(clientUserName) != std::string::npos)
		//	return (*it);
		//std::cout << "USERNAME FIND IS " << temp << std::endl;
		if (temp.find(clientUserName) != std::string::npos)
		{
			std::cout << "USERNAME FIND IS " << temp << std::endl;
			std::cout << "CLIENT USERNAME IS " << clientUserName << std::endl;
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
	std::cout << BLUE3 << "--------------IM IN FILL VECTOR REQUEST----------------\n" << END;
	while (i < count)
	{
		std::string string_copy = tmp;
		std::string temp;
		std::cout << "string_copy IS " << string_copy << std::endl;
		token = string_copy.find('\n');
		if (token == std::string::npos)
		{
			std::cout << "IN FILL VECTOR REQUEST\n";
			std::cout << "Wrong request format. Please report to IRC's request format" << std::endl;
		
			temp = string_copy;
			std::cout << "TEMP IS " << temp << std::endl;
			this->M_requestVector.push_back(temp);
			temp.erase();
			i++;

		}
		else
		{
			temp = string_copy.substr(0, token);
			std::cout << "TEMP IS " << temp << std::endl;
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
	std::cout << RED << "--------------IM IN FILL CMD MAP----------------\n" << END;
	std::cout << "REQUEST VECTOR IS\n";
	std::vector<std::string>::iterator it = this->M_requestVector.begin();
	std::vector<std::string>::iterator ite = this->M_requestVector.end();
	while (it != ite)
	{
		std::cout << *it << std::endl;
		it++;
	}
	if (this->M_requestVector.empty())
	{
		std::cout << "Request vector empty" << std::endl;
		return (1);
	}
	std::string first;
	std::string second;
	std::vector<std::string> temp1;
	std::vector<std::string> temp2;
	it = this->M_requestVector.begin();
	ite = this->M_requestVector.end();
	std::cout << "C6\n";
	std::cout << YELLOW << "JE REMPLIS LA MAP\n" << END;
	std::map< std::string, std::vector<std::string> >::iterator mi_it;
	
	while (it != ite)
	{
		size_t space = it->find(' ');
		if (space == std::string::npos)
		{
			std::cout << "C7\n";
			std::cout << "REMPLI LA MAP SI CMD PAS DARGUMENTS\n";
			first = it->c_str();
			std::cout << "first = " << first << std::endl;
			//temp = split_string_v2(second, ' ');
			//temp1[0] = "";
			temp1.push_back("");
			temp1[0] = "";
			mi_it = this->M_cmdMap.find(first);
			if ((mi_it == this->M_cmdMap.end()) || first == "")
			{
				if (first != "")
				{
					std::cout << "C7.1\n";
					this->M_cmdMap[first] = temp1;
					int count = temp1.size();
					int i = 0;
					std::cout << "ARGV VECTOR AS " << count << " ELEMENTS\n";
					while (i < count)
					{
						std::cout << i <<" ARG IS " << temp1[i] << std::endl;
						i++;
					}
				}
				else
				{
					std::cout << "C7.2\n";
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
			std::cout << "C8\n";
			std::cout << "REMPLI LA MAP SI CMD ARGUMENT \n";
			first = it->substr(0, space);
			std::cout << "first = " << first << std::endl;
			second = it->substr(space + 1, it->size());
			std::cout << "second = " << second << std::endl;

			mi_it = this->M_cmdMap.find(first);
			if ((mi_it == this->M_cmdMap.end()) || first == "")
			{
				std::cout << "C8.1\n";
				if (first != "")
				{
					std::cout << "C8.2\n";
					temp2 = split_string_v2(second, ' ');
					int count = temp2.size();
					int i = 0;
					std::cout << "ARGV VECTOR AS " << count << " ELEMENTS\n";
					while (i < count)
					{
						std::cout << i <<" ARG IS " << temp2[i] << std::endl;
						i++;
					}
					this->M_cmdMap[first] = temp2;
				}
				else
				{
					std::cout << "C8.3\n";
					temp1.push_back("");
					temp1[0] = second;
					std::cout << "C8.5\n";
					this->M_cmdMap[first] = temp1;
				}
			}
			it++;
		}
		
	}

	std::map<std::string, std::vector<std::string> >::iterator m_it = this->M_cmdMap.begin();
	std::map<std::string, std::vector<std::string> >::iterator m_ite = this->M_cmdMap.end();
	std::cout << YELLOW << "JAFFICHE LA MAP REMPLIE\n" << END;
	while (m_it != m_ite)
	{
		std::cout << "C9\n";
		std::cout << "La map = [" << m_it->first << "] = ";
		for (size_t i = 0; i < m_it->second.size(); ++i)
		{
			std::cout << m_it->second[i] << " ";
		}
		std::cout << std::endl;
		m_it++;
	}

	std::cout << YELLOW << "JE GERE CTRL D\n" << END;
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
			std::cout << "C10\n";
			tempValue = m_it->second[0];
			std::cout << "TEMP VALUE IS " << tempValue << std::endl;
			while (m_it2 != m_ite2)
			{
				m_ite2--;
				std::cout << "C10.1\n";
				if (!containsAlphanumeric(m_ite2->second))
				{
					std::cout << "C10.2\n";
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
	std::cout << "AFTER PARSING IMRANE MAP IS \n";
	m_it = this->M_cmdMap.begin();
	m_ite = this->M_cmdMap.end();

	while (m_it != m_ite)
	{
		std::cout << "La map = [" << m_it->first << "] = ";
		for (size_t i = 0; i < m_it->second.size(); ++i)
		{
			std::cout << m_it->second[i] << " ";
		}
		std::cout << std::endl;
		m_it++;
	}
	
	std::cout << "C11\n";
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
	//std::cout << "Je suis ici et la" << std::endl;
	bool	toggle = false;
	std::string returnValue;
	std::cout << PURPLE << "IM IN CHOOSE AND EXECUTE\n" << END;
	std::cout << "c2.1\n";
	returnValue = "nothing launch";
	for (; m_it != m_ite; m_it++)
	{
		int i = 1;
		toggle = false;
		// std::string message = "On recoit bien le message";
		// if (send(clientFd, message.c_str(), message.length(), 0) < 0)
		// 	std::cout << "ERROR SENDING" << std::endl;
		std::vector<std::string>::iterator it = this->M_commands.begin();
		std::vector<std::string>::iterator ite = this->M_commands.end();
		std::cout << "La commande de MAP = " << m_it->first << std::endl;
		for (; it != ite; it++)
		{
			//std::map<std::string, std::string>::iterator it_found = this->M_cmdMap.find(*(it));
			
			//std::cout << "L'iterateur est = " << it_found->first <<std::endl;
			std::cout << "La commande de COMMANDS BRUTE = " << *it << std::endl;
			//if (it_found != this->M_cmdMap.end())
			if (m_it->first.find(*it) != std::string::npos)
			{

				std::cout << "c2.2\n";
				std::cout << "La commande à lancer = " << *it << std::endl;
				// std::cout << "La commande existe !" << std::endl;
				// std::cout << "Il s'agit de " << it_found->second << std::endl;
				toggle = true;
				//on lance la fonction switch, on lui passe i
				returnValue = executeCmd(i, clientFd);
				if (returnValue == "WRONG PASS")
					return ("WRONG PASS");
				std::cout << "c2.3\n";
				if (returnValue == "WRONG NICK")
					return ("WRONG NICK");
				std::cout << "c2.4\n";
				if (returnValue == "WRONG USER")
					return ("WRONG USER");
				std::cout << "c2.5\n";
				
			}
			if (toggle == true)
				break ;
			i++;
		}
	}
	std::cout << "c2.6\n";
	return (returnValue) ;
}

std::string	Server::executeCmd(int i, int clientFd)
{
	std::cout << BLUE1 << "IM IN EXCECUTE CMD\n" << END;
	switch (i)
	{
		case 1 :
		{
			std::cout << BLUE1 << "CASE 1\n" << END;
			client* clientPtr;
			std::string msg;
			std::string oldNick;

			// :haha!imrane@localhost NICK bob
			// peut être il va manquer le code 00x
			clientPtr = this->findClientBySocket(clientFd);
			oldNick = clientPtr->getNickName();
			std::cout << "On lance NICK" << std::endl;
			std::cout << "c2.1.1\n";
			std::string message = commandObj->NICK(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				std::cout << "c2.1.2\n";
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
				std::cout << "NICKNAME AFTER NICK IS " << (this->findClientBySocket(clientFd))->getNickName() << std::endl;
			}
			break ;
		}
		case 2 :
		{
			std::cout << BLUE1 << "CASE 2\n" << END;
			std::cout << "On lance PASS" << std::endl;
			std::string message = commandObj->PASS(clientFd, this);
			std::cout << "PASS lancé" << std::endl;
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG PASS");
			}
			break ;
		}
		case 3 :
		{
			std::cout << BLUE1 << "CASE 3\n" << END;
			std::cout << "On lance PASS" << std::endl;
			std::string message = commandObj->PASS(clientFd, this);
			std::cout << "PASS lancé" << std::endl;
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG PASS");
			}
			break ;
		}
		case 4 :
		{
			std::cout << BLUE1 << "CASE 4\n" << END;
			client* clientTmp;

			clientTmp = this->findClientBySocket(clientFd);
			std::cout << "On lance USER" << std::endl;
			std::cout << "c2.4.1\n";
			std::string message = commandObj->USER(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG USER");
			}
			
			std::cout << " -------USER INFO------ " << std::endl;
			std::cout << "USERNAME: " << clientTmp->getUserName() << std::endl;
			std::cout << "MODE: " << clientTmp->getMode() << std::endl;
			std::cout << "HOSTNAME: " << clientTmp->getHostName() << std::endl;
			std::cout << "REALNAME: " << clientTmp->getRealName() << std::endl;
			
			break ;
		}
		case 5 :
		{
			std::cout << "On lance MODE" << std::endl;
			client *client1 = this->findClientBySocket(clientFd);
			//int sendReturn = commandObj->MODE(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "MODE");
			if (sendReturn == -1)
			{
				std::cout << "Error sendind" << std::endl;
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				std::cout << "On va bien ici" << std::endl;
				return ("Error mode");
			}
			break ;
		}
		case 6 :
		{
			std::cout << BLUE1 << "CASE 6\n" << END;
			std::cout << "On lance PONG" << std::endl;
			std::string message = commandObj->PING(clientFd, this);
			i_send_message(clientFd,message);
			return ("PING");
			break ;
		}
		case 7 :
		{
			std::cout << BLUE1 << "CASE 7\n" << END;
			std::cout << "On lance QUIT" << std::endl;
			std::string message = commandObj->QUIT(clientFd, this);
			i_send_message(clientFd,message);
			break ;
		}
		case 8 :
		{
			std::cout << BLUE1 << "CASE 8\n" << END;
			std::cout << "On lance OPER" << std::endl;
			break ;
		}
		case 9 :
		{
			std::cout << BLUE1 << "CASE 9\n" << END;
			std::cout << "On lance JOIN" << std::endl;
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				std::cout << "Client doesn't exist" << std::endl;
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "JOIN");
			if (sendReturn == -1)
			{
				std::cout << "Error sending" << std::endl;
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				std::cout << "On va bien ici" << std::endl;
				return ("Error Joinin channel");
			}
			// if (message.find("nothing") == std::string::npos)
			// {
			// 	i_send_message(clientFd,message);
			// 	std::cout << "On va bien ici" << std::endl;
			// 	return ("WRONG USER");
			// }
			break ;
		}
		case 10 :
		{
			std::cout << BLUE1 << "CASE 10\n" << END;
			std::cout << "On lance PART" << std::endl;
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				std::cout << "Client doesn't exist" << std::endl;
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "PART");
			if (sendReturn == -1)
			{
				std::cout << "Error sending" << std::endl;
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				std::cout << "On va bien ici" << std::endl;
				return ("Error Joinin channel");
			}
			break ;
		}
		case 11 :
		{
			std::cout << BLUE1 << "CASE 11\n" << END;
			std::cout << "On lance PART" << std::endl;
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				std::cout << "Client doesn't exist" << std::endl;
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "PART");
			if (sendReturn == -1)
			{
				std::cout << "Error sending" << std::endl;
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				std::cout << "On va bien ici" << std::endl;
				return ("Error Joinin channel");
			}
			break ;
		}
		case 12 :
		{
			std::cout << BLUE1 << "CASE 12\n" << END;
			std::cout << "On lance KILL" << std::endl;
			break ;
		}
		case 13 :
		{
			std::cout << BLUE1 << "CASE 13\n" << END;
			std::cout << "On lance KILL" << std::endl;
			break ;
		}
		case 14 :
		{
			std::cout << BLUE1 << "CASE 14\n" << END;
			std::cout << "On lance NOTICE" << std::endl;
			break ;
		}
		case 15 :
		{
			std::cout << BLUE1 << "CASE 14\n" << END;
			std::cout << "On lance TOPIC" << std::endl;
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				std::cout << "Client doesn't exist" << std::endl;
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "TOPIC");
			if (sendReturn == -1)
			{
				std::cout << "Error sending" << std::endl;
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				std::cout << "On va bien ici" << std::endl;
				return ("Error Joinin channel");
			}
			break ;
		}
		case 16 :
		{
			std::cout << BLUE1 << "CASE 15\n" << END;
			std::cout << "On lance KICK" << std::endl;
			break ;
		}
		case 17 :
		{
			std::cout << BLUE1 << "CASE 16\n" << END;
			std::cout << "On lance PRIVMSG" << std::endl;
				client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				std::cout << "Client doesn't exist" << std::endl;
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "PRIVMSG");
			if (sendReturn == -1)
			{
				std::cout << "Error sending" << std::endl;
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				std::cout << "On va bien ici" << std::endl;
				return ("Error Joinin channel");
			}
			break ;
		}
		case 18 :
		{
			std::cout << BLUE1 << "CASE 17\n" << END;
			std::cout << "On lance PRIVMSG" << std::endl;
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				std::cout << "Client doesn't exist" << std::endl;
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "PRIVMSG");
			if (sendReturn == -1)
			{
				std::cout << "Error sending" << std::endl;
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				std::cout << "On va bien ici" << std::endl;
				return ("Error Joinin channel");
			}
			break ;
			break ;
		}
		case 19 :
		{
			std::cout << BLUE1 << "CASE 18\n" << END;
			std::cout << "On lance WALLOPS" << std::endl;
			break ;
		}
		case 20 :
		{
			std::cout << BLUE1 << "CASE 19\n" << END;
			client* clientTmp;

			clientTmp = this->findClientBySocket(clientFd);
			std::cout << "On lance USER HOST" << std::endl;
			std::string message = commandObj->USER(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG USER");
			}
			
			std::cout << " -------USER INFO------ " << std::endl;
			std::cout << "USERNAME: " << clientTmp->getUserName() << std::endl;
			std::cout << "MODE: " << clientTmp->getMode() << std::endl;
			std::cout << "HOSTNAME: " << clientTmp->getHostName() << std::endl;
			std::cout << "REALNAME: " << clientTmp->getRealName() << std::endl;
			
			break ;
		}
		case 21 :
		{
			std::cout << "On lance INVITE" << std::endl;
			client *client1 = this->findClientBySocket(clientFd);
			if (!client1)
			{
				std::cout << "Client doesn't exist" << std::endl;
				break ;
			}
			//int sendReturn = commandObj->JOIN(client1, this);
			int sendReturn = commandObj->handleCmd(client1, this, "INVITE");
			if (sendReturn == -1)
			{
				std::cout << "Error sending" << std::endl;
				return ("Error Joinin channel");
			}
			else if (sendReturn > 0)
			{
				std::cout << "On va bien ici" << std::endl;
				return ("Error Joinin channel");
			}
			break ;
		}
		default :
		{
			std::cout << BLUE1 << "CASE DEFAULT\n" << END;
			std::cout << "On ne doit pas rentrer ici normalement" << std::endl;
			//std::string message = "421  was not coded =)\r\n";
			//i_send_message(clientFd,message);
			break ;
		}
	}
	std::cout << BLUE1 << "END EXECUTE CMD\n" << END;
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
		std::cout << "Channel doesn't exist" << std::endl;
		return ;
	}
	this->M_listOfChannels.push_back(chan);
	std::list<channel *>::iterator it = this->M_listOfChannels.begin();
	std::list<channel *>::iterator ite = this->M_listOfChannels.end();
	while (it != ite)
	{
		std::cout << "le channel : " << (*it)->getName() << std::endl;
		std::cout << "le mot de passe : " << (*it)->getPassword() << std::endl;
		it++;
	}
	return ;
}

int	Server::addClientToChannel(client *client1, std::vector<std::string> temp)
{
	if (!client1)
	{
		std::cout << "Client doesn't exist" << std::endl;
		return (1);
	}
	// if (parameter.empty())
	// {
	// 	std::cout << "Channel name empty, please give a complete name" << std::endl;
	// 	return ;
	// }
	// std::cout << "On arrive bien jusque la" << std::endl;
	std::cout << YELLOW << "--------------------------I AM IN ADDCLIENTTOCHANNEL------------------------" << END << std::endl;
	std::string message;
	std::list<channel *>::iterator it = this->M_listOfChannels.begin();
	std::list<channel *>::iterator ite = this->M_listOfChannels.end();
	while (it != ite)
	{
		if ((*it)->getName().compare(temp[0]) == 0)
		{
			if ((*it)->getNbrOfClients() >= (*it)->getClientLimit()) //poser la question aux gars
			{
				std::cout << "Limit of client reach, Can't join channel" << std::endl;
				this->setChanName((*it)->getName());
				return (471);
			}
			std::cout << "Password in last = " << (*it)->getPassword() << std::endl;
			// std::cout << "Temp[1] in last = " << temp[1] << std::endl;
			std::cout << "resultat du empty : " << (*it)->getPassword().empty() << std::endl;
			if ((*it)->getPassword().empty() == 0 && temp.size() >= 2)
			{
				std::cout << "resultat du compare : " << (*it)->getPassword().compare(temp[1]) << std::endl;
				if ((*it)->getPassword().compare(temp[1]) == 0)
				{
					(*it)->addClientToTheChannel(client1);
					(*it)->increaseNbrCLient();
					(*it)->printMap();
					std::cout << "ON PASSE ICI DANS LE RAJOUT D'UN CLIENT AU CHAN AVC MDP" << std::endl;
					(*it)->welcomeInChanMessage(client1);
					if ((*it)->getTopic().empty() == 0)
					{
						message = RPL_TOPIC(client1->getNickName(), (*it)->getName(), (*it)->getTopic());
						send(client1->getsocketFd(), message.c_str(), message.length(), 0);
					}
					return (0);
				}
				else
				{
					this->setChanName((*it)->getName());
					return (475);
				}
			}
			else if ((*it)->getPassword().empty() && temp.size() < 2)
			{
				(*it)->addClientToTheChannel(client1);
				(*it)->increaseNbrCLient();
				(*it)->printMap();
				std::cout << "ON PASSE ICI DANS LE RAJOUT D'UN CLIENT AU CHAN SANS MDP" << std::endl;
				(*it)->welcomeInChanMessage(client1);
				if ((*it)->getTopic().empty() == 0)
				{
					message = RPL_TOPIC(client1->getNickName(), (*it)->getName(), (*it)->getTopic());
					send(client1->getsocketFd(), message.c_str(), message.length(), 0);
				}
				return (0);
			}
			else
			{
				this->setChanName((*it)->getName());
				return (475);
			}
				// std::cout << "Wrong password, can't join the channel" << std::endl;
			// std::cout << "C1" << std::endl;
			break ;
		}
		it++;
	}
	// std::cout << "C2" << std::endl;
	return (0);
}
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
	std::cout << "IM IN Handle CONNEXION\n";
	if (requestParsing(i) == 0)
	{
		//this->M_requestVector.clear();
		this->M_cmdMap.clear();
		std::cout << PURPLE << "MAP IS CLEAR\n" << END;
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
		// faut pas que je clear si j'ai rien exécuté
		std::string returnValue;
		returnValue = chooseAndExecuteAction(i);
		if (!(returnValue == "nothing launch" /*|| returnValue == "PING")*/))
		{
			//this->M_requestVector.clear();
			this->M_cmdMap.clear();
			std::cout << PURPLE << "MAP IS CLEAR\n" << END;
		}
		// this->M_args.clear();
	}
	// this->M_args.clear();
}
void	Server::eraseChannelFromList(channel *chan)
{
	std::list<channel *>::iterator found;
	found = std::find(this->M_listOfChannels.begin(), this->M_listOfChannels.end(), chan);
	if (found != this->M_listOfChannels.end())
	{
		delete (*found);
		this->M_listOfChannels.erase(found);
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
					// this is a new connexion
					// accept connexion
					i_accept_connexion();
					std::cout << "1 if = " << i << std::endl;
					addNewSocketToThePool(this->M_struct->clientSockFd);
					
				}
				else
				{
					//handle connexion
					//std::cout << "IM BACK to HANDLE REQ\n";
					std::cout << "fd socket client/serveur = " << i << std::endl;
					i_handle_request(i);
					//getSignal(2);
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
		std::cout << "ON TEST BIEN SIGANL(2)" << std::endl;
		memset(&sa, 0, sizeof(sa));
		// sa.sa_handler = &Server::handle_sigpipe_static;
		 sa.sa_handler = &Server::handle_sigint_static;
        serverInstance = this; 
		if (sigaction(SIGPIPE, &sa, 0) == -1)
		{
			perror("Error setting up SIGPIPE handler");
			return ;
		}
		std::cout << " SIGACTION SUCCESS" << std::endl;
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
		std::cout << "SIGPIPE received" << std::endl;
		// Votre logique de gestion de SIGPIPE ici
	}
	return ;
}

void	Server::handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "SIGINT received" << std::endl;
		this->M_working = false;
		//this->M_requestVector.clear();
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
		if (temp.find(clientNickname) != std::string::npos)
		{
			std::cout << "c2.1.1.3\n";
			std::cout << "NICK IS " << clientNickname << std::endl;
			std::cout << "client nick name found is " << (*it)->getNickName() << std::endl;
			return (*it);
		}
	}
	return (NULL);
}

void	Server::eraseClientFromList(std::string clientNickname)
{
	std::string temp;
	for (std::list<client *>::iterator it = this->listOfClients.begin(); it != this->listOfClients.end(); it++) {
		if (it != this->listOfClients.end())
		{
			temp = (*it)->getNickName();
			if (temp.find(clientNickname) != std::string::npos)
			{
				listOfClients.erase(it);
				//std::cout << "IM HERE\n";
				free(*it);
				//std::cout << "IM HERE 2\n";
				return ;
			}
		}
			
	}
}

const char *Server::WrongPortException::what() const throw()
{
	return ("Wrong port used. Please report to a port allowed by the IRC server. Port needed : 6667");
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


