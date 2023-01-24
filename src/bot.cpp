#include "../class/utils.hpp"

void    server::_Bot_main(user *sender, t_IRCMessage cmd) 
{
    std::cout << "Speaking with Bot" << std::endl;

    cmd.params[1].erase(0, 1);
    std::cout << "it is : " << cmd.params[1] << std::endl;
    if (Compare_case_sensitive(cmd.params[1], "heure"))
    {
        time_t rawtime;
        struct tm * timeinfo;
        std::string str;

        time (&rawtime);
        timeinfo = localtime (&rawtime);
        _Output_client(sender->Get_fd_client() ,":" + _name_serveur + " 391 " + sender->Get_nickname() + " :\033[36m" + asctime(timeinfo) + "\033[0m");
        std::cout << "Current local time and date: " << asctime(timeinfo) << std::endl;
    }
    else if (Compare_case_sensitive(cmd.params[1], "liste"))
    {
        std::string str;
        std::vector<channel*>::iterator it;
        for (it = _list_channel.begin(); it != _list_channel.end(); it++)
            str += "\033[31m" + (*it)->Get_channel_name() + " : " + "\033[36m" + (*it)->Get_channel_topic() + "\033[0m\n";
        std::cout << str << std::endl;
        if (str.empty())
            str = "\033[33mNo Channel in server\033[0m";
        _Output_client(sender->Get_fd_client(), str);
    }
    else if (Compare_case_sensitive(cmd.params[1], "help"))
        _Output_client(sender->Get_fd_client(), BOT_HELP_TEXT(sender->Get_nickname()));
    else if (Compare_case_sensitive(cmd.params[1], "bonjour") || Compare_case_sensitive(cmd.params[1], "hello"))
    {
        srand(time(0));
        _Output_client(sender->Get_fd_client(), BOT_GREETING_TEXT);
    }   
    else
        _Output_client(sender->Get_fd_client(), BOT_NO_PARAMS);
};