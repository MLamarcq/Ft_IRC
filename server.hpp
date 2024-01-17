#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <string>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h> 
#include <sys/types.h>  
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_serv
{
	int					socket_fd;
	int					clientSockFd;
	struct sockaddr_in	sockStructServ;
	struct sockaddr_in	sockStructClient;
	fd_set current_sockets, ready_sockets;

} t_serv;

class Server
{
	public :

			Server(std::string const &port, std::string const &pass_wd);
			Server(Server const &src);
			~Server(void);

			Server & operator=(Server const &rhs);

			std::string const	&getPort(void) const;
			std::string const	&getPass_Wd(void) const;
			void				getStruct(void) const;

			void				init_struct(void);


	private :

			Server(void);
			std::string const	M_port;
			std::string const	M_pass_wd;
			t_serv				&M_struct;

};

#endif