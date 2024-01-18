#include "server.hpp"

Server::Server(void)
{
	return ;
}

Server::Server(std::string port, std::string pass_wd)
{
	// if (checkPass_Wd(pass_wd) == 0)
	// 	throw(WrongPasswordException());
	// if (checkPort(port) == 0)
	// 	throw(WrongPortException());
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
		//Copy_struct(rhs);
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
	// std::cout << "Socket			t_serv					*M_struct; fd = " << this->M_struct->socket_fd << std::endl;
	// std::cout << "Client socket fd = " << this->M_struct->clientSockFd << std::endl;
	// bzero((char *)sockStructServ, sizeof(*(sockStructServ)));
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

int	Server::set
// void	Server::Copy_struct(Server const &rhs)
// {
// 	if (this->M_struct)
// 		delete this->M_struct;
// 	this->M_struct = new t_serv;
// 	this->M_struct->socket_fd = rhs->M_struct->socket_fd;
// 	this->M_struct->clientSockFd = rhs->M_struct->clientSockFd;
// 	return ;
// }

// int	Server::checkPort(std::string port) const
// {
// 	if (port.empty())
// 		return (0);
// 	if (is_digit(port, true) == 0)
// 		return (0);
// 	return (1);
// }

// int	Server::checkPass_Wd(std::string pass_wd) const
// {
// 	if (pass_wd.empty())
// 		return (0);
// 	if (is_digit(pass_wd, false) == 0)
// 		return (0);
// 	return (1);
// }

// int Server::is_digit(std::string str, bool check) const
// {
// 	int i = 0;
// 	while (str[i])
// 	{
// 		if ((str[i] >= '0' && str[i] <= '9') && check == true)
// 			i++;
// 		else if (str[i] >= '0' && str[i] <= '9' && check == false)
// 			i++;
// 		else
// 			return (0);
// 	}
// 	return (1);
// }