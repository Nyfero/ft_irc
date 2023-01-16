/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiacomi <egiacomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 17:54:33 by egiacomi          #+#    #+#             */
/*   Updated: 2023/01/14 20:03:26 by egiacomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../class/server.hpp"
#include "../class/utils.hpp"
#include "../class/user.hpp"

bool	server::_parse_notice_wallops(t_IRCMessage cmd)
{
    if (cmd.params.empty()) {					// Check if command takes parameters
        std::cout << "ERROR : No parameter from NOTICE/WALLOPS" << std::endl;	
        return true;
	}
	if (cmd.params.size() == 1)	{				// Check if I have minimum 2 parameters (target + message)
        std::cout << "ERROR : Not enough from NOTICE/WALLOPS" << std::endl;	
        return true;
	}
    if (cmd.params[0][0] == ':') {				// Check if any target is specified
        std::cout << "ERROR : No targets from NOTICE/WALLOPS" << std::endl;	
        return true;
	}
    if (cmd.params[1][0] != ':') {				// Check message start with a ":"
        std::cout << "ERROR : No message starting with ':' from NOTICE/WALLOPS" << std::endl;	
        return true;
	}
	return false;
}

bool	server::_add_user_targetfds_notice(std::vector<int> *targets_fds, std::string target)
{
	for (size_t i = 0; i < _list_user.size(); i++)
	{
		if (Compare_case_sensitive(_list_user[i]->Get_nickname(), target))			// Check if username exists
		{
			if (_list_user[i]->Get_mode().Get_away() == false)						// Check if user is away
				targets_fds->push_back(_list_user[i]->Get_fd_client());
			return false;
		}
	}
	return true;
}

bool	server::_add_channel_targetfds_notice(user *sender, std::vector<int> *targets_fds, std::string target)
{
	for (size_t i = 0; i < _list_channel.size(); i++)
	{
		std::string channel_test = _list_channel[i]->Get_channel_name();
		if (Compare_case_sensitive(channel_test, target))
		{
			if (User_in_channel(sender, _list_channel[i]))											// Check if User is in the Channel
			{
				std::vector<user *> channel_users = _list_channel[i]->Get_list_channel_user();
				for (size_t i = 0; i < channel_users.size(); i++)									// Add all channel user's fd
					targets_fds->push_back(channel_users[i]->Get_fd_client());
			}
			return false;
		}
	}	
	return true;
}

std::vector<int> server::_targetfds_creator_notice(user *sender, std::vector<std::string> target)
{
	std::vector<int> targets_fds;
    for (std::vector<std::string>::iterator it = target.begin(); it != target.end(); ++it)
	{
        if ((*it)[0] == '#')
		{
			if (_list_channel.empty())
			{
                std::cout << "ERROR : There are no channels" << std::endl;
                continue;
			}
			else
				_add_channel_targetfds_notice(sender, &targets_fds, *it);
		}
		else
			_add_user_targetfds_notice(&targets_fds, *it);
	}
	return (targets_fds);
}
