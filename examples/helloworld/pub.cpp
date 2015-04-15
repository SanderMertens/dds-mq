#include "mq.hpp"
#include "Hello_DCPS.hpp"

int main(int argc, char *arv[])
{
    dds::domain::DomainParticipant dp( 0 );
    dds::mq::Publisher pub( dp, "myqueue" );
    dds::mq::Writer<Hello::World> writer( pub );

    writer << Hello::World( 0, "Hello World" );
    
    return 0;
}

