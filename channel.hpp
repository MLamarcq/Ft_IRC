/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlamarcq <mlamarcq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:33:59 by mlamarcq          #+#    #+#             */
/*   Updated: 2024/02/01 12:44:08 by mlamarcq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "client.hpp"
#include "server.hpp"

class client;

class channel {

	public:
		channel();
		channel(std::string channelName, int passWord);
		channel(const channel& copy);
		channel& operator=(const channel& copy);
		~channel();

		// si join 1ere fois -> client operator
		std::string	getName(void) const;

		
		void		setName(std::string parameter);
		int			setPassword(void);
		void		setListofClient(client  *client1);
		void		setOperators(client *client1);

		int			enterPassword(void) const;

		void		addClientToTheChannel(client *client1);

		int			isDigit(std::string str) const;

		void		printMap(void) const;

		// variables
	private :
	
		// std::list<client *>	_listOfClients;
		// std::list<client *>	_operators;
		std::map<client *, bool> _listOfClients;
		std::string			_name;
		std::string			_pswd;
};

#endif