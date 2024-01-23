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
	init_struct();
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
        std::cout << buff << std::endl;

        
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

void	Server::i_handle_request(int i)
{
	char buff[513];
    int nbCar;
	std::string buffStdStr;

    nbCar = read(i, buff, 512);
	buffStdStr = buff;
	//std::cout << "c1\n";
    if (nbCar == 0)
    {
		//std::cout << "c2\n";
		client * tempClient;
		close(i);
		FD_CLR(i, &(this->M_struct->current_sockets));
		tempClient = this->findClientBySocket(i);
		if (tempClient != NULL)
		{
			tempClient->goodBy();
			this->eraseClientFromList(tempClient->getNickName());
		}
    }
	else if (buffStdStr.find("QUIT") != std::string::npos)
	{
		// ok ya un moment particulier ou ça peut segfault
		// je crois c'est lié au fait que parfois ya pas d'info dans le client
		// voir pk ya pas d'info
		//std::cout << "c3\n";
		client * tempClient;
		close(i);
		FD_CLR(i, &(this->M_struct->current_sockets));
		//std::cout << "c3.1\n";
		tempClient = this->findClientBySocket(i);
		//std::cout << "c3.2\n";
		if (tempClient != NULL)
			tempClient->goodBy();
		//std::cout << "c3.3\n";
		if (tempClient != NULL)
			this->eraseClientFromList(tempClient->getNickName());
		//std::cout << "c3.4\n";
		//std::cout << "I ERASE\n";
	}
    else
    {
        std::cout << "IM HERE IN HANDLE REQUEST\n";
        buff[nbCar] = 0;
    
        std::cout << "nb car is " << nbCar << std::endl;
        std::cout << buff << std::endl;

        memset(buff, 0, 513);
    }
    
    // le pong doit ^petre envoyé avant 301 seconds
}



void	Server::mainProgram(void)
{
	while (true)
	{
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
					addNewSocketToThePool(this->M_struct->clientSockFd);
					
				}
				else
				{
					//handle connexion
					//std::cout << "IM BACK to HANDLE REQ\n";
					i_handle_request(i);
				}
			}
		}
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
	perror("FUNCTION REAL ERROR here is why error socket\n");
	return ("Error : Socket Fd failed.");
}

const char *Server::ErrorBindingException::what() const throw()
{
	perror("FUNCTION REAL ERROR here is why error binding\n");
	return ("Error : Binding socket failed.");
}

const char *Server::ErrorListenException::what() const throw()
{
	perror("FUNCTION REAL ERROR here is why error listen\n");
	return ("Error : Set socket in listen mode failed.");
}

const char *Server::WrongClientSocketFdException::what() const throw()
{
	perror("FUNCTION REAL ERROR here is why error socket\n");
	return ("Client fd error. Can't connect with server");
}

const char *Server::SelectFunctionErrorException::what() const throw()
{
	perror("FUNCTION REAL ERROR here is why error select\n");
	return ("An error occured with select function");
}
