#include "../class/utils.hpp"
# include <ctime>

# define RED__ "\033[31m"
# define GREEN "\033[32m"
# define YELLO "\033[33m"
# define BLUE_ "\033[34m"
# define PURPL "\033[35m"
# define CYAN_ "\033[36m"
# define GREY_ "\033[37m"
# define DEFAU "\033[0m"


void    server::_Bot_main(user *sender, struct s_IRCMessage cmd) {
    // heure
    (void)cmd;
    (void)sender;

    cmd.params[1].erase(0);
    if (cmd.params[0] == "@heure")
    {
        time_t rawtime;
        struct tm * timeinfo;
        std::string str;

        time (&rawtime);
        timeinfo = localtime (&rawtime);
        _Output_client(sender->Get_fd_client() ,":" + _name_serveur + " 391 " + sender->Get_nickname() + " :\033[36m" + asctime(timeinfo) + "\033[0m");
        std::cout << "Current local time and date: " << asctime(timeinfo) << std::endl;
    }
    else if (cmd.params[0] == "@liste")
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
    else
        _Output_client(sender->Get_fd_client(), "\033[35mSend message to @heure or @liste\033[0m");
};