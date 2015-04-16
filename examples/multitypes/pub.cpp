#include "mq.hpp"
#include "Types_DCPS.hpp"

int main(int argc, char *arv[])
{
    dds::domain::DomainParticipant dp( 0 );
    dds::mq::Publisher pub( dp, "myqueue" );
    dds::mq::Writer<Types::Numbers> wrNumber( pub );
    dds::mq::Writer<Types::Strings> wrString( pub );

    wrNumber << Types::Numbers(1);
    wrString << Types::Strings("2");
    wrNumber << Types::Numbers(3);
    wrString << Types::Strings("4");
    
    return 0;
}

