/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgourlin <jgourlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 08:29:18 by jgourlin          #+#    #+#             */
/*   Updated: 2022/12/16 17:51:30 by jgourlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    if (parsing(argc, argv))
        return 1;
    signal(SIGINT, Sig);
    // else
    // {
    //     server  test(argv[1], argv[2]);
    // }

    
    std::cout << "END MAIN" << std::endl;
    return (0);
}