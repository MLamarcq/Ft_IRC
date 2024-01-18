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
	fd_set				current_sockets; 
	fd_set				ready_sockets;

} t_serv;

class Server
{
	public :

			Server(std::string port, std::string pass_wd);
			Server(Server const &src);
			~Server(void);

			Server & operator=(Server const &rhs);

			std::string			getPort(void) const;
			std::string			getPass_Wd(void) const;
			// void				getStruct(void) const;

			 void				init_struct(void);

			// int 				is_digit(std::string str, bool check) const;
			// int					checkPort(std::string port) const;
			// int					checkPass_Wd(std::string port) const;



			class WrongPortException : public std::exception
			{
				public :
						const char *what() const throw();
			};

			class WrongPasswordException : public std::exception
			{
				public :
						const char *what() const throw();
			};
	
	private :

			Server(void);
			std::string				M_port;
			std::string				M_pass_wd;
			t_serv					*M_struct;

};

#endif