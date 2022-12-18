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
# include <poll.h>
# include <unistd.h>
# include <vector>
# include <csignal>
# include <algorithm>
# include <cctype>
# include "user.hpp"
# include "channel.hpp"

# include <vector>
# include "user.hpp"
# include "server.hpp"
# include "error_msg.hpp"

int     parsing(int argc, char **argv);
bool    isNumber(const std::string& s);
bool    Compare_case_sensitive(std::string str1, std::string str2);
int Stoi(std::string str);

// int socket(int domain, int type, int protocol);
// int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen);
// int listen(int sockfd, int backlog);
// int accept(int sockfd, struct sockaddr *adresse, socklen_t *longueur);

// struct sockaddr_in {
//     short            sin_family;   // e.g: AF_INET
//     unsigned short   sin_port;     // e.g: htons(3490)
//     struct in_addr   sin_addr;     // détaillé ci-dessous
//     char             sin_zero[8];  // '0' habituellement
// };
// struct in_addr {
//     unsigned long s_addr;  // initialiser avec inet_aton()
// };


// struct addrinfo {
//     int              ai_flags;
//     int              ai_family;
//     int              ai_socktype;
//     int              ai_protocol;
//     size_t           ai_addrlen;
//     struct sockaddr *ai_addr;
//     char            *ai_canonname;
//     struct addrinfo *ai_next;
// };

// struct pollfd {
//     int   fd;         /* Descripteur de fichier */
//     short events;     /* Événements attendus    */
//     short revents;    /* Événements détectés    */
// };


// PASS <password>
// USER <user> <mode> <unused> <realname>
// NICK <user> <mode> <unused> <realname>
// MODE <nickname> ( ( "+" / "-" )( "i" / "w" / "o" / "O" / "r" ) )
// MODE <channel> ( ( "-" / "+" )<modes> <modeparams> )
// QUIT <Quit Message>
// JOIN (<channel>( "," <channel> ) [ <key> ( "," <key> ) ] ) / "0"
// PART <channel>( "," <channel> ) [ <Part Message> ]
// NAMES [ <channel> ( "," <channel> ) [ <target> ] ]
// INVITE <nickname> <channel>
// KICK <channel>( "," <channel> ) <user> *( "," <user> ) [<comment>]
// PRIVMSG <msgtarget> <text to be sent>
// NOTICE <msgtarget> <text>
// AWAY [ <text> ]
// USERS [ <target> ]
// WALLOPS <Text to be sent> 