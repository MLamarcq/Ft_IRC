#include "command.hpp"

int	command::whatArg(std::vector<std::string> temp)
{
	size_t i = 0;
	int res = 0;

	while (i < temp.size())
	{
		if (i == 1)
		{
			int j = 0;
			while (temp[i][j])
			{
				if (j == 0 && (temp[i][j] == '+' || temp[i][j] == '-'))
				{
					res = temp[i][j + 1];
				}
				j++;
			}
		}
		i++;
	}
	return (res);
}

int	command::whatSign(std::vector<std::string> temp)
{
	size_t i = 0;
	int res = 0;

	while (i < temp.size())
	{
		if (i == 1)
		{
			int j = 0;
			while (temp[i][j])
			{
				if (j == 0 && (temp[i][j] == '+' || temp[i][j] == '-'))
				{
					res = temp[i][j];
				}
				j++;
			}
		}
		i++;
	}
	return (res);
}