/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiacomi <egiacomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 23:42:40 by egiacomi          #+#    #+#             */
/*   Updated: 2023/01/20 23:42:41 by egiacomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../class/utils.hpp"

extern bool g_stop;

void    Sig(int sig) {
    (void) sig;
    g_stop = 0;
}

int main(int argc, char **argv) {
    std::cout << "BEGIN MAIN" << std::endl;

    if (parsing(argc, argv)) {
        return 1;
    }
    signal(SIGINT, Sig);
    server  irc(argv[1], argv[2]);

    std::cout << "END MAIN" << std::endl;
    return 0;
};