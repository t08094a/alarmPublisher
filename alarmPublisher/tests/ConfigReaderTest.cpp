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
    vector<string> numbers = ConfigReader::GetInstance().GetTelephoneNumbers();
        
    BOOST_CHECK("1111" == numbers.at(0));
    BOOST_CHECK("2222" == numbers.at(1));
}
