/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mael <mael@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:33:59 by mlamarcq          #+#    #+#             */
/*   Updated: 2024/02/07 14:26:33 by mael             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "client.hpp"
#include "server.hpp"

class client;
class Server;

class channel {

	public:
		channel();
		channel(std::string channelName, int passWord);
		channel(const channel& copy);
		channel& operator=(const channel& copy);
		~channel();

		// si join 1ere fois -> client operator
		std::string	getName(void) const;
		std::string getPassword(void) const;
		int			getClientLimit(void) const;
		int			getNbrOfClients(void) const;
		std::map<client*, bool> getListOfClients(void) const;
		void		increaseNbrCLient(void);

		
		int			setName(std::string parameter);
		void		setPassword(std::string password);
		void		setListofClient(client  *client1);
		void		setOperators(client *client1);

		int			enterPassword(void) const;

		void		addClientToTheChannel(client *client1);

		int			isDigit(std::string str) const;

		void		printMap(void) const;

		client		*copyClient(client *original);

		int			setChannelFirstTime(client *client1, Server *serv, channel *new_one, std::vector<std::string> temp);


		// variables
	private :
	
		// std::list<client *>	_listOfClients;
		// std::list<client *>	_operators;
		int						_clientLimit;
		int						_nbrClients;
		std::map<client *, bool> _listOfClients;
		std::string				_name;
		std::string				_pswd;
};

#endif