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
	char buff[513];
    int nbCar;

    nbCar = read(this->M_struct->clientSockFd, buff, 512);
    if (nbCar == 0)
    {
        close(this->M_struct->clientSockFd);
        std::cout << "i close the connection for the sock" << this->M_struct->clientSockFd << "\n";
    }
    else
    {
        buff[nbCar] = 0;
    
        std::cout << "nb car is " << nbCar << std::endl;
        std::cout << buff << std::endl;

        memset(buff, 0, 513);
        nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 421 * LS :Unknown command\r\n");
        nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 001 haha :Welcome to the Internet Relay Network haha!imrane@localhost\r\n");
        nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 002 haha :Your host is localhost, running version 1.0\r\n");
        nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 003 haha :This server was created 01/01/24\r\n");
        nbCar = i_send_message(this->M_struct->clientSockFd,":localhost 004 haha localhost 1.0\r\n");
    }
}
void	Server::i_handle_request(int i)
{
	char buff[513];
    int nbCar;

    nbCar = read(i, buff, 512);
    if (nbCar == 0)
    {
        close(i);
        std::cout << "i close the connection for the sock" << i << "\n";
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
					i_handle_request(i);
				}
			}
		}
	}
	return ;
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
	return ("Error : Socket Fd failed.");
}

const char *Server::ErrorBindingException::what() const throw()
{
	return ("Error : Binding socket failed.");
}

const char *Server::ErrorListenException::what() const throw()
{
	return ("Error : Set socket in listen mode failed.");
}

const char *Server::WrongClientSocketFdException::what() const throw()
{
	return ("Client fd error. Can't connect with server");
}

const char *Server::SelectFunctionErrorException::what() const throw()
{
	return ("An error occured with select function");
}