#include "mq_pub.hpp"

namespace dds {
namespace mq {

Publisher::Publisher (dds::domain::DomainParticipant dp, std::string name) :
    m_participant (dp),
    m_publisher (dds::core::null),
    m_name (name) 
{
    m_publisher = dds::pub::Publisher(dp, dp.default_publisher_qos() << dds::core::policy::Partition(name));
}

dds::domain::DomainParticipant Publisher::get_participant() {
    return m_participant;
}

dds::pub::Publisher Publisher::get_publisher() {
    return m_publisher;
}

}
}
