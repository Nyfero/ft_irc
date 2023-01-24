#include "../class/utils.hpp"

std::vector<std::string> server::_split_channel_list(std::string target)
{
	std::vector<std::string> 	chan_lst;
	std::stringstream			chanstream(target);
	std::string 				chantarget;
    while (std::getline(chanstream, chantarget, ','))
	{
    	chan_lst.push_back(chantarget);			// Split chan parameter
	}
	return (chan_lst);	
}

channel *server::_filter_channel_list(std::string target_splited)
{
	for (size_t i = 0; i < _list_channel.size(); i++)
	{
		if (Compare_case_sensitive(_list_channel[i]->Get_channel_name(), target_splited)) {
			return _list_channel[i];  						// Push back channel* if it exist
		}
	}
	return NULL;
}

void server::_list_display(user *sender, std::vector<channel *> channel_targeted)
{
	_Output_client(sender->Get_fd_client(), RPL_LISTSTART(_name_serveur, sender->Get_nickname()));
	if (channel_targeted.empty())
	{	
		for (size_t i = 0; i < _list_channel.size(); i++) { 
		std::stringstream ss;
		ss << ":" << _name_serveur << " 322 " << sender->Get_nickname() << " " << _list_channel[i]->Get_channel_name() << " " << _list_channel[i]->Get_list_channel_user().size() << " :" << _list_channel[i]->Get_channel_topic();
		_Output_client(sender->Get_fd_client(), ss.str());
		}
	}
	else
	{
		for (size_t i = 0; i < channel_targeted.size(); i++) {
			if (channel_targeted[i] == NULL)
				continue;
			for (size_t j = 0; j < _list_channel.size(); j++) {
				if (Compare_case_sensitive(_list_channel[j]->Get_channel_name(), channel_targeted[i]->Get_channel_name()))
				{
					std::stringstream ss;
					ss << ":" << _name_serveur << " 322 " << sender->Get_nickname() << " " << _list_channel[j]->Get_channel_name() << " " << _list_channel[j]->Get_list_channel_user().size() << " :" << _list_channel[j]->Get_channel_topic();
					_Output_client(sender->Get_fd_client(), ss.str());
				}
			}
		}
	}
	_Output_client(sender->Get_fd_client(), RPL_LISTEND(_name_serveur, sender->Get_nickname()));
}
