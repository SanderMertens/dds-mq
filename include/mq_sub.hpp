#include "mq_DCPS.hpp"
#include "mq_util.hpp"
#include <vector>
#include <map>

namespace dds {
namespace mq {

class AnyReader {
public:
    AnyReader(std::string topicName);
    virtual void take(int count) = 0;
protected:
    std::string m_topicName;
};

class Subscriber {
public:
    Subscriber(domain::DomainParticipant dp, std::string name);
    domain::DomainParticipant get_participant();
    sub::Subscriber get_subscriber();
    void add_reader(std::string topic, AnyReader *reader);
    void dispatch();
    void operator() (dds::sub::DataReader< ::mq::order >& dr);
    AnyReader* reader(std::string topic);
    static Subscriber* get_default_subscriber();
private:
    domain::DomainParticipant m_participant;
    sub::Subscriber m_subscriber;
    std::string m_name;
    std::map<std::string, AnyReader*> m_readers;
    sub::DataReader< ::mq::order > m_orderReader;
    sub::cond::ReadCondition m_rc;
    core::cond::WaitSet m_ws;
    static Subscriber *s_default_subscriber;
};

template <class T, class H>
class Reader : public AnyReader {
public:
    class DataHandler {
    public:
        DataHandler() { } ;
        void operator() (dds::sub::DataReader<T>& dr) { } 
    };

    Reader(Subscriber& sub) : 
        AnyReader(type_name_to_topic(topic::topic_type_name<T>().value())),
        m_reader(dds::core::null),
        m_rc(dds::core::null),
        m_sub(NULL) {
        this->init(&sub);
    }

    Reader() : 
        AnyReader(type_name_to_topic(topic::topic_type_name<T>().value())),
        m_reader(dds::core::null),
        m_rc(dds::core::null),
        m_sub(NULL) {
        this->init(Subscriber::get_default_subscriber());
    }

    void dispatch() {
        m_sub->dispatch();
    }

    void take(int count) {
        dds::core::Duration timeout(0,500000000);
        dds::sub::Sample< T > sample;
        dds::core::Time t;

        // Wait until sample is received or 500msec have passed
        try {
            m_ws->dispatch(timeout);
        } catch(dds::core::TimeoutError e) {
            return;
        }

        m_reader.take(&sample, 1);

        H(sample.data());
    }
private:
    void init(Subscriber* sub) {
        topic::qos::TopicQos qos = sub->get_participant().default_topic_qos()
            << core::policy::History::KeepAll()
            << core::policy::Reliability::Reliable();

        topic::Topic< T > topic (sub->get_participant(), m_topicName, qos);
        m_reader = sub::DataReader< T >(sub->get_subscriber(), topic, topic.qos());
        m_rc = dds::sub::cond::ReadCondition(m_reader, dds::sub::status::DataState(), DataHandler());
        m_ws += m_rc;
        sub->add_reader(m_topicName, this);
        m_sub = sub;
    }

    sub::DataReader<T> m_reader;
    dds::core::cond::WaitSet m_ws;
    dds::sub::cond::ReadCondition m_rc;
    Subscriber *m_sub;
};

}
}
