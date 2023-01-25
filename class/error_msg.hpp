#pragma once

# include "utils.hpp"

# define RPL_WELCOME(serveur, nick, use, host) ":" + serveur + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + use + "@" + host
# define RPL_UMODEIS(nick, target, mode) ":" + nick + " MODE " + target + " :" + mode
# define RPL_MYMODE(serveur, nick, mode) ":" + serveur + " 221 " + nick + " " + mode
# define RPL_AWAY(serveur, nick, nick_dest, msg) ":" + serveur + " 301 " + nick + " " + nick_dest + " :" + msg
# define RPL_UNAWAY(serveur, nick) ":" + serveur + " 305 " + nick + " :You are no longer marked as being away"
# define RPL_NOWAWAY(serveur, nick) ":" + serveur + " 306 " + nick + " :You have been marked as being away"
# define RPL_LISTSTART(serveur, nick) ":" + serveur + " 321 " + nick + " Channel :Users Name"
# define RPL_LIST(serveur, nick, chan, nb_users, topic) ":" + serveur + " 322 " + nick + " " + chan + " " + nb_users + " :" + topic
# define RPL_LISTEND(serveur, nick) ":" + serveur + " 323 " + nick + " :End of /LIST"
# define RPL_CHANNELNEWOP(pre, chan, mode, nick) ":" + pre + " MODE " + chan + " " + mode + " " + nick
# define RPL_CHANNELMODEIS(pre, chan, mode) pre + " MODE " + chan + " " + mode
# define RPL_CHANNEL_MODE(serveur, nick, chan, mode) ":" + serveur + " 324 " + nick + " " + chan + " " + mode
# define RPL_NOTOPIC(serveur, chan) ":" + serveur + " 331 " + chan + " :No topic is set"
# define RPL_TOPIC(serveur, nick, chan, topic) ":" + serveur + " 332 " + nick + " " + chan + " :" + topic
# define RPL_INVITING(serveur, sender, chan, target) ":" + serveur + " 341 " + sender + " " + chan + " " + target
# define RPL_NAMREPLY(serveur, nick, chan, names) ":" + serveur + " 353 " + nick + " = " + chan + " :" + names
# define RPL_ENDOFNAMES(serveur, nick, chan) ":" + serveur + " 366 " + nick + " " + chan + " :End of NAMES list"
# define RPL_YOUREOPER(serveur, nick) ":" + serveur + " 381 " + nick + " :You are now an IRC operator"
# define RPL_USERSSTART(serveur) ":" + serveur + " 392 :UserID Terminal Host"
# define RPL_USERS(serveur, nick, usr, host) ":" + serveur + " 393 " + nick + " " + usr + " " + host
# define RPL_ENDOFUSERS(serveur) ":" + serveur + " 394 :End of users"
# define RPL_NOUSERS(serveur) ":" + serveur + " 395 :Nobody logged in"

# define ERR_NOSUCHNICK(serveur, sender, target_nick) ":" + serveur + " 401 " + sender + " " + target_nick + " :No such nick/channel"
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
# define ERR_USERNOTINCHANNEL(serveur, sender, nick, chan) ":" + serveur + " 441 " + sender + " " + nick + " " + chan + " :They aren't on that channel"
# define ERR_NOTONCHANNEL(serveur, chan) ":" + serveur + " 442 " + chan + " :You're not on that channel"
# define ERR_USERONCHANNEL(serveur, sender, chan, target) ":" + serveur + " 443 " + sender + " " + target + " :is already on channel"
# define ERR_NOLOGIN(serveur, usr) ":" + serveur + " 444 " + usr " :User not logged in"
# define ERR_USERDISABLED(serveur, nick) ":" + serveur + " 446 " + nick + " :User has been disabled"
# define ERR_NEEDMOREPARAMS(serveur, commande) ":" + serveur + " 461 " + commande + " : Not enough parameters"
# define ERR_ALREADYREGISTRED(serveur) ":" + serveur + " 462 :Unauthorized command (already registered)"
# define ERR_PASSWDMISMATCH(serveur) ":" + serveur + " 464 :Password incorrect"
# define ERR_KEYSET(serveur, chan) ":" + serveur + " 467 " + chan + " :Channel key already set" 
# define ERR_CHANNELISFULL(serveur, chan) ":" + serveur + " 471 " + chan + " :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE(serveur, chan) ":" + serveur + " 472 " + chan + " :is unknown mode char to me"
# define ERR_INVITEONLYCHAN(serveur, nick, chan) ":" + serveur + " 473 " + nick + " " + chan + " :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN(serveur, chan) ":" + serveur + " 474 " + chan + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(serveur, nick, chan) ":" + serveur + " 475 " + nick + " " + chan + " :Cannot join channel (+k)"
# define ERR_BADCHANMASK(serveur, nick, chan) ":" + serveur + " 476 " + chan + " :Bad Channel Mask"
# define ERR_NOCHANMODES(serveur, chan) ":" + serveur + " 477 " + chan + " :Channel doesn't support modes"
# define ERR_NOPRIVILEGES(serveur, sender) ":" + serveur + " 481 " + sender + " :Permission Denied, You do not have the correct irc operator privileges"
# define ERR_CHANOPRIVSNEEDED(serveur, sender, chan) ":" + serveur + " 482 " + sender + " " + chan + " :You're not channel operator"
# define ERR_RESTRICTED(serveur, nick) ":" + serveur + " 484 " + nick + " :Your connection is restricted!"
# define ERR_NOOPERHOST(serveur) ":" + serveur + " 491 :No O-lines for your host"
# define ERR_UMODEUNKNOWNFLAG(pre, nick) ":" + pre + " " + nick + " : Unknown MODE flag"
# define ERR_USERSDONTMATCH(serveur) ":" + serveur + " 502 :Cannot change mode for other users"

#define BOT_HELP_TEXT(nickname) "\033[1;36m@ " + nickname + ", here is a list of available commands:\n    - \033[1;32mheure\033[36m: displays the current time\n    - \033[1;32mliste\033[36m: displays the list of channels in this server\n    - \033[1;32mgreetings\033[36m: sends a greeting message\n    - \033[1;32mhelp\033[36m: displays this prompt\n"
#define BOT_NO_PARAMS "\033[35mAre you trying to speak with \033[1;31;1mBob the Bot\033[35m ? You can say \033[1;31;4m/msg help\033[35m to know more\033[0m"
#define BOT_GREETING_TEXT "\033[1;32m" + greetings[rand() % 5] + "\033[0m"
const std::string greetings[] = {
    "Hello! How can I help you today?",
    "Welcome! Is there anything I can assist you with?",
    "Hi! What can I do for you?",
    "Good day! How may I be of service?",
    "Greetings! How can I help you?"
};
