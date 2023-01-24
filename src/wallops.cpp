#include "../class/utils.hpp"

bool	server::_parse_wallops(t_IRCMessage cmd)
{
    if (cmd.params.empty()) {					// Check if command takes parameters
        std::cerr << "ERROR : No parameter from WALLOPS" << std::endl;	
        return true;
	}
    if (cmd.params[0][0] != ':') {				// Check if any target is specified
        std::cerr << "ERROR : No message starting with ':' from WALLOPS" << std::endl;	
        return true;
	}
	return false;
}

std::vector<int> server::_targetfds_creator_wallops(void)
{
	std::vector<int> targets_fds;
	for (size_t i = 0; i < _list_user.size(); i++)
	{
		if (_list_user[i]->Get_mode()->Get_wallops())
			targets_fds.push_back(_list_user[i]->Get_fd_client());
	}
	return (targets_fds);
}
