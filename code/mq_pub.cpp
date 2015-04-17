#include "mq_pub.hpp"
#include "unistd.h"

namespace dds {
namespace mq {

Publisher *Publisher::s_default_publisher = NULL;

Publisher::Publisher (dds::domain::DomainParticipant dp, std::string name) :
    m_participant (dp),
    m_publisher (dds::core::null),
    m_orderWriter (dds::core::null),
    m_name (name) {
    m_publisher = dds::pub::Publisher(dp, dp.default_publisher_qos() << dds::core::policy::Partition(name));
    dds::topic::qos::TopicQos qos = m_participant.default_topic_qos()
        << core::policy::History::KeepAll()
        << core::policy::Reliability::Reliable();

    dds::topic::Topic< ::mq::order > orderTopic ( m_participant, "dds_mq_order", qos );
    m_orderWriter = dds::pub::DataWriter< ::mq::order >( m_publisher, orderTopic, orderTopic.qos() );
}

dds::domain::DomainParticipant Publisher::get_participant() {
    return m_participant;
}

dds::pub::Publisher Publisher::get_publisher() {
    return m_publisher;
}

Publisher* Publisher::get_default_publisher() {
    if (!Publisher::s_default_publisher) {
        dds::domain::DomainParticipant dp( 0 );
        Publisher::s_default_publisher = new Publisher(dp, "global");
    }
    return Publisher::s_default_publisher;
}

void Publisher::write_meta(std::string topicName, DDS::Duration_t duration) {
    this->m_orderWriter << ::mq::order( topicName, duration );
}

}
}
