# include "../class/utils.hpp"

extern bool g_stop;

void    Sig(int sig) {
    (void) sig;
    g_stop = 0;
}

int main(int argc, char **argv) {

    if (parsing(argc, argv)) {
        return 1;
    }
    signal(SIGINT, Sig);
    server  irc(argv[1], argv[2]);

    return 0;
};