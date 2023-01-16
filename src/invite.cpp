/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiacomi <egiacomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 03:52:17 by egiacomi          #+#    #+#             */
/*   Updated: 2023/01/16 16:15:01 by egiacomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../class/server.hpp"
#include "../class/utils.hpp"
#include "../class/user.hpp"

bool server::_parse_invite(user *sender, t_IRCMessage cmd)
{
    if (cmd.params.empty())																				// Check if command takes parameters
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return true;
    }
	if (cmd.params.size() < 2)																			// Check if I have minimum 2 parameters (target + channel)
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return true;
    }
    if (Check_valid_channel(cmd.params[1]) == false)													// Check if 2nd parameter is a Channel
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return true;
    }
	return false;
}

user *server::_check_nick_invite(user *sender, t_IRCMessage cmd)
{
	for (size_t i = 0; i < _list_user.size(); i++)
	{
		if (Compare_case_sensitive(_list_user[i]->Get_nickname(), cmd.params[0]))	// Check if nickname exists
		{
			if (_list_user[i]->Get_mode().Get_away())								// Check if user is away, if so : send away reply
			{
				_Output_client(sender->Get_fd_client(), RPL_AWAY(_name_serveur, sender->Get_nickname(), _list_user[i]->Get_nickname(), _list_user[i]->Get_mode().Get_away_reply()));
				return NULL;
			}
			else
				return _list_user[i];												// return user if he's not away
		}
	}
	_Output_client(sender->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur, cmd.params[0]));
	return NULL;																	// if no user to add return NULL;
};

channel *server::_check_chan_invite(user *sender, struct s_IRCMessage cmd)
{
	for (size_t i = 0; i < _list_channel.size(); i++)
	{
		std::string channel_test = _list_channel[i]->Get_channel_name();
		if (Compare_case_sensitive(channel_test, cmd.params[1]))					// Check if channel already exist
		{
			if (User_in_channel(sender, _list_channel[i]))							// Check if user is in the channel (HERE I CAN ADD IF PRIVILEGES)
				return _list_channel[i];
			else
			{
				_Output_client(sender->Get_fd_client(), ERR_NOTONCHANNEL(_name_serveur, channel_test));
				return NULL;
			}
		}
	}
	// TODO : Create a channel; Addsender in channel;
	channel *NewChannel = NULL;
	return NewChannel;
}

bool	server::_user_already_member(user *target_nick, channel *target_chan)
{
	std::vector<user *> channel_users = target_chan->Get_list_channel_user();
	for (size_t i = 0; i < channel_users.size(); i++)										// Add all channel_users_fd
	{
		if (Compare_case_sensitive(channel_users[i]->Get_nickname(), target_nick->Get_nickname()))
			return true;
	}
	return false;
}