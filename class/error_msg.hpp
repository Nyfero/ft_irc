#pragma once

# include "utils.hpp"

// PREFIX Error
# define ERR_PARSINGPREFIX          "Parsing prefix error"
# define ERR_NOSUCHUSER             "No such user"
# define ERR_NOSUCHHOSTNAME         "No such hostname"

# define ERR_USEAWAY "Use AWAY command"



/*---------------------------------------------------------------------------*/
# define RPL_WELCOME(serveur, nick, use, host) ":" + serveur + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + use + "@" + host

# define RPL_UMODEIS(serveur, mode) ":" + serveur + " 221 " + mode

# define RPL_AWAY(serveur, nick, nick_dest, msg) ":" + serveur + " 301 " + nick + " " + nick_dest + " :" + msg
# define RPL_UNAWAY(serveur, nick) ":" + serveur + " 305 " + nick + " :You are no longer marked as being away"
# define RPL_NOWAWAY(serveur, nick) ":" + serveur + " 306 " + nick + " :You have been marked as being away"
# define RPL_LIST(serveur, chan, nb_users, topic) ":" + serveur + " 322 " + chan + " " + nb_users + " :" + topic
# define RPL_LISTEND(serveur) ":" + serveur + " 323 :End of LIST"
# define RPL_CHANNELMODEIS(nick, chan, mode) ":" + nick + " MODE " + chan + " " + mode
# define RPL_NOTOPIC(serveur, chan) ":" + serveur + " 331 " + chan + " :No topic is set"
# define RPL_TOPIC(serveur, nick, chan, topic) ":" + serveur + " 332 " + nick + " " + chan + " :" + topic


# define RPL_INVITING(serveur, sender, chan, target) ":" + serveur + " 341 " + sender + " " + target + " " + chan
# define RPL_NAMREPLY(serveur, nick, chan, names) ":" + serveur + " 353 " + nick + " = " + chan + " :" + names


# define RPL_ENDOFNAMES(serveur, nick, chan) ":" + serveur + " 366 " + nick + " " + chan + " :End of NAMES list"
# define RPL_YOUREOPER(serveur, nick) ":" + serveur + " 381 " + nick + " :You are now an IRC operator"
# define RPL_USERSSTART(serveur) ":" + serveur + " 392 :UserID Terminal Host"
# define RPL_USERS(serveur, nick, usr, host) ":" + serveur + " 393 " + nick + " " + usr + " " + host
# define RPL_ENDOFUSERS(serveur) ":" + serveur + " 394 :End of users"
# define RPL_NOUSERS(serveur) ":" + serveur + " 395 :Nobody logged in"

# define ERR_NOSUCHNICK(serveur, nick) ":" + serveur + " 401 " + nick + " :No such nick/channel"
# define ERR_NOSUCHSERVER(serveur, chan) ":" + serveur + " 402 " + chan + " :No such server"
# define ERR_NOSUCHCHANNEL(serveur, nick, chan) ":" + serveur + " 403 " + nick + " " + chan + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(serveur, chan) ":" + serveur + " 404 " + chan + " :Cannot send to channel"
# define ERR_TOOMANYCHANNELS(serveur, chan) ":" + serveur + " 405 " + chan + " :You have joined too many channels"
# define ERR_TOOMANYTARGETS(serveur, chan) ":" + serveur + " 407 " + chan + " :Too many targets"
# define ERR_NOSUCHSERVICE(serveur, chan) ":" + serveur + " 408 " + chan + " :No such service"
# define ERR_NOORIGIN(serveur) ":" + serveur + " 409 :No origin specified"
# define ERR_NORECIPIENT(serveur) ":" + serveur + " 411 :No recipient given"
# define ERR_NOTEXTTOSEND(serveur) ":" + serveur + " 412 :No text to send"
# define ERR_NOTOPLEVEL(serveur, chan) ":" + serveur + " 413 " + chan + " :No toplevel domain specified"
# define ERR_WILDTOPLEVEL(serveur, chan) ":" + serveur + " 414 " + chan + " :Wildcard in toplevel domain"
# define ERR_TOOMANYMATCHES(serveur, chan) ":" + serveur + " 416 " + chan + " :Wildcard in toomany matches"
# define ERR_FILEERROR(serveur, chan) ":" + serveur + " 417 " + chan + " :File error doing"
# define ERR_NONICKNAMEGIVEN(serveur) ":" + serveur + " 431 :No nickname given"
# define ERR_ERRONEUSNICKNAME(serveur, nick) ":" + serveur + " 432 " + nick + " :Erroneus nickname"
# define ERR_NICKNAMEINUSE(serveur, nick) ":" + serveur + " 433 " + nick + " :Nickname is already in use"
# define ERR_NICKCOLLISION(serveur, nick, usr, host) ":" + serveur + " 436 " + nick + " :Nickname collision KILL from " + usr + "@" + host
# define ERR_UNAVAILRESOURCE(serveur, nick) ":" + serveur + " 437 " + nick + " :Nick/channel is temporarily unavailable"
# define ERR_USERNOTINCHANNEL(serveur, nick, chan) ":" + serveur + " 441 " + nick + " " + chan + " :They aren't on that channel"
# define ERR_NOTONCHANNEL(serveur, chan) ":" + serveur + " 442 " + chan + " :You're not on that channel"
# define ERR_USERONCHANNEL(serveur, sender, chan, target) ":" + serveur + " 443 " + sender + " " + target + " :is already on channel"
# define ERR_NOLOGIN(serveur, usr) ":" + serveur + " 444 " + usr " :User not logged in"
# define ERR_USERDISABLED(serveur, nick) ":" + serveur + " 446 " + nick + " :User has been disabled"
# define ERR_NEEDMOREPARAMS(serveur, commande) ":" + serveur + " 461 " + commande + " : Not enough parameters"
# define ERR_ALREADYREGISTRED(serveur) ":" + serveur + " 462 :Unauthorized command (already registered)"
# define ERR_PASSWDMISMATCH(serveur) ":" + serveur + " 464 :Password incorrect"
# define ERR_CHANNELISFULL(serveur, chan) ":" + serveur + " 471 " + chan + " :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE(serveur, chan) ":" + serveur + " 472 " + chan + " :is unknown mode char to me"
# define ERR_INVITEONLYCHAN(serveur, chan) ":" + serveur + " 473 " + chan + " :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN(serveur, chan) ":" + serveur + " 474 " + chan + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(serveur, chan) ":" + serveur + " 475 " + chan + " :Cannot join channel (+k)"
# define ERR_BADCHANMASK(serveur, chan) ":" + serveur + " 476 " + chan + " :Bad Channel Mask"
# define ERR_NOCHANMODES(serveur, chan) ":" + serveur + " 477 " + chan + " :Channel doesn't support modes"
# define ERR_CHANOPRIVSNEEDED(serveur, chan) ":" + serveur + " 482 " + chan + " :You're not channel operator"
# define ERR_RESTRICTED(serveur, nick) ":" + serveur + " 484 " + nick + " :Your connection is restricted!"
# define ERR_NOOPERHOST(serveur) ":" + serveur + " 491 :No O-lines for your host"

# define ERR_UMODEUNKNOWNFLAG(serveur) ":" + serveur + " 501 :Unknown MODE flag"
# define ERR_USERSDONTMATCH(serveur) ":" + serveur + " 502 :Cannot change mode for other users"