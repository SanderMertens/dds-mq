#include "mq.hpp"
#include "Hello_DCPS.hpp"

class HelloWorldHandler {
public: HelloWorldHandler() {}
    void on_message(Hello::World sample) {
        std::cout << "Message received: " << sample.value();
    }
};

int main(int argc, char *arv[])
{
    dds::domain::DomainParticipant dp( 0 );
    dds::mq::Subscriber sub( dp, "myqueue" );
    dds::mq::Reader<Hello::World, HelloWorldHandler> reader( sub );

    while(1) {
        sub.dispatch();
    }
    
    return 0;
}

