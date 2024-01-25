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
	this->M_commands.push_back("userhost");
	this->M_commands.push_back("MODE");
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
	char buff[513];
	std::string nickname;
	std::string realName;
	std::string userName;
	std::string hostName;
	int nbCar;

	nbCar = read(this->M_struct->clientSockFd, buff, 512);
	if (nbCar == 0)
	{
		close(this->M_struct->clientSockFd);
		std::cout << "i close the connection for the sock" << this->M_struct->clientSockFd << "\n";
	}
	else
	{
		client * clientPtr;
		buff[nbCar] = 0;
	
		std::cout << "nb car is " << nbCar << std::endl;
		std::cout << "buff =" << buff << std::endl;

		
		// créer le client
		clientPtr = this->createClient();
		// remplir les infos du client
		clientPtr->setIp(inet_ntoa(this->M_struct->sockStructClient.sin_addr));
		clientPtr->setPort(ntohs(this->M_struct->sockStructClient.sin_port));
		clientPtr->fillStrParam(buff, clientPtr);
		clientPtr->setsocketFd(this->M_struct->clientSockFd);
		
		//ajouter le client dans la liste
		this->listOfClients.push_back(clientPtr);
		//nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 421 * LS :Unknown command\r\n");
		nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 001 " + clientPtr->getNickName() + " :Welcome to the Internet Relay Network " + clientPtr->getNickName()+"!" + clientPtr->getUserName() + "@localhost\r\n");
		nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 002 "+ clientPtr->getNickName() +  " :Your host is localhost, running version 1.0\r\n");
		nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 003 "+ clientPtr->getNickName() + " :This server was created 01/01/24\r\n");
		nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 004 " + clientPtr->getNickName() + " localhost 1.0\r\n");
		clientPtr->setWelcomeMessageSent(true);
		clientPtr->hello();
		memset(buff, 0, 513);
	}

}

void	Server::requestParsing(int ClientFd)
{
	char	buff[513];
	int		reader;

	reader = read(ClientFd, buff, 512);
	if (reader == -1)
	{
		// throw(ErrorReadingException());
		return ;
	}
	// std::cout << "c1" << std::endl;
	buff[reader] = '\0';
	int i = 0;
	int count = 0;
	// std::cout << "buff = " << buff << std::endl;
	std::string tmp(buff);
	// std::cout << "tmp = " << tmp << std::endl;
	int size  = tmp.size();
	bool toggle = false;
	while (i < size)
	{	
		if (isAlpha(tmp[i]) == 1 && toggle == false)
		{
			// std::cout << "On trouve une majuscule" << std::endl;
			while (tmp[i] != ' ')
				i++;
			count++;
			toggle = true;
		} // pas fou
		// else 
		// {
		// 	std::cout << "Wrong request format" << std::endl;
		// 	return ;
		// }
		i++;
	}
	// std::cout << "count = "  << count << std::endl;
	// std::cout << "c3" << std::endl;
	std::string string_copy = tmp;
	std::string temp;
	size_t		token = 0;
	i = 0;
	while (i < count)
	{
		token = string_copy.find('\r');
		//std::cout << "Premier token = " << token << std::endl;
		if (token == std::string::npos)
		{
			std::cout << "Wrong request format, need '\r'" << std::endl;
			return ;
		}
		token = string_copy.find('\n');
		//std::cout << "token saut de ligne = " << token << std::endl;
		if (token == std::string::npos)
		{
			std::cout << "Wrong request format, need '\n'" << std::endl;
			return ;
		}
		temp = string_copy.substr(0, token);
		// std::cout << "temp = " << temp << std::endl;
		this->M_requestVector.push_back(temp);
		// std::cout << "C4" << std::endl;
		// std::vector<std::string>::iterator ite = this->M_requestVector.end();
		// for (std::vector<std::string>::iterator it = this->M_requestVector.begin(); it != ite; it++)
			// std::cout << "vector = " << *it << std::endl;
		temp.erase();
		string_copy.erase(0, token);
		i++;
	}
	return ;
}

void	Server::chooseAction(void)
{
	std::vector<std::string>::iterator itc = this->M_commands.begin();
	std::vector<std::string>::iterator itec = this->M_commands.end();
	size_t	token = 0;
	bool	toggle = false;
	std::cout << "Je suis ici et la" << std::endl;
	for (; itc != itec; itc++)
	{
		std::cout << "Je rentre la" << std::endl;
		std::vector<std::string>::iterator it = this->M_requestVector.begin();
		std::vector<std::string>::iterator ite = this->M_requestVector.end();
		std::cout << "Dans le vecteur : " << *it << std::endl;
		// token = it->find("NICK");
		for (; it != ite; it++)
		{
			token = it->find(*(itc));
			if (token != std::string::npos)
			{
				std::cout << "La valeur est dans le vecteur" << std::endl;
				toggle = true;
				break ;
			}
		}
		if (toggle == true)
			break ;
	}
	std::cout << "token du vecteur = " << token << std::endl << std::endl;
	return ;
}


void	Server::i_handle_request(int i)
{
	requestParsing(i);
	chooseAction();
	this->M_requestVector.clear();
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
			return (*it);
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


// std::string returnLine()