#define BOOST_TEST_DYN_LINK


#include <boost/test/unit_test.hpp>
#include "GatewayManager.h"
#include "CmGateway.h"
#include "ProwlGateway.h"

#include <set>

using namespace boost::unit_test;
using namespace std;


BOOST_AUTO_TEST_CASE( GetNamesReturnsSmsTradeAndProwl )
{
    GatewayManager manager;
    set<string> names = manager.GetPossibleGatewayNames();
    
    BOOST_CHECK(2 == names.size());
    
    BOOST_CHECK(names.end() != names.find(CmGateway::GetGatewayName()));
    BOOST_CHECK(names.end() != names.find(ProwlGateway::GetGatewayName()));
}

BOOST_AUTO_TEST_CASE( SendMessage )
{
    string distributionList;
    string msg = "Hello World";
    
    GatewayManager manager;
    manager.SendMessage(distributionList, msg);
}
