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

std::string Server::getPort(void) const
{
	return (this->M_port);
}

std::string Server::getPass_Wd(void) const
{
	return (this->M_pass_wd);
}

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
	this->M_commands.push_back("NICK");
	this->M_commands.push_back("PASS");
	this->M_commands.push_back("USER");
	this->M_commands.push_back("MODE");
	this->M_commands.push_back("PING");
	this->M_commands.push_back("QUIT");
	this->M_commands.push_back("OPER");
	this->M_commands.push_back("JOIN");
	this->M_commands.push_back("PART");
	this->M_commands.push_back("KILL");
	this->M_commands.push_back("NOTICE");
	this->M_commands.push_back("TOPIC");
	this->M_commands.push_back("KICK");
	this->M_commands.push_back("PRIVMSG");
	this->M_commands.push_back("WALLOPS");
	this->M_commands.push_back("userhost");
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
	//std::cout << "New connection , socket fd is " << this->M_struct->clientSockFd << "ip is : " << inet_ntoa(this->M_struct->sockStructClient.sin_addr) << " , port : " << ntohs(this->M_struct->sockStructClient.sin_port) << std::endl;
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
	//quand ya pas de connexion et que j'envoi quit
	// je tombe sur une boucle infi
	// je crois que c'est parce que il n'y a pas d'utilisateur rattaché

	// ok fonctionne mais ne set pas les var avec ce que j'ai envoyé
	// voir pk il n'y a pas le nickname et le hostname
		// set yet Unauthorized command (already registered)
		// en gros il prend pas le nom du buff pour mettre mais celui que g enregistré avant
	// après essayer de se connecter avec le même nom
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
	clientPtr->setIp(inet_ntoa(this->M_struct->sockStructClient.sin_addr));
	clientPtr->setPort(ntohs(this->M_struct->sockStructClient.sin_port));
	//ajouter le client dans la liste
	this->listOfClients.push_back(clientPtr);
	
	std::cout << "c1\n";
	if (requestParsing(this->M_struct->clientSockFd) == 0)
		return ;
	std::cout << "c2\n";
	std::string returnValue = chooseAndExecuteAction(this->M_struct->clientSockFd);
	if (returnValue == "WRONG PASS" || returnValue == "WRONG USER" || returnValue == "WRONG NICK")
	{
		std::cout << "c3\n";
		std::cout << returnValue << std::endl;
		this->M_requestVector.clear();
		this->M_cmdMap.clear();
		this->eraseClientFromList(clientPtr->getNickName());
		return ;
	}
	this->M_requestVector.clear();
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
	std::cout << "c1.1\n";
	reader = read(ClientFd, buff, 512);
	if (reader == -1)
	{
		std::cout << "Error reading. Please check client socket." << std::endl;
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
		find_r = tmp.find('\r', find_n + 1);
		find_n = tmp.find('\n', find_n + 1);
		if (find_r == std::string::npos || find_n == std::string::npos)
		{
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
	std::cout << "c1.5\n";
	return (1);
}

client*	Server::findClientByUserName(std::string clientUserName)
{
	std::string temp;
	for (std::list<client *>::iterator it = this->listOfClients.begin(); it != this->listOfClients.end(); it++) {
		temp = (*it)->getUserName();
		if (temp.find(clientUserName) != std::string::npos)
			return (*it);
	}
	return (NULL);
}


void				Server::sendWelcomeMessage(client* clientPtr)
{
	i_send_message(this->M_struct->clientSockFd,":localhost 001 " + clientPtr->getNickName() + " :Welcome to the Internet Relay Network " + clientPtr->getNickName()+"!" + clientPtr->getUserName() + "@localhost\r\n");
	i_send_message(this->M_struct->clientSockFd,":localhost 002 "+ clientPtr->getNickName() +  " :Your host is localhost, running version 1.0\r\n");
	i_send_message(this->M_struct->clientSockFd,":localhost 003 "+ clientPtr->getNickName() + " :This server was created 01/01/24\r\n");
	i_send_message(this->M_struct->clientSockFd,":localhost 004 " + clientPtr->getNickName() + " localhost 1.0\r\n");
	clientPtr->setWelcomeMessageSent(true);
	clientPtr->hello();
}

int	Server::fillVectorRequest(int count, std::string tmp)
{
	size_t token = 0;
	int i = 0;
	//std::cout << "c1.3.1\n";
	while (i < count)
	{
		//std::cout << "c1.3.2\n";
		//std::cout << "TMP IS "<< tmp << std::endl;
		std::string string_copy = tmp;
		std::string temp;
		token = string_copy.find('\n');
		if (token == std::string::npos)
		{
			std::cout << "Wrong request format. Please report to IRC's request format" << std::endl;
			return (0);
		}
		temp = string_copy.substr(0, token);
		//std::cout << "TEMP IS " << temp << std::endl;
		this->M_requestVector.push_back(temp);
		temp.erase();
		//std::cout << "TEMP AFTER ERASE IS " << temp << std::endl;
		//original
			//string_copy.erase(0, token);
		tmp = tmp.substr(token + 1, tmp.size());
		i++;
	}
	// std::vector<std::string>::iterator ite = this->M_requestVector.end();
	// for (std::vector<std::string>::iterator it = this->M_requestVector.begin(); it != ite; it++)
	// 	std::cout << "vector = " << *it << std::endl;
	//std::cout << "c1.3.3\n";
	return (1);
}


int	Server::fillCmdMap(void)
{
	if (this->M_requestVector.empty())
	{
		std::cout << "Request vector empty" << std::endl;
		return (0);
	}
	std::string first;
	std::string second;
	std::vector<std::string>::iterator it = this->M_requestVector.begin();
	std::vector<std::string>::iterator ite = this->M_requestVector.end();
	while (it != ite)
	{
		size_t space = it->find(' ');
		if (space == std::string::npos)
		{
			std::cout << "Wrong request format. Please report to IRC's request format" << std::endl;
			return (0);
		}
		first = it->substr(0, space);
		std::cout << "first = " << first << std::endl;
		second = it->substr(space, it->size());
		std::cout << "second = " << second << std::endl;
		this->M_cmdMap[first] = second;
		it++;
	}
	std::map<std::string, std::string>::iterator m_it = this->M_cmdMap.begin();
	std::map<std::string, std::string>::iterator m_ite = this->M_cmdMap.end();
	while (m_it != m_ite)
	{
		std::cout << "La map = [" << m_it->first << "] = ";
		std::cout << m_it->second << std::endl;
		m_it++;
	}
	return (1);
}

std::string	Server::chooseAndExecuteAction(int clientFd)
{
	std::map<std::string, std::string>::iterator m_it = this->M_cmdMap.begin();
	std::map<std::string, std::string>::iterator m_ite = this->M_cmdMap.end();
	std::cout << "Je suis ici et la" << std::endl;
	bool	toggle = false;
	std::string returnValue;
	std::cout << "c2.1\n";
	for (; m_it != m_ite; m_it++)
	{
		int i = 1;
		toggle = false;
		std::vector<std::string>::iterator it = this->M_commands.begin();
		std::vector<std::string>::iterator ite = this->M_commands.end();
		std::cout << "La commande de MAP = " << m_it->first << std::endl;
		for (; it != ite; it++)
		{
			//std::map<std::string, std::string>::iterator it_found = this->M_cmdMap.find(*(it));
			
			//std::cout << "L'iterateur est = " << it_found->first <<std::endl;
			std::cout << "La commande de COMMANDS BRUTE = " << *it << std::endl;
			//if (it_found != this->M_cmdMap.end())
			if (m_it->first.find(*it)!= std::string::npos)
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
	return (" ") ;
}

std::string	Server::executeCmd(int i, int clientFd)
{
	
	switch (i)
	{
		case 1 :
		{
			std::cout << "On lance NICK" << std::endl;
			std::cout << "c2.1.1\n";
			std::string message = commandObj->NICK(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				std::cout << "c2.1.2\n";
				i_send_message(clientFd,message);
				return ("WRONG NICK");
			}
			//std::cout << "NICKNAME AFTER NICK IS " << (this->findClientBySocket(clientFd))->getNickName() << std::endl;
			break ;
		}
		case 2 :
		{
			std::cout << "On lance PASS" << std::endl;
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
			//client* clientTmp;

    		//clientTmp = this->findClientBySocket(clientFd);
			std::cout << "On lance USER" << std::endl;
			std::cout << "c2.4.1\n";
			std::string message = commandObj->USER(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG USER");
			}
			/*
			std::cout << " -------USER INFO------ " << std::endl;
			std::cout << "USERNAME: " << clientTmp->getUserName() << std::endl;
			std::cout << "MODE: " << clientTmp->getMode() << std::endl;
			std::cout << "HOSTNAME: " << clientTmp->getHostName() << std::endl;
			std::cout << "REALNAME: " << clientTmp->getRealName() << std::endl;
			*/
			break ;
		}
		case 4 :
		{
			std::cout << "On lance MODE" << std::endl;
			break ;
		}
		case 5 :
		{
			std::cout << "On lance PING" << std::endl;
			break ;
		}
		case 6 :
		{
			std::cout << "On lance QUIT" << std::endl;
			commandObj->QUIT(clientFd, this);
			break ;
		}
		case 7 :
		{
			std::cout << "On lance OPER" << std::endl;
			break ;
		}
		case 8 :
		{
			std::cout << "On lance JOIN" << std::endl;
			break ;
		}
		case 9 :
		{
			std::cout << "On lance PART" << std::endl;
			break ;
		}
		case 10 :
		{
			std::cout << "On lance KILL" << std::endl;
			break ;
		}
		case 11 :
		{
			std::cout << "On lance NOTICE" << std::endl;
			break ;
		}
		case 12 :
		{
			std::cout << "On lance TOPIC" << std::endl;
			break ;
		}
		case 13 :
		{
			std::cout << "On lance KICK" << std::endl;
			break ;
		}
		case 14 :
		{
			std::cout << "On lance PRIVMSG" << std::endl;
			break ;
		}
		case 15 :
		{
			std::cout << "On lance WALLOPS" << std::endl;
			break ;
		}
		case 16 :
		{
			//client* clientTmp;

    		//clientTmp = this->findClientBySocket(clientFd);
			std::cout << "On lance USER HOST" << std::endl;
			std::string message = commandObj->USER(clientFd, this);
			if (message.find("nothing") == std::string::npos)
			{
				i_send_message(clientFd,message);
				return ("WRONG USER");
			}
			/*
			std::cout << " -------USER INFO------ " << std::endl;
			std::cout << "USERNAME: " << clientTmp->getUserName() << std::endl;
			std::cout << "MODE: " << clientTmp->getMode() << std::endl;
			std::cout << "HOSTNAME: " << clientTmp->getHostName() << std::endl;
			std::cout << "REALNAME: " << clientTmp->getRealName() << std::endl;
			*/
			break ;
		}
		default :
		{
			std::cout << "On ne doit pas rentrer ici normalement" << std::endl;
			break ;
		}
	}
	return ("nothing");
}

void	Server::i_handle_request(int i)
{
	std::cout << "IM IN Handle CONNEXION\n";
	if (requestParsing(i) == 0)
	{
		this->M_requestVector.clear();
		this->M_cmdMap.clear();
		close(i);
		FD_CLR(i, &(this->M_struct->current_sockets));
		return ;
	}
	chooseAndExecuteAction(i);
	
	this->M_requestVector.clear();
	this->M_cmdMap.clear();
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
	return ;
}

void Server::handle_sigint_static(int signal)
{
	if (serverInstance)
		serverInstance->handle_sigint(signal);
}

void	Server::handle_sigint(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "SIGINT received" << std::endl;
		this->M_working = false;
		this->M_requestVector.clear();
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


