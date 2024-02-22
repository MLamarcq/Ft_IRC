#include "server.hpp"

Server* Server::serverInstance = NULL;

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		return (0);
	}
	std::string port(argv[1]);
	std::string pass_wd(argv[2]);
	signal(SIGTSTP, SIG_IGN);
	try
	{
		Server server(port, pass_wd);
		server.Setup_Socket();
		server.mainProgram();
	}
	catch (std::exception &e)
	{
		return (0);
	}
	return (0);
}
