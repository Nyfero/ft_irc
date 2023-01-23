#include "../class/utils.hpp"
# include <ctime>

void    server::_Bot_main(user *sender, struct s_IRCMessage cmd) {
    // heure
    (void)sender;
    (void)cmd;

    // :hash.ix.nl.dal.net 391 nick42 hash.ix.nl.dal.net 1674434608 :Monday January 23 2023 -- 00:43 +00:00

    //struct tm * localtime(const time_t * timer);
    
    time_t rawtime, tmp;
    struct tm * timeinfo;
    std::string str;
    char c;


    time (&rawtime);
    std::cout << "rawtime = " << rawtime << std::endl;
    tmp = rawtime;
    while (tmp > 9)
    {
        c = tmp % 10 + 48;
        str.insert(str.begin(), c);
        tmp = tmp / 10;
    }
    c = tmp % 10 + 48;
    str.insert(str.begin(), c);
    std::cout << "str = " << str << std::endl;

    timeinfo = localtime (&rawtime);
    _Output_client(sender->Get_fd_client() ,":" + _name_serveur + " 391 " + sender->Get_nickname() + " :\033[36m" + asctime(timeinfo) + "\033[0m");
    std::cout << "Current local time and date: " << asctime(timeinfo) << std::endl;

    //\033[36m[DiamondTrap] Named Constructor\033[0m

}