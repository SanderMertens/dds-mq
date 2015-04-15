
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
private:
    domain::DomainParticipant m_participant;
    pub::Publisher m_publisher;
    std::string m_name;
};

template <class T>
class Writer {
public:
    Writer(Publisher pub) : 
        m_writer (dds::core::null),
        m_orderWriter (dds::core::null),
        m_topicName(type_name_to_topic(topic::topic_type_name<T>().value())),
        count(0) { 
        dds::topic::qos::TopicQos qos = pub.get_participant().default_topic_qos()
            << core::policy::History::KeepAll()
            << core::policy::Reliability::Reliable();

        dds::topic::Topic< T > topic (pub.get_participant(), m_topicName, qos);
        m_writer = dds::pub::DataWriter< T >(pub.get_publisher(), topic, topic.qos());

        dds::topic::Topic< ::mq::order > orderTopic (pub.get_participant(), "dds_mq_order", qos);
        m_orderWriter = dds::pub::DataWriter< ::mq::order >(pub.get_publisher(), orderTopic, orderTopic.qos());
    }

    void write(T sample) {
        count++;
        dds::core::Time time(0, count);
        DDS::Duration_t duration;
        duration.sec = time.sec();
        duration.nanosec = time.nanosec();
        m_writer.write(sample, time);
        m_orderWriter << ::mq::order(m_topicName, duration);
    }

    void operator<< (T sample) {
        this->write(sample);
    }

private:
    std::string m_topicName;
    dds::pub::DataWriter< T > m_writer;
    dds::pub::DataWriter< ::mq::order > m_orderWriter;
    int count;
};

}
}
