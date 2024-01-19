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
#include <cstring>

typedef struct s_serv
{
	int					socket_fd;
	int					clientSockFd;
	struct sockaddr_in	sockStructServ;
	struct sockaddr_in	sockStructClient;
	fd_set				current_sockets; 
	fd_set				temp_sockets;

} t_serv;

class Server
{
	public :

			Server(std::string port, std::string pass_wd);
			Server(Server const &src);
			~Server(void);

			Server & operator=(Server const &rhs);

			//Getters and init constructor
			std::string			getPort(void) const;
			std::string			getPass_Wd(void) const;
			void				Copy_Struct(Server const &rhs);
			void				init_struct(void);

			//All about socket
			void				Setup_Socket(void);
			void				addNewSocketToThePool(int new_socket) const;

			//Connextion between new request/accepted request and socket
			int					send_message(int clientSockFd, std::string message) const;
			void				accept_connexion(void);
			void				handle_connexion(void);

			//Main program
			void				mainProgram(void);

			//Exception for error handling
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
			
			class WrongSocketFdEexception : public std::exception
			{
				public :
						const char *what() const throw();
			};

			class ErrorBindingException : public std::exception
			{
				public :
						const char *what() const throw();
			};

			class ErrorListenException : public std::exception
			{
				public :
						const char *what() const throw();
			};

			class WrongClientSocketFdException : public std::exception
			{
				public :
						const char *what() const throw();
			};

			class SelectFunctionErrorException : public std::exception
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