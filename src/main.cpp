# include "../class/utils.hpp"

int main(int argc, char **argv)
{
    std::cout << "BEGIN MAIN" << std::endl;
    if (parsing(argc, argv))
        return 1;
    // else
    // {
    //     server  test(argv[1], argv[2]);
    // }

    
    std::cout << "END MAIN" << std::endl;
    return (0);
}