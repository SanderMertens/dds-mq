#include "mq.hpp"
#include "Hello_DCPS.hpp"

int main(int argc, char *arv[])
{
    dds::mq::Writer<Hello::World> writer;

    writer << Hello::World( 0, "Hello World" );
    
    return 0;
}

