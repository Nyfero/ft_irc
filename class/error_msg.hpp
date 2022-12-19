#pragma once

# include "utils.hpp"

// PASS Error
# define ERR_NEEDMOREPARAMS "Not enough parameters"
# define ERR_ALREADYREGISTRED "This username is already registred"
# define ERR_PASSWDMISMATCH "Wrong password"
# define ERR_PASSWITHPREFIX "Password cannot start with ':'"

// NICK Error
# define ERR_NONICKNAMEGIVEN "No nickname given"
# define ERR_ERRONEUSNICKNAME "Erroneus nickname"
# define ERR_NICKNAMEINUSE "Nickname is already in use"
# define ERR_NICKCOLLISION "Nickname collision"
# define ERR_UNAVAILRESOURCE "Nick/channel is temporarily unavailable"
# define ERR_RESTRICTED "Your connection is restricted"

// PRIVMSG Error
# define ERR_NORECIPIENT(serveur) ":" + serveur + " 412 : No message to send"
# define ERR_NOSUCHNICK(serveur, nick_target) ":" + serveur + " 401 :" + nick_target + " : Nickname does not exist"
# define ERR_NOSUCHCHANNEL(serveur, chan_target) ":" + serveur + " 403 :" + chan_target + " : Channel does not exist"
