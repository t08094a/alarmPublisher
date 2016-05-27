#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include "SmsTradeGateway.h"

using namespace boost::unit_test;
using namespace std;


BOOST_AUTO_TEST_CASE( GetNameReturnsSmstrade )
{
    string expected = "SmsTrade";
    
    SmsTradeGateway gateway;
    string name = gateway.GetName();
    
    BOOST_CHECK(expected == name);
}

BOOST_AUTO_TEST_CASE( GetGatewayNameReturnsSmstrade )
{
    string expected = "SmsTrade";
    
    BOOST_CHECK(expected == SmsTradeGateway::GetGatewayName());
}

BOOST_AUTO_TEST_CASE( SendMessage )
{
    // TODO: Debugmodus aktuell nicht möglich -> es wird immer eine SMS gesandt
    
    string msg = "Hello World";
        
    SmsTradeGateway gateway;
    
    gateway.SendMessage(msg, true);
    
}
