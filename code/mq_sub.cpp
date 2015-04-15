#include "mq_sub.hpp"

namespace dds {
namespace mq {

AnyReader::AnyReader(std::string topicName) : m_topicName(topicName) { }
void AnyReader::take() { }

Subscriber::Subscriber (domain::DomainParticipant dp, std::string name) :
    m_participant (dp),
    m_subscriber (core::null),
    m_name (name) 
{
    m_subscriber = sub::Subscriber(dp, dp.default_subscriber_qos() << core::policy::Partition(name));
}

domain::DomainParticipant Subscriber::get_participant() {
    return m_participant;
}

sub::Subscriber Subscriber::get_subscriber() {
    return m_subscriber;
}

void Subscriber::add_reader(AnyReader reader) {

}

/*
class DataHandler {
public:
    DataHandler() { } ;
    void operator() (dds::sub::DataReader<Hello::World>& dr) {
        dds::sub::LoanedSamples<Hello::World> samples = dr.take();
        for (dds::sub::LoanedSamples<Hello::World>::const_iterator
            sample = samples.begin(); sample < samples.end(); ++sample)
        {
            std::cout << "Sample received: id=" 
                      << sample->data().id() 
                      << ", value='" 
                      << sample->data().value() << "'" << std::endl;
        }
    } 
};
*/

void Subscriber::dispatch() {
    topic::qos::TopicQos qos = m_participant.default_topic_qos()
        << core::policy::History::KeepAll()
        << core::policy::Reliability::Reliable();

    dds::topic::Topic< ::mq::order > orderTopic (m_participant, "dds_mq_order", qos);
    dds::sub::DataReader< ::mq::order > dr = dds::sub::DataReader< ::mq::order >(
        m_subscriber, orderTopic, orderTopic.qos());

    /*dds::sub::cond::ReadCondition rc(dr, dds::sub::status::DataState(), DataHandler()) ;
    dds::core::cond::WaitSet ws;
    ws += rc;
    ws.dispatch();*/
}

}
}




