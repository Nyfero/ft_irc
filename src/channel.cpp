# include "../class/channel.hpp"

channel::channel(){}

channel::channel(std::string name, user creator):
_name(name)
{
    (void) creator;
    //_op.push_back(new user(creator))
}


channel::~channel()
{

}
