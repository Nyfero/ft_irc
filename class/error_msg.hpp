#pragma once

# include "utils.hpp"

// PREFIX Error
// # define ERR_NOSUCHNICK(serveur)    ":" + serveur + " 401 : No such nick"
# define ERR_PARSINGPREFIX          "Parsing prefix error"
# define ERR_NOSUCHUSER             "No such user"
# define ERR_NOSUCHHOSTNAME         "No such hostname"


// PASS Error
# define ERR_NEEDMOREPARAMS(serveur)         ":" + serveur + " 461 : Not enough parameters"
# define ERR_ALREADYREGISTRED(serveur)       ":" + serveur + " 462 : This username is already registred"
# define ERR_PASSWDMISMATCH(serveur)         ":" + serveur + " 464 : Wrong password"

// NICK Error
# define ERR_NONICKNAMEGIVEN(serveur)          ":" + serveur + " 431 : No nickname given"
# define ERR_ERRONEUSNICKNAME(serveur, nick)    ":" + serveur + " 432 :" + nick + " : Erroneus nickname"
# define ERR_NICKNAMEINUSE(serveur, nick)       ":" + serveur + " 433 :" + nick + " : Nickname is already in use"
# define ERR_NICKCOLLISION(serveur, nick)       ":" + serveur + " 436 :" + nick + " : Nickname collision"
# define ERR_UNAVAILRESOURCE "Nick/channel is temporarily unavailable"
# define ERR_RESTRICTED "Your connection is restricted"

// MODE Error
# define ERR_USERSDONTMATCH(serveur)       ":" + serveur + " 502 : User don't match"
# define ERR_USEAWAY "Please use command AWAY"

// USER Error
# define ERR_UMODEUNKNOWNFLAG(serveur)       ":" + serveur + " 501 : Unknown MODE flag"

// JOIN Error
# define ERR_BADCHANNELKEY(serveur, chan)   ":" + serveur + " 475 " + chan + ": Key-locked channel, either without a key or with the wrong key"
# define ERR_NOSUCHCHANNEL(serveur, chan)   ":" + serveur + " 403 " + chan + ": Channel name is invalid"
# define ERR_TOMUCHPARAMS                   "To much parameters"

# define ERR_BANNEDFROMCHAN
# define ERR_INVITEONLYCHAN
# define ERR_CHANNELISFULL
# define ERR_BADCHANMASK

// PART Error

# define ERR_NOTONCHANNEL   ":" + srv + " 442 :" + chan + " : Not in channel"

// RPL
// PRIVMSG Error
# define ERR_NORECIPIENT(serveur) ":" + serveur + " 412 : No message to send"
# define ERR_NOSUCHNICK(serveur, nick_target) ":" + serveur + " 401 :" + nick_target + " : Nickname does not exist"
# define ERR_NOSUCHCHANNEL(serveur, chan_target) ":" + serveur + " 403 :" + chan_target + " : Channel does not exist"

# define RPL_WELCOME(serv, name, use, hostname)    ":"+serv+" 001 "+name+" :Welcome to the "+serv+" network, "+name+"["+use+"@"+hostname+"]"