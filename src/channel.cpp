# include "../class/channel.hpp"

/***************************/
/******* CONSTRUCTOR *******/
/***************************/

channel::channel(){}

channel::channel(std::string name, user *creator):
_name(name)
{
    _user.push_back(creator);
    _op.push_back(creator);
}

/**************************/
/******* DESTRUCTOR *******/
/**************************/

channel::~channel()
{

}


/**************/
/****  MOD ****/
/**************/

/************************/
/******* ACCESSOR *******/
/************************/

std::string channel::Get_name()
{return(_name);}

std::vector<user *> channel::Get_user()
{return(_user);}

std::vector<user *> channel::Get_op()
{return(_op);}

