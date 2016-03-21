#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include "ProwlGateway.h"

using namespace boost::unit_test;
using namespace std;


BOOST_AUTO_TEST_CASE( GetNameReturnsProwl )
{
    string expected = "Prowl";
    
    ProwlGateway gateway;
    string name = gateway.GetName();
    
    BOOST_CHECK(expected == name);
}

BOOST_AUTO_TEST_CASE( GetGatewayNameReturnsProwl )
{
    string expected = "Prowl";
    
    BOOST_CHECK(expected == ProwlGateway::GetGatewayName());
}

BOOST_AUTO_TEST_CASE( SendMessage )
{
    // TODO: Debugmodus aktuell nicht möglich -> es wird immer eine SMS gesandt
    
    string to; // is ignored
    string msg = "Hello World";
        
    ProwlGateway gateway;
    
    gateway.SendMessage(to, msg, true);
}