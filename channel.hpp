/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mael <mael@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:33:59 by mlamarcq          #+#    #+#             */
/*   Updated: 2024/02/21 20:41:47 by mael             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "client.hpp"
#include "command.hpp"
#include "server.hpp"

class client;
class Server;
class command;

class channel {

	public:
		channel();
		channel(std::string channelName, int passWord);
		channel(const channel& copy);
		channel& operator=(const channel& copy);
		~channel();

		// si join 1ere fois -> client operator
		std::string				getName(void) const;
		std::string 			getPassword(void) const;
		std::string				getTopic(void) const;
		int						getClientLimit(void) const;
		int						getNbrOfClients(void) const;
		bool					getIsInvite(void) const ;
		bool					getTopicEstate(void) const ;
		std::map<client*, bool> getListOfClients(void) const;
		std::list<client *>		getWaitingList(void) const;
		void					setWaitingList(client *clien1);
		
		void		increaseNbrCLient(void);
		// void		changeIsInvite(void);
		void		changeIsInviteOff(std::string name, std::string username, std::string mode);
		void		changeIsInviteOn(std::string name, std::string username, std::string mode);
		
		void		setTopic(std::string topic);
		void		setTopicEstate(std::string name, std::string username, std::string mode, int code);
		int			setName(std::string parameter);
		void		setPassword(std::string password);
		void		setListofClient(client  *client1);
		void		setOperators(client *client1);
		void		setNewPassWord(std::string pass, std::string name, std::string username, std::string mode);
		void		clearPassWord(std::string name, std::string username, std::string mode);

		int			enterPassword(void) const;

		void		addClientToTheChannel(client *client1);

		int			isDigit(std::string str) const;

		void		printMap(void) const;

		client		*copyClient(client *original);

		int			setChannelFirstTime(client *client1, Server *serv, std::vector<std::string> temp);

		void		welcomeInChanMessage(client *client1);

		void		sendToAllChan(std::string message);
		void		sendPrivMsg(client *client1, std::string message);


		void		changePrivileges(client *client_emit, std::string name, std::string username, std::string mode, std::string client1, int code);

		void		setClientLimit(std::string name, std::string username, std::string mode, std::string nb);

		void		eraseClientLimit(std::string name, std::string username, std::string mode);

		void		eraseCLientFromChan(client *client1, std::string reason);

		int			handleIsInvite( client *client1, Server *serv, std::vector<std::string> temp);


		bool		isInThechan(client *client1) const;
		bool		isOperatorInChan(client *client1) const;
		int			addClientToChannel(client *client1, Server *serv, std::vector<std::string> temp);

		bool		isInChan(client *client1);


		std::map<client *, bool> _listOfClients;
		// variables
	private :
	
		// std::list<client *>	_listOfClients;
		// std::list<client *>	_operators;
		int						_clientLimit;
		int						_nbrClients;
		std::list<client *>		_waitingList;
		std::string				_name;
		std::string				_pswd;
		std::string				_topic;
		bool					_isInvite;
		bool					_topicEstate;
};

#endif