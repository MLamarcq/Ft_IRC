void	Server::i_handle_request(int i)
{
	requestParsing(i);
}




	while (i < size)
	{	
		if (isAlpha(tmp[i]) == 1 && toggle == false)
		{
			while (tmp[i] != ' ')
				i++;
			count++;
			toggle = true;
		} // pas fou
		// else 
		// {
		// 	return ;
		// }
		i++;
	}
	std::string string_copy = tmp;
	std::string temp;
	size_t		token = 0;
	i = 0;
	while (i < count)
	{
		// token = string_copy.find('\r');
		// if (token == std::string::npos)
		// {
		// 	return ;
		// }
		token = string_copy.find('\n');
		if (token == std::string::npos)
		{
			return ;
		}
		temp = string_copy.substr(0, token);
		this->M_requestVector.push_back(temp);
		// std::vector<std::string>::iterator ite = this->M_requestVector.end();
		// for (std::vector<std::string>::iterator it = this->M_requestVector.begin(); it != ite; it++)
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
	for (; itc != itec; itc++)
	{
		std::vector<std::string>::iterator it = this->M_requestVector.begin();
		std::vector<std::string>::iterator ite = this->M_requestVector.end();
		// token = it->find("NICK");
		for (; it != ite; it++)
		{
			token = it->find(*(itc));
			if (token != std::string::npos)
			{
				toggle = true;
				break ;
			}
		}
		if (toggle == true)
			break ;
	}
	return ;
}