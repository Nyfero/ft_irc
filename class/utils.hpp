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

# include "user.hpp"
# include "channel.hpp"


// # include "channel.hpp"
// # include "user.hpp"

# include <vector>
# include "user.hpp"
# include "server.hpp"
# include "command.hpp"

int     parsing(int argc, char **argv);
bool    isNumber(const std::string& s);

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
