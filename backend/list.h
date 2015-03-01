#ifndef BACKEND_LIST_H

#define BACKEND_LIST_H

#include <tr1/memory>

class ListHead {
public:
    ListHead() {}
    ListHead(std::tr1::shared_ptr<ListHead> p, std::tr1::shared_ptr<ListHead> n) :
        prev(p), next(n) {}
    std::tr1::shared_ptr<ListHead> getNext() {
        return next;
    }
    std::tr1::shared_ptr<ListHead> getPrev() {
        return prev;
    }
    std::tr1::shared_ptr<const ListHead> getNext() const {
        return next;
    }
    std::tr1::shared_ptr<const ListHead> getPrev() const {
        return prev;
    }
    void insert(ListHead &);
    virtual ~ListHead();
private:
    std::tr1::shared_ptr<ListHead> prev, next;
};

#endif
