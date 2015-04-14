#include "mq.hpp"
#include "Hello_DCPS.h"

int main(int argc, char *arv[])
{
    dds::domain::DomainParticipant dp( 0 );
    dds::mq::Publisher pub( dp, "myqueue" );
    dds::mq::Writer<Hello::World> writer( pub );

    Hello::World sample( 0, "Hello World" );
    writer << sample;
    
    return 0;
}

