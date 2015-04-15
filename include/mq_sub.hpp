#include "mq_DCPS.hpp"
#include "mq_util.hpp"
#include <vector>

namespace dds {
namespace mq {

class AnyReader {
public:
    AnyReader(std::string topicName);
    virtual void take(int count);
protected:
    std::string m_topicName;
};

class Subscriber {
public:
    Subscriber(domain::DomainParticipant dp, std::string name);
    domain::DomainParticipant get_participant();
    sub::Subscriber get_subscriber();
    void add_reader(AnyReader reader);
    void dispatch();
private:
    domain::DomainParticipant m_participant;
    sub::Subscriber m_subscriber;
    std::string m_name;
};

    /*dds::sub::cond::ReadCondition rc(dr, dds::sub::status::DataState(), DataHandler()) ;
    dds::core::cond::WaitSet ws;
    ws += rc;
    ws.dispatch();*/

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
        m_rc(dds::core::null) {
        topic::qos::TopicQos qos = sub.get_participant().default_topic_qos()
            << core::policy::History::KeepAll()
            << core::policy::Reliability::Reliable();

        topic::Topic< T > topic (sub.get_participant(), m_topicName, qos);
        m_reader = sub::DataReader< T >(sub.get_subscriber(), topic, topic.qos());
        m_rc = dds::sub::cond::ReadCondition(m_reader, dds::sub::status::DataState(), DataHandler());
        m_ws += m_rc; 
    }

    void take(int count) {
        dds::core::Duration timeout(0,500000000);
        std::vector< dds::sub::Sample< T > > samples;
        dds::core::Time t;
        do {
            // Wait until sample is received or 500msec have passed
            try {
                m_ws->dispatch(timeout);
            } catch(dds::core::TimeoutError e) {
                break; // If no sample is received within 500msec, move on to next metasample
            }

            m_reader.read(samples.begin(), 1);

            t = samples[0].info().timestamp();
            if (!t.nanosec()) {
                m_reader.take(samples.begin(), 1);
                break; // Filter out samples with invalid values
            }

            if (t.nanosec() != count) {
                if (t.nanosec() > count) {
                    // More samples than metasamples have been received.
                    // Wait for the next metasample.
                    break;
                } else {
                    // More metasamples than samples have been received.
                    // Read the next sample.
                    m_reader.take(samples.begin(), 1);
                }
            } else {
                H().on_message(samples[0].data());
                m_reader.take(samples.begin(), 1); // Remove sample from reader
            }
        } while(t.nanosec() != count);
    }
private:
    sub::DataReader<T> m_reader;
    dds::core::cond::WaitSet m_ws;
    dds::sub::cond::ReadCondition m_rc;
};

}
}
