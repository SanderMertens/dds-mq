#include "mq.hpp"
#include "Hello_DCPS.hpp"

struct HelloWorldHandler {
    HelloWorldHandler(Hello::World sample) {
        std::cout << "Message received: " << sample.value() << std::endl;
    }
};

int main(int argc, char *arv[]) {
    dds::mq::Reader< Hello::World, HelloWorldHandler > reader;

    while(1) reader.dispatch();
    
    return 0;
}

