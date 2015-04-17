
#include "mq_DCPS.hpp"
#include "mq_util.hpp"

namespace dds {
namespace mq {

class Publisher {
public:
    Publisher (dds::domain::DomainParticipant dp, std::string name);
    dds::domain::DomainParticipant get_participant();
    dds::pub::Publisher get_publisher();
    int next();
    static Publisher* get_default_publisher();
    void write_meta(std::string topicName, DDS::Duration_t duration);
private:
    domain::DomainParticipant m_participant;
    pub::Publisher m_publisher;
    std::string m_name;
    static Publisher *s_default_publisher;
    dds::pub::DataWriter< ::mq::order > m_orderWriter;
};

template <class T>
class Writer {
public:
    Writer(Publisher& pub) : 
        m_writer (dds::core::null),
        m_topicName(type_name_to_topic(topic::topic_type_name<T>().value())),
        m_count(0),
        m_pub(NULL) { 
        this->init( &pub );  
    }

    Writer() :
        m_writer (dds::core::null),
        m_topicName(type_name_to_topic(topic::topic_type_name<T>().value())),
        m_count(0),
        m_pub(NULL) {
        this->init( Publisher::get_default_publisher() );
    }

    void write(T sample) {
        m_count++;
        dds::core::Time time(0, m_count);
        DDS::Duration_t duration;
        duration.sec = time.sec();
        duration.nanosec = time.nanosec();
        m_writer.write(sample, time);
        m_pub->write_meta( m_topicName, duration );
    }

    void operator<< (T sample) {
        this->write( sample );
    }

private:
    void init(Publisher *pub) {
        dds::topic::qos::TopicQos qos = pub->get_participant().default_topic_qos()
            << core::policy::History::KeepAll()
            << core::policy::Reliability::Reliable();

        dds::topic::Topic< T > topic (pub->get_participant(), m_topicName, qos);
        m_writer = dds::pub::DataWriter< T >(pub->get_publisher(), topic, topic.qos());
        m_pub = pub;
    }

    std::string m_topicName;
    dds::pub::DataWriter< T > m_writer;
    int m_count;
    Publisher *m_pub;
};

}
}
