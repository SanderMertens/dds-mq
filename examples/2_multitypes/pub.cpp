#include "mq.hpp"
#include "Types_DCPS.hpp"

int main(int argc, char *arv[])
{
    dds::mq::Writer<Types::Numbers> wrNumber;
    dds::mq::Writer<Types::Strings> wrString;

    wrNumber << Types::Numbers(1);
    wrString << Types::Strings("2");
    wrNumber << Types::Numbers(3);
    wrString << Types::Strings("4");
    
    return 0;
}

