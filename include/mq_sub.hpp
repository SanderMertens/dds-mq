
#include "mq_DCPS.hpp"

namespace dds {
namespace mq {

class AnyReader {
public:
    AnyReader();
    virtual void take();
};

class Subscriber {
public:
    Subscriber(domain::DomainParticipant dp, std::string name);
};

template <class T>
class Handler {
    Handler() {}
    virtual void on_message(T msg);
};

template <class T>
class Reader : public AnyReader {
public:
    Reader(Subscriber sub, Handler<T> handler);
private:
    Handler<T> m_handler;
};

}
}
