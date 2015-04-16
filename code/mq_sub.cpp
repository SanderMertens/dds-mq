#include "mq_sub.hpp"

namespace dds {
namespace mq {

AnyReader::AnyReader(std::string topicName) : m_topicName(topicName) { }

Subscriber *Subscriber::s_default_subscriber = NULL;

class DataHandler {
public:
    DataHandler(Subscriber& subscriber) : m_subscriber(subscriber) { } ;
    void operator() (dds::sub::DataReader< ::mq::order >& dr) {
        dds::sub::LoanedSamples< ::mq::order > samples = dr.take();
        for (dds::sub::LoanedSamples< ::mq::order >::const_iterator
            sample = samples.begin(); sample < samples.end(); ++sample) {
            if (sample->info().valid()) {
                m_subscriber.reader(sample->data().topic())->take(sample->data().timestamp().nanosec);
            }
        }
    } 
private:
    Subscriber& m_subscriber;
};

Subscriber::Subscriber (domain::DomainParticipant dp, std::string name) :
    m_participant (dp),
    m_subscriber (core::null),
    m_orderReader(core::null),
    m_rc(core::null),
    m_name (name) 
{
    m_subscriber = sub::Subscriber(dp, dp.default_subscriber_qos() << core::policy::Partition(name));

    topic::qos::TopicQos qos = m_participant.default_topic_qos()
        << core::policy::History::KeepAll()
        << core::policy::Reliability::Reliable();

    dds::topic::Topic< ::mq::order > orderTopic (m_participant, "dds_mq_order", qos);
    m_orderReader = dds::sub::DataReader< ::mq::order >(m_subscriber, orderTopic, orderTopic.qos());

    m_rc = sub::cond::ReadCondition(m_orderReader, dds::sub::status::DataState(), DataHandler(*this)) ;
    m_ws += m_rc;
}

Subscriber* Subscriber::get_default_subscriber() {
    if (!Subscriber::s_default_subscriber) {
        dds::domain::DomainParticipant dp( 0 );
        Subscriber::s_default_subscriber = new Subscriber(dp, "global");
    }
    return Subscriber::s_default_subscriber;
}

domain::DomainParticipant Subscriber::get_participant() {
    return m_participant;
}

sub::Subscriber Subscriber::get_subscriber() {
    return m_subscriber;
}

void Subscriber::add_reader(std::string topic, AnyReader *reader) {
    m_readers[topic] = reader;
}

AnyReader* Subscriber::reader(std::string topic) {
    return m_readers[topic];
}

void Subscriber::dispatch() {
    m_ws.dispatch();
}

}
}




