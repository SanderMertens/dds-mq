#include "mq.hpp"
#include "Types_DCPS.hpp"

struct NumberHandler {
    NumberHandler(Types::Numbers sample) {
        std::cout << "Number received: " << sample.value() << std::endl;
    }
};

struct StringHandler {
    StringHandler(Types::Strings sample) {
        std::cout << "String received: " << sample.value() << std::endl;
    }
};

int main(int argc, char *arv[]) {
    dds::domain::DomainParticipant dp( 0 );
    dds::mq::Subscriber sub( dp, "myqueue" );
    dds::mq::Reader< Types::Numbers, NumberHandler > drNumber( sub );
    dds::mq::Reader< Types::Strings, StringHandler > drString( sub );

    while(1) {
        sub.dispatch(); // Could've used any reader to dispatch
    }
    
    return 0;
}
