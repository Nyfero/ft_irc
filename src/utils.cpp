#include "../class/utils.hpp"

int parsing(int argc, char **argv) {


    /*
    **  3 Arg
    */

    if (argc != 3) {
        std::cerr << "The executable will run as follows:\n\t./ircserv <port> <password>" << std::endl;
        return 1;
    }

    /*
    **  Port only contain number (0 - 9)
    **  Port's range (1024 - 65535) 16-bits
    */
   
    std::string valid_port(argv[1]);
    if (!isNumber(valid_port)) {
        std::cerr << "The port must be an int" << std::endl;
        return 1;
    }
    int range_port = std::atoi(argv[1]);
    if (range_port < 1024 || range_port > 65535) {
        std::cerr << "The port's range must be between 1024 and 65535" << std::endl;
        return 1;
    }

    /*
    **  password lenght: 6 - 12 char
    **  only 0-9 a-z A-Z
    */
   
    std::string password(argv[2]);
    if (password.length() < 6 || password.length() > 12) {
        std::cerr << "Password's lenght must be between 6 and 12" << std::endl;
        return 1;
    }
    for(size_t i = 0; i < password.length(); i++) {
		if (!isalnum(password[i])) {
			std::cerr << "Password doesn't contains only alphanumerics characters :\n(0 - 9)\t(a - z)\t(A - Z)" << std::endl;
			return 1;
		}
    }
    return 0;
};

bool isNumber(const std::string& str) {
    for (size_t i = 0; i < (str.size()); i++) {
        if (std::isdigit(str[i]) == 0)
            return false;
    }
    return true;
};