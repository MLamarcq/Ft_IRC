void	Server::i_handle_request(int i)
{
	// char buff[513];
	// int nbCar;
	// std::string buffStdStr;

	// nbCar = read(i, buff, 512);
	// buffStdStr = buff;
	// //std::cout << "c1\n";
	// if (nbCar == 0)
	// {
	// 	//std::cout << "c2\n";
	// 	client * tempClient;
	// 	close(i);
	// 	FD_CLR(i, &(this->M_struct->current_sockets));
	// 	tempClient = this->findClientBySocket(i);
	// 	if (tempClient != NULL)
	// 	{
	// 		tempClient->goodBy();
	// 		this->eraseClientFromList(tempClient->getNickName());
	// 	}
	// }
	// else if (buffStdStr.find("QUIT") != std::string::npos)
	// {
	// 	// ok ya un moment particulier ou ça peut segfault
	// 	// je crois c'est lié au fait que parfois ya pas d'info dans le client
	// 	// voir pk ya pas d'info
	// 	//std::cout << "c3\n";
	// 	client * tempClient;
	// 	close(i);
	// 	FD_CLR(i, &(this->M_struct->current_sockets));
	// 	//std::cout << "c3.1\n";
	// 	tempClient = this->findClientBySocket(i);
	// 	//std::cout << "c3.2\n";
	// 	if (tempClient != NULL)
	// 		tempClient->goodBy();
	// 	//std::cout << "c3.3\n";
	// 	if (tempClient != NULL)
	// 		this->eraseClientFromList(tempClient->getNickName());
	// 	//std::cout << "c3.4\n";
	// 	//std::cout << "I ERASE\n";
	// }
	// else
	// {
		requestParsing(i);
		// std::cout << "IM HERE IN HANDLE REQUEST\n";
		// buff[nbCar] = 0;
	
		// std::cout << "nb car is " << nbCar << std::endl;
		// std::cout << "buff = " << buff << std::endl;

		// memset(buff, 0, 513);
	// }
	
	// le pong doit ^petre envoyé avant 301 seconds
}




	while (i < size)
	{	
		if (isAlpha(tmp[i]) == 1 && toggle == false)
		{
			// std::cout << "On trouve une majuscule" << std::endl;
			while (tmp[i] != ' ')
				i++;
			count++;
			toggle = true;
		} // pas fou
		// else 
		// {
		// 	std::cout << "Wrong request format" << std::endl;
		// 	return ;
		// }
		i++;
	}
	// std::cout << "count = "  << count << std::endl;
	// std::cout << "c3" << std::endl;
	std::string string_copy = tmp;
	std::string temp;
	size_t		token = 0;
	i = 0;
	while (i < count)
	{
		// token = string_copy.find('\r');
		// //std::cout << "Premier token = " << token << std::endl;
		// if (token == std::string::npos)
		// {
		// 	std::cout << "Wrong request format, need '\r'" << std::endl;
		// 	return ;
		// }
		token = string_copy.find('\n');
		if (token == std::string::npos)
		{
			std::cout << "Wrong request format, need '\n'" << std::endl;
			return ;
		}
		temp = string_copy.substr(0, token);
		// std::cout << "temp = " << temp << std::endl;
		this->M_requestVector.push_back(temp);
		// std::cout << "C4" << std::endl;
		// std::vector<std::string>::iterator ite = this->M_requestVector.end();
		// for (std::vector<std::string>::iterator it = this->M_requestVector.begin(); it != ite; it++)
			// std::cout << "vector = " << *it << std::endl;
		temp.erase();
		string_copy.erase(0, token);
		i++;
	}


	void	Server::chooseAction(void)
{
	std::vector<std::string>::iterator itc = this->M_commands.begin();
	std::vector<std::string>::iterator itec = this->M_commands.end();
	size_t	token = 0;
	bool	toggle = false;
	std::cout << "Je suis ici et la" << std::endl;
	for (; itc != itec; itc++)
	{
		std::cout << "Je rentre la" << std::endl;
		std::vector<std::string>::iterator it = this->M_requestVector.begin();
		std::vector<std::string>::iterator ite = this->M_requestVector.end();
		std::cout << "Dans le vecteur : " << *it << std::endl;
		// token = it->find("NICK");
		for (; it != ite; it++)
		{
			token = it->find(*(itc));
			if (token != std::string::npos)
			{
				std::cout << "La valeur est dans le vecteur" << std::endl;
				toggle = true;
				break ;
			}
		}
		if (toggle == true)
			break ;
	}
	std::cout << "token du vecteur = " << token << std::endl << std::endl;
	return ;
}