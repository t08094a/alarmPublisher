#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include "SmsTradeGateway.h"

#include "SmsTradeUrl.h"

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
    /*
    string to = "01749441938";
    string msg = "Hello World";
        
    SmsTradeGateway gateway;
    
    gateway.SendMessage(to, msg, true);
    */
}

BOOST_AUTO_TEST_CASE( GetNameReturnsSmstradeUrl )
{
    string expected = "SmsTradeUrl";
    
    SmsTradeUrl gateway;
    string name = gateway.GetName();
    
    BOOST_CHECK(expected == name);
}

BOOST_AUTO_TEST_CASE( GetGatewayNameReturnsSmstradeUrl )
{
    string expected = "SmsTradeUrl";
    
    BOOST_CHECK(expected == SmsTradeUrl::GetGatewayName());
}

// BOOST_AUTO_TEST_CASE( SendMessageUrl )
// {
//     string to = "01749441938";
//     string msg = "Hello World";
//     
//     SmsTradeUrl target("8Clj1mgg95qD394g48AK0xLQ");
//     //SmsTradeUrl target("8Clj1mgg95qD394g48AK0xLk");
//     
//     target.SendMessage(to, msg, true);
// }