#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include "cmgateway.h"

using namespace boost::unit_test;
using namespace std;


BOOST_AUTO_TEST_CASE( GetNameReturnsCm )
{
    string expected = "cm";

    CmGateway gateway;
    string name = gateway.GetName();

    BOOST_CHECK(expected == name);
}


BOOST_AUTO_TEST_CASE( GetGatewayNameReturnsProwl )
{
    string expected = "cm";

    BOOST_CHECK(expected == CmGateway::GetGatewayName());
}

// BOOST_AUTO_TEST_CASE( SendMessage )
// {
//     string msg = "Hello World";
//
//     CmGateway gateway;
//
//     gateway.SendMessage(msg, true);
// }
