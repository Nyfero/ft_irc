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

// USERS Error
# define ERR_NOSUCHSERVER(serveur)       ":" + serveur + " 402 : No such server"

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
# define ERR_NORECIPIENT(serveur) ":" + serveur + " 411 : No recipient specified"
# define ERR_NOTEXTTOSEND(serveur) ":" + serveur + " 412 : No text to send"
# define ERR_NOSUCHNICK(serveur, nick_target) ":" + serveur + " 401 :" + nick_target + " : Nickname does not exist"
# define ERR_CANNOTSENDTOCHAN(serveur, channel_target) ":" + serveur + " 404 :" + channel_target + " : User cannot send to this channel"
# define ERR_UNVALIDMSG(serveur) ":" + serveur + " : Message to send is invalid 9must start with :"

# define RPL_WELCOME(serv, name, use, hostname)    ":"+serv+" 001 "+name+" :Welcome to the "+serv+" network, "+name+"["+use+"@"+hostname+"]"



# define RPL_WELCOME(nickname, username, hostname) "Welcome to the Internet Relay Network " + nickname + "!" + username + "@" + hostname
# define RPL_YOURHOST(servername, version) "Your host is " +  servername + ", running version " + version
# define RPL_CREATED(creation) "This server was created " + creation
# define RPL_MYINFO(servername, version, user_modes, channel_modes) servername + version + user_modes + channel_modes

# define RPL_AWAY(nickname, message) nickname + " :" + message


/* 221 */ std::string	irc::RplUModeIs (std::string modes) { return (" " + modes); }
// 234 - No need
// 235 - No need
// 242 - No need
// 243 - No need
// 251 to 255 - No need
/* 256 */ std::string	irc::RplAdminMe (std::string server) { return (" " + server + " :Administrative info"); }
/* 257 */ std::string	irc::RplAdminLocOne (std::string adminloc1) { return (" :" + adminloc1); }
/* 258 */ std::string	irc::RplAdminLocTwo (std::string adminloc2) { return (" :" + adminloc2); }
/* 259 */ std::string	irc::RplAdminEmail (std::string adminemail) { return (" :" + adminemail); }
// 261 to 263 - No need
/* 301 */ std::string	irc::RplAway (std::string dest_nick, std::string away_msg) { return (" " + dest_nick + " :" + away_msg); }
/* 302 */ std::string	irc::RplUserhost (std::string replies) { return (" :" + replies); }
/* 303 */ std::string	irc::RplIson (std::string nicks_presents) { return (" :" + nicks_presents); }
/* 305 */ std::string	irc::RplUnaway () { return (" :You are no longer marked as being away"); }
/* 306 */ std::string	irc::RplNowAway () { return (" : You have been marked as being away"); }
/* 311 */ std::string	irc::RplWhoIsUser (std::string w_nick, std::string w_user, std::string w_host, std::string w_real) { return (" " + w_nick + " " + w_user + " " + w_host + " * :" + w_real); }
/* 312 */ std::string	irc::RplWhoIsServer (std::string server, std::string server_nick, std::string infos) { return (" " + server_nick + " " + server + " :" + infos); };
/* 313 */ std::string	irc::RplWhoIsOperator (std::string o_nick) { return (" " + o_nick + " :is an IRC operator"); }
// 315 - No need
// 317 - No need
/* 318 */ std::string	irc::RplEndOfWhoIs () { return (" :End of WHOIS list"); }
/* 319 */ std::string	irc::RplWhoIsChannels (std::string w_nick, std::string w_list_channels_and_op) { return (" " + w_nick + " :" + w_list_channels_and_op); } 
/* 322 */ std::string	irc::RplList (std::string channel, std::string visible, std::string topic) { return (" " + channel + " " + visible + " :" + topic); }
/* 323 */ std::string	irc::RplListEnd () { return (" :End of LIST"); }
/* 324 */ std::string	irc::RplChannelModeIs (std::string channel, std::string mode, std::string mode_params) { return (" " + channel + " " + mode + " " + mode_params); }
/* 331 */ std::string	irc::RplNoTopic (std::string channel) { return (" " + channel + " :No topic is set"); }	
/* 332 */ std::string	irc::RplTopic (std::string channel, std::string topic) { return (" " + channel + " :" + topic); }
/* 341 */ std::string	irc::RplInviting (std::string nick, std::string channel) { return (" " + channel + " " + nick); }
// 342 - No need
/* 346 */ std::string	irc::RplInviteList (std::string channel, std::string invitemask) { return (" " + channel + " " + invitemask); }
/* 347 */ std::string	irc::RplEndOfInviteList (std::string channel) { return (" " + channel + " :End of channel invite list"); }
/* 348 */ std::string	irc::RplExceptList (std::string channel, std::string exceptionmask) { return (" " + channel + " " + exceptionmask); }
/* 349 */ std::string	irc::RplEndOfExceptList (std::string channel) { return (" " + channel + " :End of channel invite list"); }
/* 351 */ std::string	irc::RplVersion (std::string server, std::string version, std::string debug, std::string comments) { return (" " + version + "." + debug + " " + server + " :" + comments); }
// 352 - No need
/* 353 */ std::string	irc::RplNamReply (std::string type_chan, std::string channel, std::string nicks) { return (" " + type_chan + " " + channel + " :" + nicks); }
// nicks must contains each nicks of the users presents in the channel with either nothing/@/+ as prefix (@ = operator of channel, +=authorization to speak in moderated channel)
// 364 - No need
// 365 - No need
/* 366 */ std::string	irc::RplEndOfNames (std::string channel) { return (" " + channel + " :End of NAMES list"); }
/* 367 */ std::string	irc::RplBanList (std::string channel, std::string mask) { return (" " + channel + " " + mask); }
/* 368 */ std::string	irc::RplEndOfBanList (std::string channel) { return (" " + channel + " :End of channel ban list"); }
/* 371 */ std::string	irc::RplInfo (std::string infos) { return (" :" + infos); }
/* 372 */ std::string	irc::RplMotd (std::string motd_line) { return (":- " + motd_line); }
// Note : Max 80 char for each motd line
/* 374 */ std::string	irc::RplEndOfInfo () { return (" :End of INFO list"); }
/* 375 */ std::string	irc::RplMotdStart (std::string server) { return (" :- " + server + " Message of the day -"); }
/* 376 */ std::string	irc::RplEndOfMotd () { return (" :End of MOTD command"); }
/* 381 */ std::string	irc::RplYourOper () { return ( " :You are now an IRC operator"); }
// 382 - No need
// 383 - No need
/* 391 */ std::string	irc::RplTime (std::string server, std::string time) { return (" " + server + " :" + time); }
// 392 to 395 - No need

