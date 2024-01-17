#include "server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments. We need 3 arguments : ";
		std::cout << "./irc <a_port> <password>" << std::endl;
		return (0);
	}
	std::string port(argv[1]);
	std::string pass_wd(argv[2]);
	
	try
	{
		Server server(port, pass_wd);
		std::cout << "The port is : " << server.getPort() << std::endl;
		std::cout << "The password is : " << server.getPass_Wd() << std::endl;
	}
	catch (Server::WrongPasswordException &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	catch (Server::WrongPortException &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	return (0);
}