
#include "mq_DCPS.hpp"
#include "mq_util.hpp"

namespace dds {
namespace mq {

std::string type_name_to_topic(std::string type_name);

class Publisher {
public:
    Publisher(domain::DomainParticipant dp, std::string name);
    dds::domain::DomainParticipant get_participant();
    dds::pub::Publisher get_publisher();
    ~Publisher();
private:
    class impl;
    impl *ptr;
};

template <class T>
class Writer {
public:
    Writer(Publisher pub) : 
        m_writer (dds::core::null),
        m_topicName(type_name_to_topic(topic::topic_type_name<T>().value())) { 
        dds::topic::qos::TopicQos qos = pub.get_participant().default_topic_qos();

        // Create queue writer
        dds::topic::Topic<T> topic (pub.get_participant(), m_topicName, qos);
        m_writer = dds::pub::DataWriter<T>(pub.get_publisher(), topic, topic.qos());

        // Create metatopic writer
        dds::topic::Topic< ::mq::order> orderTopic (pub.get_participant(), "dds.mq.order", qos);
        m_orderWriter = dds::pub::DataWriter< ::mq::order>(pub.get_publisher(), orderTopic, orderTopic.qos());
    }

    void write(T sample) {
        dds::core::Duration t = now();
        m_writer.write_w_timestamp(sample, t);
        m_orderWriter << ::mq::order(m_topicName, t);
    }

    void operator<< (T sample) {
        this.write(sample);
    }

private:
    std::string m_topicName;
    dds::pub::DataWriter<T> m_writer;
    dds::pub::DataWriter< ::mq::order> m_orderWriter;
};

}
}
