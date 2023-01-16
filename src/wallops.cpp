/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wallops.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiacomi <egiacomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 17:54:33 by egiacomi          #+#    #+#             */
/*   Updated: 2023/01/14 20:06:33 by egiacomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../class/server.hpp"
#include "../class/utils.hpp"
#include "../class/user.hpp"

bool	server::_add_channel_targetfds_wallops(user *sender, std::vector<int> *targets_fds, std::string target)
{
	for (size_t i = 0; i < _list_channel.size(); i++)
	{
		std::string channel_test = _list_channel[i]->Get_channel_name();
		if (Compare_case_sensitive(channel_test, target))
		{
			if (User_in_channel(sender, _list_channel[i]))											// Check if User is in the Channel
			{
				std::vector<user *> channel_users = _list_channel[i]->Get_list_channel_user();
				for (size_t i = 0; i < channel_users.size(); i++) {									// Add all channel user's fd if they have the WallOps Mode
                    if (channel_users[i]->Get_mode().Get_wallops()) {
						targets_fds->push_back(channel_users[i]->Get_fd_client());
					}
				}
			}
			return false;
		}
	}	
	return true;
}

std::vector<int> server::_targetfds_creator_wallops(user *sender, std::vector<std::string> target)
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
				_add_channel_targetfds_wallops(sender, &targets_fds, *it);
		}
	}
	return (targets_fds);
}
