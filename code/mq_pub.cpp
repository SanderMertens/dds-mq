#include "mq_pub.hpp"

namespace dds {
namespace mq {

std::string type_name_to_topic(std::string type_name) {
    return type_name;
}

class Publisher::impl {
public:
    impl (dds::domain::DomainParticipant dp, std::string name) :
        m_participant (dp),
        m_publisher (dds::core::null),
        m_name (name) 
    {
        m_publisher = dds::pub::Publisher(dp, dp.default_publisher_qos() << dds::core::policy::Partition(name));
    }

    dds::domain::DomainParticipant get_participant() {
        return m_participant;
    }

    dds::pub::Publisher get_publisher() {
        return m_publisher;
    }

private:
    domain::DomainParticipant m_participant;
    pub::Publisher m_publisher;
    std::string m_name;
};

Publisher::Publisher(dds::domain::DomainParticipant dp, std::string name) {
    this->ptr = new Publisher::impl(dp, name);
}

Publisher::~Publisher() {
    delete this->ptr;
}

dds::domain::DomainParticipant Publisher::get_participant() {
    return this->ptr->get_participant();
}

dds::pub::Publisher Publisher::get_publisher() {
    return this->ptr->get_publisher();
}

}
}
