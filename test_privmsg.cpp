/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_privmsg.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiacomi <egiacomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 11:27:26 by egiacomi          #+#    #+#             */
/*   Updated: 2022/12/19 19:09:17 by egiacomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <sstream>
# include <unistd.h>
# include <cerrno>
# include <fcntl.h>
# include <cstdlib>
# include <string.h>
# include <poll.h>
# include <unistd.h>
# include <vector>
# include <csignal>
# include <algorithm>
# include <cctype>
# include <cctype>

void    Privmsg_cmd(std::string cmd) {
    std::cout << "COMMANDE -> PRIVMSG" << std::endl;

    // Skip la command & check si il y a des arguments
    size_t pos = cmd.find_first_not_of(" ", 7);
    if (pos == std::string::npos) {
      std::cout << "No Parameters" << std::endl;
      return ;
    }
    std::cout << pos << std::endl;
    
    // Check target
    std::vector<std::string> target;
    size_t end_otarget;
    
    end_otarget = cmd.find(" ", pos);
    target.push_back(cmd.substr(pos, end_otarget - pos));
    std::cout << target[0] << std::endl;
    
    // Check si il y a un message
    size_t strt_omsg = cmd.find_first_not_of(" ", end_otarget);
    if (strt_omsg == std::string::npos) {
      std::cout << "No Message to send" << std::endl;
      return ;
    }
    
    std::stringstream targstream(target[0]);
    std::string newtarget;
    while (std::getline(targstream, newtarget, ',')) {
      target.push_back(newtarget);
    }
    target.erase(target.begin());
    std::vector<std::string>::iterator fst_target = target.begin();
    std::vector<std::string>::iterator lst_target = target.end();     
    std::cout << "Targets are :";
    for (std::vector<std::string>::iterator it = fst_target ; it != lst_target; ++it)
      std::cout << '\n' << *it;
    std::cout << std::endl;
    for (std::vector<std::string>::iterator it = fst_target ; it != lst_target; ++it)
    {
      if ((*it)[0] == '#') {
        std::cout << *it << " : I am a channel" << std::endl; 
        // for (size_t i = 0; i < _list_user.size(); i++) {
        //   if (Compare_case_sensitive(_list_user[i]->Get_username(), *it))
        //     std::cout << "No user with that nickname" << std::endl;
        // }
      }
      else {
        std::cout << *it << " : I am a user" << std::endl; 
        // for (size_t i = 0; i < _list_user.size(); i++) {
        //   if (Compare_case_sensitive(_list_user[i]->Get_username(), *it))
        //     std::cout << "No user with that nickname" << std::endl;
        // }
      }
    }
   // Check if target is good (useror channel)
    
    // Check if message is "ok" (lenght, prefix?)

    // Send message
}

int main(int ac, char ** av)
{
	(void)av;
	if (ac > 1)
	{	
		std::string cmd(av[1]);
		Privmsg_cmd(cmd);
	}
	else
		std::cout << "No cmd" << std::endl;
	return 0;
}

/* std::vector<std::string> split(const std::string& str) {
  std::vector<std::string> result;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss, token, ',')) {
    result.push_back(token);
  }

  return result;
}

int main() {
  std::string str = "hello,world,foo,bar";
  std::vector<std::string> tokens = split(str);

  // tokens should contain {"hello", "world", "foo", "bar"}
} */