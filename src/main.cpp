# include "../class/utils.hpp"

extern bool g_stop;

void    Sig(int sig)
{
    (void) sig;
    g_stop = 0;
}

int main(int argc, char **argv)
{
    std::cout << "BEGIN MAIN" << std::endl;
    signal(SIGINT, Sig);
    if (parsing(argc, argv))
        return 1;
    // else
    // {
    //     server  test(argv[1], argv[2]);
    // }

    
    std::cout << "END MAIN" << std::endl;
    return (0);
}