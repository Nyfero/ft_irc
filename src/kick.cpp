#include "../class/utils.hpp"

bool server::_parse_kick(user *sender, t_IRCMessage cmd)
{
    if (cmd.params.empty())																				// Check if command takes parameters
    {
		_Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "KICK"));
        return true;
    }
	if (cmd.params.size() < 2)																			// Check if I have minimum 2 parameters (target + channel)
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "KICK"));
        return true;
    }
	if (cmd.params.size() > 2) {
		if (cmd.params[2].at(0) != ':') {
			_Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "KICK"));				// Check si le kick msg contient :
        	return true;
		}
	}
	return false;
}

user *server::_check_nick_kick(user *sender, t_IRCMessage cmd)
{
	for (size_t i = 0; i < _list_user.size(); i++)
	{
		if (Compare_case_sensitive(_list_user[i]->Get_nickname(), cmd.params[1]))	// Check if nickname exists
			return _list_user[i];													// return user
	}
	_Output_client(sender->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur, sender->Get_nickname(), cmd.params[0]));
	return NULL;																	// if no user to add return NULL;
};

channel *server::_chan_kick_creator(user *sender, std::string chan_to_kick)
{
    if (Check_valid_channel(chan_to_kick) == false)									// Check if its a valid Channel name
    {
        _Output_client(sender->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur, sender->Get_nickname(), chan_to_kick));
        return NULL;
    }	
	for (size_t i = 0; i < _list_channel.size(); i++)
	{
		if (Compare_case_sensitive(_list_channel[i]->Get_channel_name(), chan_to_kick))
		{
			if (User_in_channel(sender, _list_channel[i]))							// Check if user is in the channel
			{
				if (sender->Is_op_channel(_list_channel[i]) == false)				// Check if the server is not Operator
				{
					_Output_client(sender->Get_fd_client(), ERR_CHANOPRIVSNEEDED(_name_serveur, sender->Get_nickname(), _list_channel[i]->Get_channel_name())); // If not Channel operator, send error message
					return NULL;
				}
				return _list_channel[i];
			}
			else
			{
				_Output_client(sender->Get_fd_client(), ERR_NOTONCHANNEL(_name_serveur, chan_to_kick));	// If user is not member of the channel
				return NULL;
			}
		}
	}
    _Output_client(sender->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, sender->Get_nickname(), chan_to_kick)); // If channel does not exists
	return NULL;
}

channel *server::_user_not_in_channel(user *sender, user *target_nick, channel *channel_kick)
{
	if (channel_kick)
	{
		std::vector<channel *> target_channels_member = target_nick->Get_channel_register();
		for (size_t i = 0; i < target_channels_member.size(); i++)										// Add all channel_users_fd
		{
			if (Compare_case_sensitive(target_channels_member[i]->Get_channel_name(), channel_kick->Get_channel_name()))
				return channel_kick;
		}
		_Output_client(sender->Get_fd_client(), ERR_USERNOTINCHANNEL(_name_serveur, sender->Get_nickname(), target_nick->Get_nickname(), channel_kick->Get_channel_name()));
	}
	return NULL;
}

std::vector<channel *> server::_split_channels_kick(user *sender, t_IRCMessage cmd)
{
	std::vector<std::string> 	chan_lst_kick;
	std::stringstream			chanstream(cmd.params[0]);
	std::string 				chantarget;
    while (std::getline(chanstream, chantarget, ','))
	{
    	chan_lst_kick.push_back(chantarget);										// Split chan parameter
	}
	std::vector<channel *> targeted_chan;
	for (size_t i = 0; i < chan_lst_kick.size(); i++)
		targeted_chan.push_back(_chan_kick_creator(sender, chan_lst_kick[i]));		// Push back channel* if its correct or NULL if its an error
	return (targeted_chan);
}

bool server::_kick_from_channel(user *target_nick, channel *channel_kick)
{
	if (channel_kick != NULL)
	{
		target_nick->Remove_Channel(channel_kick);
		channel_kick->Remove_user(target_nick);
		if (channel_kick->Get_list_channel_user().empty())
		{
			_Remove_channel(channel_kick);
			return false;
		}
		return true;
	}
	return false;
}

void server::_kick_success_message(user *target_nick, channel *channel_kick, t_IRCMessage cmd)
{
	std::string kicked_message_channel = cmd.prefix + " " + cmd.command + " " + channel_kick->Get_channel_name() + " " + cmd.params[1];

	if (cmd.params.size() < 2) {
		kicked_message_channel.append(":" + cmd.prefix);
	}
	else
	{
		std::string note = Join(cmd.params, 2, cmd.params.size());
		kicked_message_channel.append(" " + note);
	}
	_Output_channel(channel_kick, kicked_message_channel);
	_Output_client(target_nick->Get_fd_client(), kicked_message_channel);
}
