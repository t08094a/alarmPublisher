#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include "ConfigReader.h"

using namespace boost::unit_test;
using namespace std;


BOOST_AUTO_TEST_CASE( GetActiveGateway )
{
    string expected = "SmsTrade";
    
    string activeGateway = ConfigReader::GetInstance().Get("GatewaySelector.Active");
        
    BOOST_CHECK(expected == activeGateway);
}

BOOST_AUTO_TEST_CASE( GetTelephonNumber )
{
    string expected = "1111;2222";
    
    string numbers = ConfigReader::GetInstance().GetTelephoneNumbers();
        
    BOOST_CHECK(expected == numbers);
}