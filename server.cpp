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
	this->M_struct->socket_fd = 0;
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
	this->M_struct->socket_fd = rhs.M_struct->socket_fd;
	this->M_struct->clientSockFd = rhs.M_struct->clientSockFd;
	this->M_struct->sockStructServ.sin_family = rhs.M_struct->sockStructServ.sin_family;
	this->M_struct->sockStructServ.sin_addr.s_addr = rhs.M_struct->sockStructServ.sin_addr.s_addr;
	this->M_struct->sockStructServ.sin_port = rhs.M_struct->sockStructServ.sin_port;
	this->M_struct->current_sockets = rhs.M_struct->current_sockets;
	return ;
}

void	Server::Setup_Socket(void)
{
	this->M_struct->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->M_struct->socket_fd < 0)
	{
		throw(WrongSocketFdEexception());
	}
	if (bind(this->M_struct->socket_fd, reinterpret_cast<struct sockaddr*>(&this->M_struct->sockStructServ), sizeof(this->M_struct->sockStructServ)) < 0)
	{
		throw(ErrorBindingException());
	}
	if (listen(this->M_struct->socket_fd, 100) < 0)
	{
		throw(ErrorListenException());
	}
	addNewSocketToThePool(this->M_struct->socket_fd);
	return ;
}

void	Server::addNewSocketToThePool(int new_socket) const
{
	FD_SET(new_socket, &this->M_struct->current_sockets);
	return ;
}


int		Server::send_message(int clientSockFd, std::string message) const
{
	return (write(clientSockFd, message.c_str(), message.length()));
}


void	Server::accept_connexion(void)
{
	socklen_t clientLen;

	clientLen = sizeof(this->M_struct->sockStructClient);
	std::cout << "waiting for a request..." << std::endl;
	this->M_struct->clientSockFd = accept(this->M_struct->socket_fd, reinterpret_cast<struct sockaddr*>(&this->M_struct->sockStructClient), &clientLen);
	if (this->M_struct->clientSockFd < 0)
	{
		throw (WrongClientSocketFdException());
	}
	std::cout << "I have accepted your request !" << std::endl;
	return ;
}

void	Server::handle_connexion(void)
{
	char	buff[513];
	int		nbCar;

	nbCar = read(this->M_struct->clientSockFd, buff, 512);

	std::cout << "nb car = " << nbCar << std::endl;
	std::cout << buff << std::endl;

	memset(buff, 0, 513);

	/* Write a response to the client */
	nbCar = send_message(this->M_struct->clientSockFd,":Welcome to the Internet Relay Network haha!imrane@localhost\r\n");
	
	close(this->M_struct->clientSockFd);
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
				if (i == this->M_struct->socket_fd)
				{
					// this is a new connexion
					// accept connexion
					accept_connexion();
					addNewSocketToThePool(this->M_struct->clientSockFd);
				}
				else
				{
					//handle connexion
					handle_connexion();
					FD_CLR(i, &this->M_struct->current_sockets);
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
	return ("Error : Binding socker failed.");
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