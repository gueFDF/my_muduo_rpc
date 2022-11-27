#include <iostream>
#include "test.pb.h"

using namespace fixbug;

int main()
{
    GetFrendlistResponse scp;
    Resultcode *s = scp.mutable_result();
    s->set_errcode(1);
    s->set_errmsg("nothing")
;    User *p = scp.add_friends_list();
    p->set_age(13);
    p->set_name("zhangsan");
    p->set_sex(User::MAN);

    p = scp.add_friends_list();
    p->set_age(13);
    p->set_name("wangwu");
    p->set_sex(User::WOMAN);
    std::cout << scp.DebugString() << std::endl;
}