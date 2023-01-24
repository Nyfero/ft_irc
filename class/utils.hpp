#pragma once

# include <sys/socket.h>
# include <iostream>
# include <unistd.h>
# include <cerrno>
# include <fcntl.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <cstdlib>
# include <string.h>
# include <sstream>
# include <poll.h>
# include <vector>
# include <csignal>
# include <algorithm>
# include <cctype>
# include <ctime>

# include "channel.hpp"
# include "error_msg.hpp"
# include "mode.hpp"
# include "server.hpp"
# include "user.hpp"

typedef struct s_IRCMessage {
    std::string prefix;
    std::string command;
    std::vector<std::string> params;
}				t_IRCMessage;

// Fonctions de parsing
int     parsing(int argc, char **argv);
bool    isNumber(const std::string& s);
bool    Compare_case_sensitive(std::string str1, std::string str2);
int     Stoi(std::string str);
std::string Itos(int nb);

// Fonctions pour les channels
int     Check_valid_channel(std::string str);
bool	User_in_channel(user *user, channel *chan);
bool	User_in_channel_is_op(user *user, channel *chan);
user	*Get_user_in_channel(user *use, channel *chan);

// Fonctions utiles pour les commandes
t_IRCMessage                split_message(user *user, std::string& input);
std::vector<std::string>    Split(std::string str, char delimiter);
std::string		            Join(std::vector<std::string> vec, size_t start, size_t end);
bool                        isRestricted(user *sender);
std::string                 Create_names_rpl(channel *chan);
std::string                 Create_aff_mode(std::string str, char c);

