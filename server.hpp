#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
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
#include <arpa/inet.h>
#include "client.hpp"
#include "ft_split.hpp"
#include <vector>
#include "colors.hpp"
#include <signal.h>
#include <map>
#include <algorithm>
#include <string>
#include <iomanip>

#include "command.hpp"

typedef struct s_serv
{
	int					serveurSockFd;
	int					clientSockFd;
	struct sockaddr_in	sockStructServ;
	struct sockaddr_in	sockStructClient;
	fd_set				current_sockets; 
	fd_set				temp_sockets;

} t_serv;

class client;
class command;
class Server
{
	public :

			Server(std::string port, std::string pass_wd);
			Server(Server const &src);
			~Server(void);

			Server & operator=(Server const &rhs);
			
			//Essai pour les signaux
			static Server* serverInstance;
			
			// about client
			int nbofClients;
			client* createClient();
			std::list<client *> listOfClients;
			client* findClientBySocket(int clientSocketFd);
			client*	findClientByNickName(std::string clientNickname);
			void	eraseClientFromList(std::string clientNickname);
			client*	findClientByUserName(std::string clientUserName);
			void	sendWelcomeMessage(client* clientPtr);



			//Getters and init constructor
			std::string			getPort(void) const;
			std::string			getPass_Wd(void) const;
			void				Copy_Struct(Server const &rhs);
			void				init_struct(void);
			void				fill_commands_vector(void);

			//All about socket
			void				Setup_Socket(void);
			void				addNewSocketToThePool(int new_socket) const;
			void				closeSocket(void);

			//Connextion between new request/accepted request and socket
			int					i_send_message(int clientSockFd, std::string message) const;
			void				i_accept_connexion(void);
			void				i_handle_request(int i);
			void				i_handle_first_connexion(void);

			//Main program
			void				mainProgram(void);

			//Parsing functions
			int					isAlpha(char c);
			int					requestParsing(int ClientFd);
			int					fillVectorRequest(int count, std::string tmp);
			int					fillCmdMap(void);
			std::string			executeCmd(int i, int clientFd);
			std::string				chooseAndExecuteAction(int clientFd);


			//Handle Signal
			void				getSignal(int index);
			void				handle_sigint(int signal);
			static void			handle_sigint_static(int signal);

			//geter for la structure

			//seter for la structure

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

		t_serv								*M_struct;
		std::map<std::string, std::string>	M_cmdMap;
		std::string							M_pass_wd;
	
	private :

			Server(void);
			std::string							M_port;
			std::vector<std::string>			M_requestVector;
			std::vector<std::string>			M_commands;
			bool								M_working;
			//t_serv								*M_struct;
			command								*commandObj;
};

std::string			intTostring(int number);


#endif