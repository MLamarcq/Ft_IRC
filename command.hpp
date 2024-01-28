/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlamarcq <mlamarcq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:33:59 by mlamarcq          #+#    #+#             */
/*   Updated: 2024/01/25 15:59:38 by mlamarcq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP


# define SERVER_NAME				"ircserv"
# define SERVER_HOSTNAME			std::string(SERVER_NAME) + ".fr"
# define SERVER_VERSION				"v4.2"
# define CHANNEL_MODES				"klnt"
# define USER_MODES					"io"
# define RPL_PREFIX(code, nick)		std::string(":" + SERVER_HOSTNAME + " " + code + " " + nick)
//		RPL
# define RPL_WELCOME(nick, user, host)			RPL_PREFIX("001", nick) + " :Welcome to the " + SERVER_NAME + " Network, " + nick + "!" + user + "@" + host + CLRF
# define RPL_YOURHOST(nick)						RPL_PREFIX("002", nick) + " :Your host is " + SERVER_HOSTNAME + ", running version " + SERVER_VERSION + CLRF
# define RPL_CREATED(nick, datetime)			RPL_PREFIX("003", nick) + " :This server was created " + datetime + CLRF
# define RPL_MYINFO(nick)						RPL_PREFIX("004", nick) + " " + SERVER_HOSTNAME + " " + SERVER_VERSION + " " + USER_MODES + " " + CHANNEL_MODES + CLRF
# define RPL_UMODEIS(nick, modes)				RPL_PREFIX("221", nick) + " :" + modes + CLRF
# define RPL_AWAY(nick, senderNick, msg)		RPL_PREFIX("301", nick) + " " + senderNick + " " + msg + CLRF
# define RPL_CHANNELMODEIS(channel, mode)		RPL_PREFIX("324", "") + " " + channel + " " + mode + " " + CLRF
# define RPL_NOTOPIC(nick, chan)				RPL_PREFIX("331", nick) + " " + chan + " :No topic is set" + CLRF
# define RPL_TOPIC(nick, chan, topic)			RPL_PREFIX("332", nick) + " " + chan + " :" + topic + CLRF
# define RPL_NAMREPLY(nick, chan, names)		RPL_PREFIX("353", nick) + " = " + chan + " :" + names + CLRF
# define RPL_ENDOFNAMES(nick, chan)				RPL_PREFIX("366", nick) + " " + chan + " :End of /NAMES list" + CLRF
# define RPL_YOUREOPER(nick)					RPL_PREFIX("381", nick) + " :You are now an IRC operator" + CLRF
# define RPL_QUIT(nick, senderNick, msg)		RPL_PREFIX("999", nick) + " :" + senderNick + " " + msg + CLRF

//		ERROR
# define ERR_NOSUCHNICK(nick, errNick)				RPL_PREFIX("401", nick) + " " + errNick + " :No such nick" + CLRF
# define ERR_NOSUCHCHANNEL(nick, chan)				RPL_PREFIX("403", nick) + " " + chan + " :No such channel" + CLRF
# define ERR_CANNOTSENDTOCHAN(nick, chan)			RPL_PREFIX("404", nick) + " " + chan + " :Cannot send to channel" + CLRF
# define ERR_TOOMANYTARGETS(nick)					RPL_PREFIX("407", nick) + " :Too many targets" + CLRF
# define ERR_NORECIPIENT(nick, command)				RPL_PREFIX("411", nick) + " :No recipient given (" + command + ")" + CLRF 
# define ERR_NOTEXTTOSEND(nick)						RPL_PREFIX("412", nick) + " :No text to send" + CLRF
# define ERR_UNKNOWNCOMMAND(nick, command)			RPL_PREFIX("421", nick) + " :" + command + " :Unknown command" + CLRF
# define ERR_NONICKNAMEGIVEN(nick)					RPL_PREFIX("431", nick) + " :No nickname given" + CLRF
# define ERR_ERRONEUSNICKNAME(nick, errNick)		RPL_PREFIX("432", nick) + " " + errNick + " :Erroneous nickname" + CLRF
# define ERR_NICKNAMEINUSE(nick, errNick)			RPL_PREFIX("435", nick) + " " + errNick + " :Nickname is already in use" + CLRF 
# define ERR_USERNOTINCHANNEL(nick, userNick, chan)	RPL_PREFIX("441", nick) + " " + chan + " " + userNick + " :They aren't on that channel" + CLRF
# define ERR_NOTONCHANNEL(nick, chan)				RPL_PREFIX("442", nick) + " :" + chan + " :You're not on that channel" + CLRF
# define ERR_NEEDMOREPARAMS(nick, cmd)				RPL_PREFIX("461", nick) + " :" + cmd + " :Not enough parameters" + CLRF
# define ERR_ALREADYREGISTRED(nick)					RPL_PREFIX("462", nick) + " :Unauthorized command (already registered)" + CLRF
# define ERR_PASSWDMISMATCH(nick)					RPL_PREFIX("464", nick) + " :Password incorrect" + CLRF
# define ERR_CHANNELISFULL(nick, chan)				RPL_PREFIX("471", nick) + " " + chan + " :Cannot join channel (+l)" + CLRF
# define ERR_BADCHANNELKEY(nick, chan)				RPL_PREFIX("475", nick) + " " + chan + " :Cannot join channel (+k)" + CLRF
# define ERR_NOPRIVILEGES(nick)						RPL_PREFIX("481", nick) + " :Permission Denied- You're not an IRC operator" + CLRF
# define ERR_CHANOPRIVSNEEDED(channel)				RPL_PREFIX("482", "") + " " + channel + " :You're not channel operator" + CLRF
# define ERR_NOOPERHOST(nick)						RPL_PREFIX("491", nick) + " :No O-lines for your host" + CLRF
# define ERR_UMODEUNKNOWNFLAG(target)				RPL_PREFIX("501", "") + " " + target + " :Unknown MODE flag" + CLRF
# define ERR_USERSDONTMATCH(target)					RPL_PREFIX("502", "") + " " + target + " :Cant change mode for other users" +  CLRF

#include "server.hpp"

class command {

	public:
		command();
		command(const command& copy);
		command& operator=(const command& copy);
		~command();

		
		std::string		PASS();
		std::string		NICK();
		std::string		USER();
		std::string		PING();
		std::string		PONG();
		std::string		OPER();
		std::string		QUIT(int fd);
		std::string		JOIN();
		std::string		PART();
		std::string		TOPIC();
		std::string		KICK();
		std::string		MODE();
		std::string		PRIVMSG();
		std::string		NOTICE();
		std::string		KILL();
		std::string		WALLOPS();
		std::string		bot();


};
#endif