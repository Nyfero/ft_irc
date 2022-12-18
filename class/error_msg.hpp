#pragma once

# include "utils.hpp"

// PREFIX Error
# define ERR_NOSUCHNICK "No such nick"
# define ERR_PARSINGPREFIX "Parsing prefix error"
# define ERR_NOSUCHUSER "No such user"
# define ERR_NOSUCHHOSTNAME "No such hostname"


// PASS Error
# define ERR_NEEDMOREPARAMS "Not enough parameters"
# define ERR_ALREADYREGISTRED "This username is already registred"
# define ERR_PASSWDMISMATCH "Wrong password"

// NICK Error
# define ERR_NONICKNAMEGIVEN "No nickname given"
# define ERR_ERRONEUSNICKNAME "Erroneus nickname"
# define ERR_NICKNAMEINUSE "Nickname is already in use"
# define ERR_NICKCOLLISION "Nickname collision"
# define ERR_UNAVAILRESOURCE "Nick/channel is temporarily unavailable"
# define ERR_RESTRICTED "Your connection is restricted"

// USER Error
# define ERR_UMODEUNKNOWNFLAG "Unknown MODE flag"

// JOIN Error
# define ERR_BADCHANNELKEY  "Key-locked channel, either without a key or with the wrong key"
# define ERR_NOSUCHCHANNEL  "Channel name is invalid"
# define ERR_TOMUCHPARAMS   "To much parameters"

# define ERR_BANNEDFROMCHAN
# define ERR_INVITEONLYCHAN
# define ERR_CHANNELISFULL
# define ERR_BADCHANMASK

// JOIN RPL
# define RPL_TOPIC

// PART Error

# define ERR_NOTONCHANNEL