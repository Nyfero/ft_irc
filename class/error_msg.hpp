#pragma once

# include "utils.hpp"

// PREFIX Error
// # define ERR_NOSUCHNICK(serveur)    ":" + serveur + " 401 : No such nick"
# define ERR_PARSINGPREFIX          "Parsing prefix error"
# define ERR_NOSUCHUSER             "No such user"
# define ERR_NOSUCHHOSTNAME         "No such hostname"



// PART Error

# define ERR_NOTONCHANNEL   ":" + srv + " 442 :" + chan + " : Not in channel"

// RPL
// PRIVMSG Error
# define ERR_NORECIPIENT(serveur) ":" + serveur + " 411 : No recipient specified"
# define ERR_NOTEXTTOSEND(serveur) ":" + serveur + " 412 : No text to send"
# define ERR_NOSUCHNICK(serveur, nick_target) ":" + serveur + " 401 :" + nick_target + " : Nickname does not exist"
# define ERR_CANNOTSENDTOCHAN(serveur, channel_target) ":" + serveur + " 404 :" + channel_target + " : User cannot send to this channel"
# define ERR_UNVALIDMSG(serveur) ":" + serveur + " : Message to send is invalid 9must start with :"

# define RPL_WELCOME(serv, name, use, hostname)    ":"+serv+" 001 "+name+" :Welcome to the "+serv+" network, "+name+"["+use+"@"+hostname+"]"

/*---------------------------------------------------------------------------*/

// Pass
# define ERR_NEEDMOREPARAMS(serveur, commande) ":" + serveur + " 461 " + commande + " : Not enough parameters"
# define ERR_ALREADYREGISTRED(serveur) ":" + serveur + " 462 :Unauthorized command (already registered)"

// Nick
# define ERR_NONICKNAMEGIVEN(serveur) ":" + serveur + " 431 :No nickname given"
# define ERR_NICKNAMEINUSE(serveur, nick) ":" + serveur + " 433 " + nick + " :Nickname is already in use"
# define ERR_ERRONEUSNICKNAME(serveur, nick) ":" + serveur + " 432 " + nick + " :Erroneus nickname"
# define ERR_NICKCOLLISION(serveur, nick, usr, host) ":" + serveur + " 436 " + nick + " :Nickname collision KILL from " + usr + "@" + host
# define ERR_UNAVAILRESOURCE(serveur, nick) ":" + serveur + " 437 " + nick + " :Nick/channel is temporarily unavailable"
# define ERR_RESTRICTED(serveur, nick) ":" + serveur + " 484 " + nick + " :Your connection is restricted!"

// User
// same as pass

// Oper
# define RPL_YOUREOPER(serveur, nick) ":" + serveur + " 381 " + nick + " :You are now an IRC operator"
# define ERR_PASSWDMISMATCH(serveur) ":" + serveur + " 464 :Password incorrect"
# define ERR_NOOPERHOST(serveur) ":" + serveur + " 491 :No O-lines for your host"

// Mode
# define RPL_UMODEIS(serveur, mode) ":" + serveur + " 221 " + mode
# define ERR_UMODEUNKNOWNFLAG(serveur) ":" + serveur + " 501 :Unknown MODE flag"
# define ERR_USERSDONTMATCH(serveur) ":" + serveur + " 502 :Cannot change mode for other users"

// Join
# define RPL_TOPIC(serveur, chan, topic) ":" + serveur + " 332 " + chan + " :" + topic
# define ERR_NOSUCHCHANNEL(serveur, chan) ":" + serveur + " 403 " + chan + " :No such channel"
# define ERR_TOOMANYCHANNELS(serveur, chan) ":" + serveur + " 405 " + chan + " :You have joined too many channels"
# define ERR_TOOMANYTARGETS(serveur, chan) ":" + serveur + " 407 " + chan + " :Too many targets"
# define ERR_CHANNELISFULL(serveur, chan) ":" + serveur + " 471 " + chan + " :Cannot join channel (+l)"
# define ERR_INVITEONLYCHAN(serveur, chan) ":" + serveur + " 473 " + chan + " :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN(serveur, chan) ":" + serveur + " 474 " + chan + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(serveur, chan) ":" + serveur + " 475 " + chan + " :Cannot join channel (+k)"
# define ERR_BADCHANMASK(serveur, chan) ":" + serveur + " 476 " + chan + " :Bad Channel Mask"

// Part
# define ERR_NOTONCHANNEL(serveur, chan) ":" + serveur + " 442 " + chan + " :You're not on that channel"

// Topic
# define RPL_NOTOPIC(serveur, chan) ":" + serveur + " 331 " + chan + " :No topic is set"
# define RPL_TOPIC(serveur, chan, topic) ":" + serveur + " 332 " + chan + " :" + topic
# define ERR_CHANOPRIVSNEEDED(serveur, chan) ":" + serveur + " 482 " + chan + " :You're not channel operator"
# define ERR_NOCHANMODES(serveur, chan) ":" + serveur + " 477 " + chan + " :Channel doesn't support modes"

// Names
# define RPL_NAMREPLY(serveur, chan, names) ":" + serveur + " 353 " + chan + " :" + names
# define ERR_TOOMANYMATCHES(serveur, chan) ":" + serveur + " 416 " + chan + " :Wildcard in toomany matches"