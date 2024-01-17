#include "server.hpp"

Server::Server(void)
{
	return ;
}

Server::Server(std::string const &port, std::string const &pass_wd)
{
	this->M_port = port;
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
		Copy_struct(rhs);
	}
	return (*this);
}

Server::~Server(void)
{
	if (this->M_struct)
		delete this->M_struct;
	return ;
}

std::string const Server::getPort(void) const
{
	return (this->M_port);
}

std::string const Server::getPass_Wd(void) const
{
	return (this->M_pass_wd);
}

void	Server::init_struct(void)
{
	this->M_struct = new t_serv;
	this->M_struct->socket_fd = 0;
	this->M_struct->clientSockFd = 0;
	bzero((char *)sockStructServ, sizeof(*(sockStructServ)));
	return ;
}

void	Server::Copy_struct(Server const &rhs)
{
	if (this->M_struct)
		delete this->M_struct;
	this->M_struct = new t_serv;
	this->M_struct->socket_fd = rhs->M_struct->socket_fd;
	this->M_struct->clientSockFd = rhs->M_struct->clientSockFd;
	
}