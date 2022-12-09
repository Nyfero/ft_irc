/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 08:29:18 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/09 08:35:07 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../class/utils.hpp"
# include "../class/server.hpp"

int ft_parsing(char *port)
{
    (void)port;
    std::cout << "*** PARSING PORT ***" << std::endl;
    std::cout << "port = " << port << std::endl;
    return (0);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    std::cout << "BEGIN MAIN" << std::endl;
    if (argc != 3 || ft_parsing(argv[1]))
    {
        std::cout << "Need 3 arg or port incorrect" << std::endl;
    }
    else
    {
        server  test(argv[1], argv[2]);
    }

    
    std::cout << "END MAIN" << std::endl;
    return (0);
}