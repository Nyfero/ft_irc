#include "../class/utils.hpp"

bool server::_parse_invite(user *sender, t_IRCMessage cmd)
{
    if (cmd.params.empty() || cmd.params.size() < 2)												// Check if command takes parameters
    {
		_Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "INVITE"));
        return true;
    }
    if (Check_valid_channel(cmd.params[1]) == false)													// Check if 2nd parameter is a Channel
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "INVITE"));
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
			if (_list_user[i]->Get_mode()->Get_away())								// Check if user is away, if so : send away reply
			{
				_Output_client(sender->Get_fd_client(), RPL_AWAY(_name_serveur, sender->Get_nickname(), _list_user[i]->Get_nickname(), _list_user[i]->Get_mode()->Get_away_reply()));
				return NULL;
			}
			else
				return _list_user[i];												// return user if he's not away
		}
	}
	_Output_client(sender->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur, sender->Get_nickname(), cmd.params[0]));
	return NULL;																	// if no user to add return NULL;
};

channel *server::_check_chan_invite(user *sender, struct s_IRCMessage cmd)
{
	for (size_t i = 0; i < _list_channel.size(); i++)
	{
		std::string channel_test = _list_channel[i]->Get_channel_name();
		if (Compare_case_sensitive(channel_test, cmd.params[1]))							// Check if channel already exist
		{
			if (User_in_channel(sender, _list_channel[i]))									// Check if user is in the channel
			{
				if (_list_channel[i]->Get_invite_only())									// Check if the server is Invite-Only Mode
				{
					if (sender->Is_op_channel(_list_channel[i]) == false)					// Check if the server is not Operator
					{
						_Output_client(sender->Get_fd_client(), ERR_CHANOPRIVSNEEDED(_name_serveur, sender->Get_nickname(), channel_test)); // If not Channel operator, send error message
						return NULL;
					}
				}
				return _list_channel[i];													// Return the correct channel target
			}
			else
			{
				_Output_client(sender->Get_fd_client(), ERR_NOTONCHANNEL(_name_serveur, channel_test));	// Channel exists but user is not member
				return NULL;
			}
		}
	}
	_Output_client(sender->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, sender->Get_nickname(), cmd.params[1]));	// Send an error if channel does not exists
	return NULL;
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

void	server::_invite_success(user *sender, user *target_nick, channel *target_chan, s_IRCMessage cmd)
{
	target_chan->Add_invited_user(target_nick);	
	_Output_client(sender->Get_fd_client(), RPL_INVITING(_name_serveur, sender->Get_nickname(), target_nick->Get_nickname(), target_chan->Get_channel_name()));
	std::string invited_message = cmd.prefix + " " + cmd.command + " " + target_nick->Get_nickname() + " :" + target_chan->Get_channel_name();	
	std::cout << invited_message << std::endl;
	_Output_client(target_nick->Get_fd_client(), invited_message);
}