/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 11:57:18 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/09 09:44:06 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../class/utils.hpp"
# include "../class/user.hpp"

user::user(){}

user::user(int  socket, addrinfo info):
_addrinfo_cli(info)
{
    (void) _addrinfo_cli;
    _fd_poll.fd = socket,
    _fd_poll.revents = 0,
    _fd_poll.events = POLLIN,
    std::cout << "User create: " << _fd_poll.fd << std::endl;
}

user::~user()
{
    std::cout << "User destroy: " << _fd_poll.fd << std::endl;
    close(_fd_poll.fd);
}

