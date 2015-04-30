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
    dds::mq::Reader< Types::Numbers, NumberHandler > drNumber;
    dds::mq::Reader< Types::Strings, StringHandler > drString;

    while(1) {
        dds::mq::dispatch();
    }
    
    return 0;
}
