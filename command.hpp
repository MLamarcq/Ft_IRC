/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mael <mael@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 15:33:59 by mlamarcq          #+#    #+#             */
/*   Updated: 2024/02/22 16:47:25 by mael             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// a la fin de chaque commande faut mettre control+V suivit de contol+m
	// j'ai l'impression que c pas le cas dans tous les repo
	// en fait suffit de faire en sorte de regarder que \n et ça va fonctionner sans control machin


// voir si dans les autres repo aussi quand j'envoi des fausses commande nc quitte
	// ne quitte pas dans les autre repo




// envoyer command partielle
	// quand j'envoie control D ça prend bien seulement en compte les derniers truc que g tapé meme si g pas effacer le precedent
	// voir comment ça fonctionne dans les autre repo
		// j'ai l'impression que ignore et au final ne prend que le dernier truc
		// en fait quand je fais control D ça bloque le terminal et je peux pas supp le prec
		// je peux seulement continuer a ecrire et si je met pas un espace entre le nom de la command et larg ça fonctionne pas
		// du coup normalement notre truc devrait déjà fonctionner
			//on prend juste la dernière et on sfait pas chier, a tester
			// en fait je peux pas tester car nc quitte avant après un NI ctr+D et CK ctr+ D
			// régler ça et normalement ça devrait fonctionner
			// en fait il faut que je vois ce que les repo recoivent et envoi a chaque requete nc
			// comme ça je sais ce qu'ils envoient pour pas qu'il quitte
				// ma théorie c que a chaque message qu'il reçoit il envoi un truc et si nc 
				// ne recoit pas de message pendant un certain nbr de message il quitte
				// en fait g capté c pas une question d'envoi
				// c jujste que mon program va pas à la suite quand il voit pas d'espace
				// et si je rajoute un espace bah meme si il fait r bah ça fonctionne
				// du coup faut que j'arrive à gérer si ya pas d'espacde
			// g reussi a gerer les fausses commandes sans quitter
			//voir pk control D 2 fois fait quitter
			//en fait la je peux fgaire autant de control D que je veux
			// mais ça sépare bien les commande lol
			// comment regrouper le tout ?
			//tester avec plusieurs command



			// g l'impression qu'il envoi rien bizzare, suivre le parcours du message dan sle code clbouche
			//yanou les commande fractionné ont tout cours l'air de pas de fonctionner, retester avec puis voir parcours du message
			// tester avec d'autre repo

		// faire en sorte que quand je quitte avec nc de manière inoipiné le client est deco

	// faire strategie pour tout recouper
// faire test sujet de correction

#ifndef COMMAND_HPP
#define COMMAND_HPP

# define CLRF	"\r\n"
//# define SERVER_NAME				"ircserv"
# define SERVER_NAME				"localhost"
//# define SERVER_HOSTNAME			std::string(SERVER_NAME) + ".fr"
#define SERVER_HOSTNAME				std::string("localhost")
# define SERVER_VERSION				"v4.2"
# define CHANNEL_MODES				"klnt"
# define USER_MODES					"io"
# define RPL_PREFIX(code, nick)		std::string(":" + SERVER_HOSTNAME + " " + code + " " + nick)
# define CLIENT_ID(nickname, username, command) (":" + nickname + "!~" + username + "@" + SERVER_NAME + " " + command + " ")


//		RPL
# define RPL_WELCOME(nick, user, host)			RPL_PREFIX("001", nick) + " :Welcome to the " + SERVER_NAME + " Network, " + nick + "!" + user + "@" + host + CLRF
# define RPL_YOURHOST(nick)						RPL_PREFIX("002", nick) + " :Your host is " + SERVER_HOSTNAME + ", running version " + SERVER_VERSION + CLRF
# define RPL_CREATED(nick, datetime)			RPL_PREFIX("003", nick) + " :This server was created " + datetime + CLRF
# define RPL_MYINFO(nick)						RPL_PREFIX("004", nick) + " " + SERVER_HOSTNAME + " " + SERVER_VERSION + " " + USER_MODES + " " + CHANNEL_MODES + CLRF
# define RPL_UMODEIS(nick, modes)				RPL_PREFIX("221", nick) + " :" + modes + CLRF
# define RPL_AWAY(nick, senderNick, msg)		RPL_PREFIX("301", nick) + " " + senderNick + " " + msg + CLRF
# define RPL_CHANNELMODEIS(channel, mode)		RPL_PREFIX("324", "") + " " + channel + " " + mode + " " + CLRF
// # define RPL_NOTOPIC(nick, chan)				RPL_PREFIX("331", nick) + " " + chan + " :No topic is set" + CLRF
# define RPL_TOPIC(nick, chan, topic)			RPL_PREFIX("332", nick) + " " + chan + " " + topic + CLRF
# define RPL_NAMREPLY(nick, chan, names)		RPL_PREFIX("353", nick) + " = " + chan + " :" + names + CLRF
# define RPL_ENDOFNAMES(nick, chan)				RPL_PREFIX("366", nick) + " " + chan + " :End of /NAMES list" + CLRF
# define RPL_YOUREOPER(nick)					RPL_PREFIX("381", nick) + " :You are now an IRC operator" + CLRF
# define RPL_QUIT(nick, senderNick, msg)		RPL_PREFIX("999", nick) + " :" + senderNick + " " + msg + CLRF

//		ERROR
# define ERR_NOSUCHNICK(nick, errNick)				RPL_PREFIX("401", nick) + "" + errNick + " :No such nick" + CLRF
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
// # define ERR_NOTONCHANNEL(nick, chan)				RPL_PREFIX("442", nick) + " :" + chan + " :his not on that channel" + CLRF
# define ERR_NEEDMOREPARAMS(nick, cmd)				RPL_PREFIX("461", nick) + " :" + cmd + " :Not enough parameters" + CLRF
# define ERR_ALREADYREGISTRED(nick)					RPL_PREFIX("462", nick) + " :Unauthorized command (already registered)" + CLRF
# define ERR_PASSWDMISMATCH(nick)					RPL_PREFIX("464", nick) + " :Password incorrect" + CLRF
# define ERR_CHANNELISFULL(nick, chan)				RPL_PREFIX("471", nick) + " " + chan + " :Cannot join channel (+l)" + CLRF
# define ERR_BANNEDFROMCHAN(nick, chan)				RPL_PREFIX("474", nick) + " " + chan + " :Bannded from channel (+b)" + CLRF
# define ERR_BADCHANNELKEY(nick, chan)				RPL_PREFIX("475", nick) + " " + chan + " :Cannot join channel (+k)" + CLRF
# define ERR_NOPRIVILEGES(nick)						RPL_PREFIX("481", nick) + " :Permission Denied- You're not an IRC operator" + CLRF
# define ERR_CHANOPRIVSNEEDED(channel)				RPL_PREFIX("482", "") + channel + " :You're not channel operator\r\n"
# define ERR_NOOPERHOST(nick)						RPL_PREFIX("491", nick) + " :No O-lines for your host" + CLRF
# define ERR_UMODEUNKNOWNFLAG(target)				RPL_PREFIX("501", "") + " " + target + " :Unknown MODE flag" + CLRF
# define ERR_USERSDONTMATCH(target)					RPL_PREFIX("502", "") + " " + target + " :Cant change mode for other users" +  CLRF
#define PONG(nickname) (std::string(":") + SERVER_NAME + " PONG " + SERVER_NAME + " :" + SERVER_NAME + "\r\n")

# define ERR_UNKNOWNMODE(nick, chan)				RPL_PREFIX("472", nick) + " " + chan + " is unknown mode char to me" + CLRF
# define ERR_INVITEONLYCHAN(nick, channel)			(std::string(":") + SERVER_NAME + " 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n")
# define MODE_CHANNEL_NEWMDP(nickname, username, channel, mode, pass)		(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " password is now :" + pass + "\r\n")
# define MODE_CHANNEL_CLEARMDP(nickname, username, channel, mode)			(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " password is now erased" + "\r\n")
# define MODE_CHANNEL_NOWOP(nickname, username, channel, mode, name)		(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " " + name + " is now a channel operator" + "\r\n")
# define MODE_CHANNEL_NOMOREOP(nickname, username, channel, mode, name)		(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " " + name + " is no longer a channel operator" + "\r\n")
# define MODE_CHANNEL_CLIENTLIMIT(nickname, username, channel, mode, nb)	(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " " + "client limit is now " + nb + "\r\n")
# define MODE_CHANNEL_NOCLIENTLIMIT(nickname, username, channel, mode)		(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " " + "no more client limit" + "\r\n")
# define MODE_CHANNEL_NO_INVITE(nickname, username, channel, mode) 			(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " " + "(invite_only) remove " + "\r\n")
# define MODE_CHANNEL_YES_INVITE(nickname, username, channel, mode)			(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " " + "(invite_only) add. The channel is now in restricted mode." + "\r\n")

# define MODE_UNKNOW_MODE(nickname, username, channel, mode)				(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " is unknown mode char to me" + "\r\n")
# define MODE_TOPIC_OFF(nickname, username, channel, mode)					(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " Only operators can now change channel's topic." + "\r\n")
# define MODE_TOPIC_ON(nickname, username, channel, mode)					(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " Channel's topic can now be change by everyone." + "\r\n")

# define MODE_NEEDMOREPARAMS(nickname, username, channel, mode)				(CLIENT_ID(nickname, username, "MODE")  + channel + " +" + mode + " Error. Need more prarameters." + "\r\n")

// # define NOTONCHANNEL(nickname, username, channel) 							(CLIENT_ID(nickname, username, "") + channel + " Error. You are not on that channel.\r\n")
#define NOTONCHANNEL(nickname, username, channel) 							(std::string(":") + SERVER_NAME + " 442 " + nickname + " " + channel + " :You are not on that channel\r\n")

# define NOSUCHCHANNEL(nickname, username, channel)							(CLIENT_ID(nickname, username, "") + " " + channel + " Error. No such channel." + "\r\n")
// # define TOPIC_IS_OFF(nick, chan)											(std::string(":") + SERVER_NAME + " " + nick + " :Topic changes desabled on " + chan + " !" + CLRF)
# define ERR_USERONCHANNEL(nickname, username, channel, name)				(CLIENT_ID(nickname, username, "") + " 443 " + channel + " " + name + " :is already on channel." + "\r\n")
// # define ERR_NOSUCHNICK(nickname, username, name)							(RPL_PREFIX("401", nickname) + " " + name + " :no such nick" + "\r\n")
# define INVITE_ON_CHAN(nickname, username, channel, name)					(CLIENT_ID(nickname, username, "") + " " + name + " You are invited on " + channel + " by " + nickname + "\r\n")
# define NEEDMOREPARAMS(nickname, username, cmd)							(CLIENT_ID(nickname, username, "")  + cmd + " Error. Need more prarameters." + "\r\n")
# define PRIVMSG_CHAN(nickname, username, dest, msg) 						(CLIENT_ID(nickname, username, "PRIVMSG") + dest + " :" + msg + "\r\n")
// # define PRIVMSG_USER(nickname, username, name, msg) 						(CLIENT_ID(nickname, username, "PRIVMSG") + name + " :" + msg + "\r\n")
// # define RPL_PRIVMSG(nick, username, target, message) 						(":" + nick + "!" + username + "@localhost PRIVMSG " + target + " :" + message + "\r\n")
# define NOSUCHUSER(nickname, username, errNick) 							(CLIENT_ID(nickname, username, "PRIVMSG") + errNick + " :No such Nick" + "\r\n")
# define PART_CHAN(nickname, username, dest, msg) 							(CLIENT_ID(nickname, username, "PART") + dest + " reason :" + msg + "\r\n")
# define RPL_PART(user_id, channel, reason) 								(user_id + " PART " + channel + " " + reason + "\r\n")
// # define KICK(nickname, username, channel, target, message) 				(CLIENT_ID(nickname, username, "KICK") + "#" + channel + " " + target + " :" + message + "\r\n")

# define KICK_CHAN(nickname, username, dest, target, msg) 				(CLIENT_ID(nickname, username, "KICK") + dest + " " + target + msg + "\r\n")
# define XKICK(nickname, username, channel, target, message)			(CLIENT_ID(nickname, username, "KICK") + channel + " " + target + " :" + message + "\r\n")
# define YOU_KICK(nickname, username, channel, target, message)			(CLIENT_ID(nickname, username, "KICK") + channel +" " + target + " " + message + "\r\n")
# define ERR_BADOPERKEY(nickname, username)								(CLIENT_ID(nickname, username, "OPER") + ": Operator password missmatch " + "\r\n")
# define ISNOWOPE(nickname, username)									(CLIENT_ID(nickname, username, "OPER") + ": is now operator on " + SERVER_NAME + "\r\n")
# define ISNOWOPE_BY_USER(nickname, username)							(CLIENT_ID(nickname, username, "OPER") + ": " + nickname + " mades you an operator on " + SERVER_NAME + "\r\n")
# define PRIVMSG_WALLOPS(nickname, username, msg) 						(CLIENT_ID(nickname, username, "WALLOPS") + msg + "\r\n")

# define ERR_WALLOPSPRIVSNEEDED(nickname, username)						CLIENT_ID(nickname, username, "WALLOPS") + ":You're not an operator\r\n"
// # define ISNOWOPEFROMCHAN(nickname, username, chan)								(CLIENT_ID(nickname, username, "OPER") + ": is now operator on " + SERVER_NAME + " and " + chan + "\r\n")
# define ISNOWOPEFROMCHAN(nickname, username, channel, name)			(CLIENT_ID(nickname, username, "") + name + " is now a channel operator in "  + channel + "\r\n")

#define CLIENT_NOTONCHANNEL(nickname, username, channel, target, mode) 	(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " " + target + " is not on the channel" + "\r\n")
#define WRONG_USER_MODE(nickname, username, channel, target, mode) 		(CLIENT_ID(nickname, username, "MODE")  + channel + " " + mode + " can't affect channel privileges " + target + " is a server operator" + "\r\n")
#define XQUIT(nickname, username, message) (CLIENT_ID(nickname, username, "QUIT") + ":" + message + "\r\n")

#define ERR_BADCHANNAME(nick, chan)										RPL_PREFIX("479", nick) + " " + chan + " :Illegal channel name\r\n"



# define JOIN_IN_CHAN(nickname, username, channel)						(CLIENT_ID(nickname, username, "JOIN")  + channel + " " + nickname + " has joigned the channel " + "\r\n")
# define WELCOME_CHAN(nickname, username, channel)						(CLIENT_ID(nickname, username, "JOIN")  + channel + " welcome to the channel " + "\r\n")
#define XNOTICE(nickname, username, dest, msg) 							(CLIENT_ID(nickname, username, "NOTICE") + dest + " :" + msg + "\r\n")

#define CLIENT_NOTONCHANNEL_KICK(nickname, username, channel, target) 	(CLIENT_ID(nickname, username, "")  + channel + " " + target + " is not on the channel" + "\r\n")

#define ERR_NOTONCHANNEL(nickname, username, channel, target) (std::string(":") + SERVER_NAME + " 442 " + target + " " + channel + " : his not not on that channel\r\n")
# define ERR_KICKPRIVSNEEDED(nickname, username)						CLIENT_ID(nickname, username, "") + "You're not an operator\r\n"

# define RPL_NOTOPIC(nick, chan)										RPL_PREFIX("331", nick) + chan + ":No topic is set\r\n"


#include "server.hpp"

class client;
class Server;
class channel;

class command {

	public:
		command();
		command(const command& copy);
		command& operator=(const command& copy);
		~command();

		
		std::string		PASS(int fd, Server *serv);
		std::string		NICK(int fd, Server *serv);
		std::string		USER(int fd, Server *serv);
		std::string		PING(int fd, Server *serv);
		//std::string		PONG();
		std::string		QUIT(int fd, Server* serv);
		int				WALLOPS(int fd, Server* serv);
		int				OPER(int fd, Server* serv);
		int				KICK(int fd, Server* serv);
		int				TOPIC(client *client1, Server *serv);
		int				MODE(client *client1, Server *serv);
		int				JOIN(client *client1, Server *serv);
		int				INVITE(client *client1, Server *serv);
		int				PRIVMSG(client *client1, Server *serv);
		int				PART(client *client1, Server *serv);
		int				KILL(int fd, Server* serv);
		int				NOTICE(int fd, Server* serv);
		int				PART_QUIT(client *client1, Server *serv, std::string);

		std::string		bot();

		std::vector<std::string>	parsTemp(std::vector<std::string> temp);

		int	handleCmd(client *client1, Server *serv, std::string cmd);
		int			whatArg(std::vector<std::string> temp);
		int			whatSign(std::vector<std::string> temp);

};
#endif