/* ERROR REPLIES */
/* 401 */ std::string	irc::ErrNoSuchNick (std::string nick) { return (" " + nick + " :No suck nick/channel"); }
/* 402 */ std::string	irc::ErrNoSuchServer (std::string server_name) { return (" " + server_name + " :No such server"); }
/* 403 */ std::string	irc::ErrNoSuchChannel (std::string channel_name) { return (" " + channel_name + " :No such channel"); }
/* 404 */ std::string	irc::ErrCannotSendToChan (std::string channel_name) { return (" " + channel_name + " :Cannot send to channel"); }
/* 405 */ std::string	irc::ErrTooManyChannels (std::string channel_name) { return (" " + channel_name + " :You have joined too many channels"); }
// 406 - No need
// 407 - NEED MORE INFOS
// 408 - No nedd
/* 409 */ std::string	irc::ErrNoOrigin () { return (" :No origin specified"); }
/* 411 */ std::string	irc::ErrNoRecipient (std::string command) { return (" : No recicipent given (" + command + ")"); }
/* 412 */ std::string	irc::ErrNoTextToSend () { return (" : No text to send"); }
/* 413 */ std::string	irc::ErrNoTopLevel (std::string mask) { return (" " + mask + " :No toplevel domain specified"); }
/* 414 */ std::string	irc::ErrWildTopLevel (std::string mask) { return (" " + mask + " :Wildcard in toplevel domain"); }
/* 415 */ std::string	irc::ErrBadMask (std::string mask) { return (" " + mask + " :Bad Server/host mask"); }
/* 421 */ std::string	irc::ErrUnknownCommand (std::string command) { return (" " +  command + "  :Unknown command"); }
/* 422 */ std::string	irc::ErrNoMotd () { return (" : MOTD File is missing"); }
/* 423 */ std::string	irc::ErrNoAdminInfo (std::string server) { return (" " + server + " : No administrative info available"); }
/* 424 */ std::string	irc::ErrFileError (std::string file_op, std::string file) { return (" : File error doing " + file_op + " on " + file); }
/* 431 */ std::string	irc::ErrNoNicknameGiven () { return ( " :No nickname given"); }
/* 432 */ std::string	irc::ErrErroneusNickname (std::string nick) { return (" " + nick + " :Erroneous nickname"); }
/* 433 */ std::string	irc::ErrNicknameInUse (std::string nick) { return (" " + nick + " :Nickname is already in use"); }
// 436 - No need
// 437 - No need
/* 441 */ std::string	irc::ErrUserNotInChannel (std::string not_in_nick, std::string channel) { return (" " + not_in_nick + " " + channel +" :They aren't on that channel"); }
/* 442 */ std::string	irc::ErrNotOnChannel (std::string channel) { return (" " + channel + " :You're not on that channel"); }
/* 443 */ std::string	irc::ErrUserOnChannel (std::string user, std::string channel) { return (" " + user + " " + channel +" :is already on channel"); }
/* 444 */ std::string	irc::ErrNoLogin (std::string user) { return (" " + user + " :Not logged in"); }
/* 445 */ std::string	irc::ErrSummonDisabled () { return (" :SUMMON has been disabled"); }
/* 446 */ std::string	irc::ErrUsersDisabled () { return (" :USERS has be disabled"); }
/* 451 */ std::string	irc::ErrNotRegistered () { return (" :You have not registered"); }
/* 461 */ std::string	irc::ErrNeedMoreParams (std::string command) { return (" " + command + " :Not enough parameters"); }
/* 462 */ std::string	irc::ErrAlreadyRegistered () { return (" :Unauthorized command (already registered)"); }
// 463 - No need
/* 464 */ std::string	irc::ErrPasswordMismatch () { return (" : Password incorrect"); }
// 465 - No need
// 466 - No need
/* 467 */ std::string	irc::ErrKeySet (std::string channel) { return (" " + channel + " :Channel key already set"); }
/* 471 */ std::string	irc::ErrChannelIsFull (std::string channel) { return (" " + channel + " :Cannot join channel (+l)"); }
/* 472 */ std::string	irc::ErrUnknownMode (std::string letter, std::string channel) { return (" " + letter + " :is unknown mode char to me for " + channel); }
/* 473 */ std::string	irc::ErrInviteOnlyChan (std::string channel) { return (" " + channel + " :Cannot join channel (+i)"); }
/* 474 */ std::string	irc::ErrBannedFromChan (std::string channel) { return (" " + channel + " :Cannot join channel (+b)"); }
/* 475 */ std::string	irc::ErrBadChannelKey (std::string channel) { return (" " + channel + " :Cannot join channel (+k)"); }
/* 476 */ std::string	irc::ErrBadChanMask (std::string channel) { return (" " + channel + " :Bad Channel Mask"); }
/* 477 */ std::string	irc::ErrNoChanModes (std::string channel) { return (" " + channel + " :Channel doesn't support mode"); }
/* 478 */ std::string	irc::ErrBanListFull (std::string channel, std::string letter) { return (" " + channel + " " + letter + " :Channel list is full"); }
/* 481 */ std::string	irc::ErrNoPrivileges () { return (" : Permission Denied- You're not an IRC operator"); }
/* 482 */ std::string	irc::ErrChanNoPrivsNeeded (std::string channel) { return (" " + channel + " :You're not channel operator"); }
/* 483 */ std::string	irc::ErrCantKillServer () { return (" :You can't kill a server"); }
/* 484 */ std::string	irc::ErrRestricted () { return (" :Your connection is restricted!"); }
// 485 - No need
/* 491 */ std::string	irc::ErrNoOperHost () { return (" :No O-lines for your host"); }
/* 501 */ std::string	irc::ErrUModeUnknownFlag () { return (" :Unknown MODE flag"); }
/* 502 */ std::string	irc::ErrUsersDontMatch () { return (" :Cannot change mode for tohers users"); }