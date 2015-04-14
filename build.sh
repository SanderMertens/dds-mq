idlpp -I$OSPL_HOME/etc/idl -lisocpp -S idl/mq.idl -d gen
g++ code/*.cpp gen/*.cpp -I gen -I include -g -m32 -I$OSPL_HOME/include -I$OSPL_HOME/include/sys -I$OSPL_HOME/include/dcps/C++/isocpp -I$OSPL_HOME/include/dcps/C++/SACPP -L$OSPL_HOME/lib -ldcpsisocpp -lddskernel --shared -o queue
