#include "mq.hpp"
#include "Hello_DCPS.h"

class HelloWorldHandler : dds::mq::Handler<Hello::World> {
    HelloWorldHandler() {}
    void on_message(Hello::World sample) {
        std::cout << "Message received: " << sample.value();
    }
};

int main(int argc, char *arv[])
{
    dds::domain::DomainParticipant dp( 0 );
    dds::mq::Subscriber sub( dp, "myqueue" );
    dds::mq::Reader<Hello::World> reader( sub, HelloWorldHandler() );

    while(1) {
        sub->dispatch();
    }
    
    return 0;
}

