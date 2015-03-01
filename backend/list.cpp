#include "list.h"

using namespace std;
using namespace std::tr1;

ListHead::~ListHead()
{
    if(prev) {
        prev->next = next;
    }
    if(next) {
        next->prev = prev;
    }
}

void ListHead::insert(ListHead &n)
{
    shared_ptr<ListHead> tail(&n);
    while(tail->next) {
        tail = tail->next;
    }
    tail->next = this->next;
    if(this->next) {
        this->next->prev = tail;
    }
    this->next.reset(&n);
    //TODO
}
