idlpp -lisocpp -S Types.idl -d gen
g++ pub.cpp gen/*.cpp -I gen -I ../../gen -g -m32 -I ../../include -I$OSPL_HOME/include -I$OSPL_HOME/include/sys -I$OSPL_HOME/include/dcps/C++/isocpp -I$OSPL_HOME/include/dcps/C++/SACPP -L$OSPL_HOME/lib -ldcpsisocpp -lddskernel -L../.. -lddsmq -o pub
g++ sub.cpp gen/*.cpp -I gen -I ../../gen -g -m32 -I ../../include -I$OSPL_HOME/include -I$OSPL_HOME/include/sys -I$OSPL_HOME/include/dcps/C++/isocpp -I$OSPL_HOME/include/dcps/C++/SACPP -L$OSPL_HOME/lib -ldcpsisocpp -lddskernel -L../.. -lddsmq -o sub
