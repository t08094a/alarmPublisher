#define BOOST_TEST_MODULE "sms_gateway Unit Tests"
#define BOOST_TEST_DYN_LINK

#include <fstream>
#include <iostream>
#include <boost/test/included/unit_test.hpp>

using namespace std;

struct MyConfig 
{
    MyConfig()
    {
        BOOST_TEST_MESSAGE( "setup fixture" ); 
        
        CreateSettingsFile();
    }
    
    ~MyConfig()
    { 
        BOOST_TEST_MESSAGE( "teardown fixture" ); 
     
        // TODO: delete settings file
    }
    
    void CreateSettingsFile()
    {
        ofstream settings("smsgateway.config");
        
        settings << ";#### GENERATED BY TEST ####" << endl;
        settings << endl;
        settings << "[GatewaySelector]" << endl;
        settings << "Active = SmsTrade" << endl;
        settings << endl;
        settings << "[SmsTrade]" << endl;
        settings << "Key = AAAABBBBCCCC" << endl;
        settings << "Route = direct" << endl;
        settings << "From = FF Alarm" << endl;
        settings << endl;
        settings << "[Prowl]" << endl;
        settings << "ApiKey = 9VUtGMBbe151aaf4E38BNnp" << endl;
        settings << "Url = https://prowl.weks.net/publicapi/add" << endl;
        settings << "Application = FF Alarm" << endl;
        settings << "Event = Alarm" << endl;
        settings << "Priority = 2" << endl;
        settings << endl;
        settings << "[Recipients]" << endl;
        settings << "Nachname1_Vorname1 = 1111" << endl;
        settings << "Nachname2_Vorname2 = 2222" << endl;
        
        settings.close();
    }
};

BOOST_GLOBAL_FIXTURE( MyConfig );