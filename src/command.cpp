# include "../class/server.hpp"

void    server::Enter(user *user, std::string str)
{
    std::cout << "*** server::ENTER + ***" << std::endl;
    // recoit line ex: "je suis la command\0"

    std::cout << "fd uer: " <<user->Get_fd_client() << std::endl;
    std::cout << "str: " << str << std::endl;
    std::cout << "*** server::ENTER - ***" << std::endl;
}

// join with key
void    server::join(user *user, std::string chan, std::string key)
{
    (void) user;
    (void) chan;
    (void) key;
//  JOIN &foo fubar                 ; server to join channel &foo using key "fubar".//  JOIN #foo,&bar fubar            ; server to join channel #foo using key "fubar" and &bar using no key.
//  JOIN #foo,#bar fubar,foobar     ; server to join channel #foo using key "fubar", and channel #bar using key "foobar".
}

// join with only channel
void    server::join(user *user, std::string chan)
{
    (void) user;
    (void) chan;
//  JOIN #foobar                    ; server to join channel #foobar.
//  JOIN #foo,#bar                  ; server to join channels #foo and #bar.
}


//  JOIN 0                          
void    server::join(user *user) // Leave all currently joined channels.
{
    (void) user;
}

int  server::pass(user *user, std::string pass) {
    (void) user;
    (void) pass;
    return 0;
};

void    server::nick(user *user, std::string nick)
{
    (void) nick;
    (void) user;
    //check nick
    // ou appeler fonction de user pour changer nick
}