#include "../class/utils.hpp"

bool	server::_parse_privmsg(user *sender, t_IRCMessage cmd)
{
    if (cmd.params.empty())																				// Check if command takes parameters
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return true;
    }
	if (cmd.params.size() == 1)																			// Check if I have minimum 2 parameters (target + message)
    {
        _Output_client(sender->Get_fd_client(), ERR_NEEDMOREPARAMS(_name_serveur, "PRIVMSG"));
        return true;
    }
    if (cmd.params[0][0] == ':')																		// Check if any target is specified
    {
        _Output_client(sender->Get_fd_client(), ERR_NORECIPIENT(_name_serveur));
        return true;
    }
    if (cmd.params[1][0] != ':')																		// Check message start with a ":"
    {
        _Output_client(sender->Get_fd_client(), ERR_NOTEXTTOSEND(_name_serveur));
        return true;
    }	
	return false;
}

std::vector<std::string> server::_target_handle(t_IRCMessage cmd)
{
    std::vector<std::string> target;
    target.push_back(cmd.params[0]);

    std::stringstream targstream(target[0]);
    std::string newtarget;
    while (std::getline(targstream, newtarget, ','))
    {
        target.push_back(newtarget);
    }
    target.erase(target.begin());	
	return (target);
}

bool	server::_add_user_targetfds_privmsg(user *sender, std::vector<int> *targets_fds, std::string target)
{
	for (size_t i = 0; i < _list_user.size(); i++)
	{
		if (Compare_case_sensitive(_list_user[i]->Get_nickname(), target))								// Check if username exists
		{
			if (_list_user[i]->Get_mode()->Get_away())													// Check if user is away, if so : send away reply
				_Output_client(sender->Get_fd_client(), RPL_AWAY(_name_serveur, sender->Get_nickname(), _list_user[i]->Get_nickname(), _list_user[i]->Get_mode()->Get_away_reply()));
			targets_fds->push_back(_list_user[i]->Get_fd_client());										// Add user_fd
			return false;
		}
	}
	_Output_client(sender->Get_fd_client(), ERR_NOSUCHNICK(_name_serveur, sender->Get_nickname(), target));
	return true;
}

bool	server::_add_channel_targetfds_privmsg(user *sender, std::vector<int> *targets_fds, std::string target)
{
	for (size_t i = 0; i < _list_channel.size(); i++)
	{
		std::string channel_test = _list_channel[i]->Get_channel_name();
		if (Compare_case_sensitive(channel_test, target))
		{
			if (User_in_channel(sender, _list_channel[i]))												// Check is user is in the channel
			{
				std::vector<user *> channel_users = _list_channel[i]->Get_list_channel_user();
				for (size_t i = 0; i < channel_users.size(); i++)										// Add all channel_users_fd
				{
					if (channel_users[i]->Get_fd_client() != sender->Get_fd_client())
						targets_fds->push_back(channel_users[i]->Get_fd_client());
				}
			}
			else
				_Output_client(sender->Get_fd_client(), ERR_CANNOTSENDTOCHAN(_name_serveur, channel_test));
			return false;
		}
	}	
	_Output_client(sender->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, sender->Get_nickname(), target));
	return true;
}

std::vector<int> server::_targetfds_creator_privmsg(user *sender, std::vector<std::string> target)
{
	std::vector<int> targets_fds;
    for (std::vector<std::string>::iterator it = target.begin(); it != target.end(); ++it)
	{
		if (Check_valid_channel(*it))
		{
			if (_list_channel.empty())
            {
                _Output_client(sender->Get_fd_client(), ERR_NOSUCHCHANNEL(_name_serveur, sender->Get_nickname(), *it));
                continue;
            }
			else
				_add_channel_targetfds_privmsg(sender, &targets_fds, *it);
		}
		else
			_add_user_targetfds_privmsg(sender, &targets_fds, *it);
	}
	return (targets_fds);
}

std::string server::_create_msg(t_IRCMessage cmd)
{
	std::string message = cmd.prefix + " " + cmd.command + " " + cmd.params[0];	
    for (size_t i = 1; i < cmd.params.size(); i++) {
        message += " " + cmd.params[i];
	}
	std::cout << message << std::endl;
	return message;
}