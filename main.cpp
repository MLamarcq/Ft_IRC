#include "server.hpp"

Server* Server::serverInstance = NULL;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments. We need 3 arguments : ";
		std::cout << "./irc <port> <password>" << std::endl;
		return (0);
	}
	std::string port(argv[1]);
	std::string pass_wd(argv[2]);
	//signal(SIGTSTP, SIG_IGN);
	try
	{
		Server server(port, pass_wd);
		std::cout << "The port is : " << server.getPort() << std::endl;
		std::cout << "The password is : " << server.getPass_Wd() << std::endl;
		server.Setup_Socket();
		server.mainProgram();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	return (0);
}
