#define BOOST_TEST_DYN_LINK

#include <memory>
#include <boost/test/unit_test.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "../include/Operation.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE( GetInitialIdReturns0 )
{
    int expected = 0;
    
    Operation operation;
    
    BOOST_CHECK(expected == operation.GetId());
}

BOOST_AUTO_TEST_CASE( SetAndGetIdReturns123 )
{
    int expected = 123;
    
    Operation operation;
    operation.SetId(expected);
    
    BOOST_CHECK(expected == operation.GetId());
}


BOOST_AUTO_TEST_CASE( VerifyInitialGuidReturnsNotEmptyGuid )
{
    boost::uuids::uuid expected();
    
    Operation operation;
    
    BOOST_CHECK(false == operation.GetGuid().is_nil());
}

BOOST_AUTO_TEST_CASE( SetGuidUpdatesGuid )
{
    boost::uuids::uuid expected = boost::uuids::random_generator()();
    
    Operation operation;
    operation.SetGuid(expected);
    
    BOOST_CHECK(expected == operation.GetGuid());
}

BOOST_AUTO_TEST_CASE( SetTimestampUpdatesTimestamp )
{
    boost::posix_time::ptime t1(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3));
    
    Operation operation;
    operation.SetTimestamp(t1);
    
    BOOST_CHECK(t1 == operation.GetTimestamp());
    BOOST_CHECK(operation.GetTimestampIncome() != operation.GetTimestamp());
}

BOOST_AUTO_TEST_CASE( SetTimestampIncomeUpdatesTimestampIncome )
{
    boost::posix_time::ptime t1(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3));
    
    Operation operation;
    operation.SetTimestampIncome(t1);
    
    BOOST_CHECK(t1 == operation.GetTimestampIncome());
    BOOST_CHECK(operation.GetTimestamp() != operation.GetTimestampIncome());
}

BOOST_AUTO_TEST_CASE( VerifyAbsenderIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetAbsender());
}

BOOST_AUTO_TEST_CASE( SetAbsenderUpdatesIt )
{
    std::string expected = "ILS";
    
    Operation operation;
    operation.SetAbsender(expected);
    
    BOOST_CHECK(expected == operation.GetAbsender());
}

BOOST_AUTO_TEST_CASE( VerifyTerminIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetTermin());
}

BOOST_AUTO_TEST_CASE( SetTerminUpdatesIt )
{
    std::string expected = "morgen";
    
    Operation operation;
    operation.SetTermin(expected);
    
    BOOST_CHECK(expected == operation.GetTermin());
}

BOOST_AUTO_TEST_CASE( VerifyEinsatzortZusatzIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetEinsatzortZusatz());
}

BOOST_AUTO_TEST_CASE( SetEinsatzortZusatzUpdatesIt )
{
    std::string expected = "Hinterhof";
    
    Operation operation;
    operation.SetEinsatzortZusatz(expected);
    
    BOOST_CHECK(expected == operation.GetEinsatzortZusatz());
}

BOOST_AUTO_TEST_CASE( VerifyEinsatzortPlannummerIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetEinsatzortPlannummer());
}

BOOST_AUTO_TEST_CASE( SetEinsatzortPlannummerUpdatesIt )
{
    std::string expected = "XXYY";
    
    Operation operation;
    operation.SetEinsatzortPlannummer(expected);
    
    BOOST_CHECK(expected == operation.GetEinsatzortPlannummer());
}

BOOST_AUTO_TEST_CASE( VerifyEinsatzortStationIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetEinsatzortStation());
}

BOOST_AUTO_TEST_CASE( SetEinsatzortStationUpdatesIt )
{
    std::string expected = "TTT";
    
    Operation operation;
    operation.SetEinsatzortStation(expected);
    
    BOOST_CHECK(expected == operation.GetEinsatzortStation());
}

BOOST_AUTO_TEST_CASE( VerifyZielortZusatzIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetZielortZusatz());
}

BOOST_AUTO_TEST_CASE( SetZielortZusatzUpdatesIt )
{
    std::string expected = "AAA";
    
    Operation operation;
    operation.SetZielortZusatz(expected);
    
    BOOST_CHECK(expected == operation.GetZielortZusatz());
}

BOOST_AUTO_TEST_CASE( VerifyZielortStationIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetZielortStation());
}

BOOST_AUTO_TEST_CASE( SetZielortStationUpdatesIt )
{
    std::string expected = "BBB";
    
    Operation operation;
    operation.SetZielortStation(expected);
    
    BOOST_CHECK(expected == operation.GetZielortStation());
}

BOOST_AUTO_TEST_CASE( VerifyOperationNumberIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetOperationNumber());
}

BOOST_AUTO_TEST_CASE( SetOperationNumberUpdatesIt )
{
    std::string expected = "Op1";
    
    Operation operation;
    operation.SetOperationNumber(expected);
    
    BOOST_CHECK(expected == operation.GetOperationNumber());
}

BOOST_AUTO_TEST_CASE( VerifyMessengerIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetMessenger());
}

BOOST_AUTO_TEST_CASE( SetMessengerUpdatesIt )
{
    std::string expected = "Hans Müller";
    
    Operation operation;
    operation.SetMessenger(expected);
    
    BOOST_CHECK(expected == operation.GetMessenger());
}

BOOST_AUTO_TEST_CASE( VerifyPriorityIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK("" == operation.GetPriority());
}

BOOST_AUTO_TEST_CASE( SetPriorityUpdatesIt )
{
    std::string expected = "2";
    
    Operation operation;
    operation.SetPriority(expected);
    
    BOOST_CHECK(expected == operation.GetPriority());
}

BOOST_AUTO_TEST_CASE( VerifyResourcesIsInitialEmpty )
{
    Operation operation;
        
    BOOST_CHECK(0 == operation.GetResources().size());
}

BOOST_AUTO_TEST_CASE( AddResourceReturnsCorrectPointer )
{
    OperationResource* resource1 = new OperationResource();
    
    Operation operation;
    operation.AddResource(resource1);
    
    BOOST_CHECK(1 == operation.GetResources().size());
    BOOST_CHECK(resource1 == operation.GetResources()[0].get());
    
    // delete resource1; // NOTE: not needed, because the Operation deletes it on dispose
}

BOOST_AUTO_TEST_CASE( ToStringReturnsAllContent )
{
    OperationResource* resource1 = new OperationResource();
    resource1->SetFullName("Res1");
    resource1->SetTimestamp(boost::posix_time::second_clock::local_time());
    resource1->AddRequestedEquipment("THL");
    resource1->AddRequestedEquipment("Rauchverschluss");
    
    Operation operation;
    operation.SetId(1);
    operation.SetAbsender("Klaus");
    operation.SetTermin("morgen");
    operation.SetEinsatzortZusatz("Hinterhof");
    operation.SetEinsatzortPlannummer("443");
    operation.SetEinsatzortStation("Bahnhof");
    operation.SetOperationNumber("3");
    operation.SetMessenger("Heinz");
    operation.SetPriority("9");
    operation.SetComment("Eilt");
    operation.SetZielortStation("Männa");
    operation.SetZielortZusatz("Sackgasse");
    operation.AddResource(resource1);
    
    PropertyLocation& einsatzort = operation.GetEinsatzortInternal();
    einsatzort.SetStreet("Hauptstraße");
    einsatzort.SetStreetNumber("1a");
    einsatzort.SetZipCode("91472");
    einsatzort.SetCity("Ipsheim");
    
    PropertyLocation& zielort = operation.GetZielortInternal();
    zielort.SetStreet("Hauptstraße");
    zielort.SetStreetNumber("1a");
    zielort.SetZipCode("91472");
    zielort.SetCity("Ipsheim");
    
    OperationKeywords& keywords = operation.GetKeywordsInternal();
    keywords.SetB("B");
    keywords.SetR("R");
    keywords.SetS("S");
    keywords.SetT("T");
    keywords.SetKeyword("THL");
    keywords.SetEmergencyKeyword("Crash");
        
    cout << operation << endl;
    
    // delete resource1; // NOTE: pointer is deleted on destruction of operation
}

BOOST_AUTO_TEST_CASE( VerifyCopyConstructorCreatesAnEqualObject )
{
    OperationResource* resource1 = new OperationResource();
    resource1->SetFullName("Res1");
    resource1->SetTimestamp(boost::posix_time::second_clock::local_time());
    resource1->AddRequestedEquipment("THL");
    resource1->AddRequestedEquipment("Rauchverschluss");
    
    Operation operation;
    operation.SetId(1);
    operation.SetAbsender("Klaus");
    operation.SetTermin("morgen");
    operation.SetEinsatzortZusatz("Hinterhof");
    operation.SetEinsatzortPlannummer("443");
    operation.SetEinsatzortStation("Bahnhof");
    operation.SetOperationNumber("3");
    operation.SetMessenger("Heinz");
    operation.SetPriority("9");
    operation.SetComment("Eilt");
    operation.SetZielortStation("Männa");
    operation.SetZielortZusatz("Sackgasse");
    operation.AddResource(resource1);
    
    PropertyLocation& einsatzort = operation.GetEinsatzortInternal();
    einsatzort.SetStreet("Hauptstraße");
    einsatzort.SetStreetNumber("1a");
    einsatzort.SetZipCode("91472");
    einsatzort.SetCity("Ipsheim");
    
    PropertyLocation& zielort = operation.GetZielortInternal();
    zielort.SetStreet("Hauptstraße");
    zielort.SetStreetNumber("1a");
    zielort.SetZipCode("91472");
    zielort.SetCity("Ipsheim");
    
    OperationKeywords& keywords = operation.GetKeywordsInternal();
    keywords.SetB("B");
    keywords.SetR("R");
    keywords.SetS("S");
    keywords.SetT("T");
    keywords.SetKeyword("THL");
    keywords.SetEmergencyKeyword("Crash");
    
    Operation copy = operation;
    
    BOOST_CHECK(operation.GetAbsender() == copy.GetAbsender());
    BOOST_CHECK(operation.GetComment() == copy.GetComment());
    BOOST_CHECK(operation.GetEinsatzort() == copy.GetEinsatzort());
    BOOST_CHECK(operation.GetEinsatzortPlannummer() == copy.GetEinsatzortPlannummer());
    BOOST_CHECK(operation.GetEinsatzortStation() == copy.GetEinsatzortStation());
    BOOST_CHECK(operation.GetEinsatzortZusatz() == copy.GetEinsatzortZusatz());
    BOOST_CHECK(operation.GetGuid() == copy.GetGuid());
    BOOST_CHECK(operation.GetId() == copy.GetId());
    BOOST_CHECK(operation.GetKeywords() == copy.GetKeywords());
    BOOST_CHECK(operation.GetMessenger() == copy.GetMessenger());
    BOOST_CHECK(operation.GetOperationNumber() == copy.GetOperationNumber());
    BOOST_CHECK(operation.GetPriority() == copy.GetPriority());
    BOOST_CHECK(operation.GetTermin() == copy.GetTermin());
    BOOST_CHECK(operation.GetTimestamp() == copy.GetTimestamp());
    BOOST_CHECK(operation.GetTimestampIncome() == copy.GetTimestampIncome());
    BOOST_CHECK(operation.GetZielort() == copy.GetZielort());
    BOOST_CHECK(operation.GetZielortStation() == copy.GetZielortStation());
    BOOST_CHECK(operation.GetZielortZusatz() == copy.GetZielortZusatz());
    
    BOOST_CHECK(operation.GetResources().size() == copy.GetResources().size());
    BOOST_CHECK(*operation.GetResources().at(0) == *copy.GetResources().at(0));
}

BOOST_AUTO_TEST_CASE( VerifyAssignmentOperatorCreatesAnEqualObject )
{
    OperationResource* resource1 = new OperationResource();
    resource1->SetFullName("Res1");
    resource1->SetTimestamp(boost::posix_time::second_clock::local_time());
    resource1->AddRequestedEquipment("THL");
    resource1->AddRequestedEquipment("Rauchverschluss");
    
    Operation operation;
    operation.SetId(1);
    operation.SetAbsender("Klaus");
    operation.SetTermin("morgen");
    operation.SetEinsatzortZusatz("Hinterhof");
    operation.SetEinsatzortPlannummer("443");
    operation.SetEinsatzortStation("Bahnhof");
    operation.SetOperationNumber("3");
    operation.SetMessenger("Heinz");
    operation.SetPriority("9");
    operation.SetComment("Eilt");
    operation.SetZielortStation("Männa");
    operation.SetZielortZusatz("Sackgasse");
    operation.AddResource(resource1);
    
    PropertyLocation& einsatzort = operation.GetEinsatzortInternal();
    einsatzort.SetStreet("Hauptstraße");
    einsatzort.SetStreetNumber("1a");
    einsatzort.SetZipCode("91472");
    einsatzort.SetCity("Ipsheim");
    
    PropertyLocation& zielort = operation.GetZielortInternal();
    zielort.SetStreet("Hauptstraße");
    zielort.SetStreetNumber("1a");
    zielort.SetZipCode("91472");
    zielort.SetCity("Ipsheim");
    
    OperationKeywords& keywords = operation.GetKeywordsInternal();
    keywords.SetB("B");
    keywords.SetR("R");
    keywords.SetS("S");
    keywords.SetT("T");
    keywords.SetKeyword("THL");
    keywords.SetEmergencyKeyword("Crash");
    
    Operation copy;
    copy = operation;
    
    BOOST_CHECK(operation == copy);
    
    BOOST_CHECK(operation.GetAbsender() == copy.GetAbsender());
    BOOST_CHECK(operation.GetComment() == copy.GetComment());
    BOOST_CHECK(operation.GetEinsatzort() == copy.GetEinsatzort());
    BOOST_CHECK(operation.GetEinsatzortPlannummer() == copy.GetEinsatzortPlannummer());
    BOOST_CHECK(operation.GetEinsatzortStation() == copy.GetEinsatzortStation());
    BOOST_CHECK(operation.GetEinsatzortZusatz() == copy.GetEinsatzortZusatz());
    BOOST_CHECK(operation.GetGuid() == copy.GetGuid());
    BOOST_CHECK(operation.GetId() == copy.GetId());
    BOOST_CHECK(operation.GetKeywords() == copy.GetKeywords());
    BOOST_CHECK(operation.GetMessenger() == copy.GetMessenger());
    BOOST_CHECK(operation.GetOperationNumber() == copy.GetOperationNumber());
    BOOST_CHECK(operation.GetPriority() == copy.GetPriority());
    BOOST_CHECK(operation.GetTermin() == copy.GetTermin());
    BOOST_CHECK(operation.GetTimestamp() == copy.GetTimestamp());
    BOOST_CHECK(operation.GetTimestampIncome() == copy.GetTimestampIncome());
    BOOST_CHECK(operation.GetZielort() == copy.GetZielort());
    BOOST_CHECK(operation.GetZielortStation() == copy.GetZielortStation());
    BOOST_CHECK(operation.GetZielortZusatz() == copy.GetZielortZusatz());
    
    BOOST_CHECK(operation.GetResources().size() == copy.GetResources().size());
    BOOST_CHECK(*operation.GetResources().at(0) == *copy.GetResources().at(0));
}

BOOST_AUTO_TEST_CASE( ComparedObjectWithDifferendMessengerObjectsResultInFalse )
{
    OperationResource* resource1 = new OperationResource();
    resource1->SetFullName("Res1");
    resource1->SetTimestamp(boost::posix_time::second_clock::local_time());
    resource1->AddRequestedEquipment("THL");
    resource1->AddRequestedEquipment("Rauchverschluss");
    
    Operation operation;
    operation.SetId(1);
    operation.SetAbsender("Klaus");
    operation.SetTermin("morgen");
    operation.SetEinsatzortZusatz("Hinterhof");
    operation.SetEinsatzortPlannummer("443");
    operation.SetEinsatzortStation("Bahnhof");
    operation.SetOperationNumber("3");
    operation.SetMessenger("Heinz");
    operation.SetPriority("9");
    operation.SetComment("Eilt");
    operation.SetZielortStation("Männa");
    operation.SetZielortZusatz("Sackgasse");
    operation.AddResource(resource1);
        
    Operation copy;
    copy = operation;
    
    // modify copy
    copy.SetMessenger("Wilfried");
    
    BOOST_CHECK(operation != copy);
}

BOOST_AUTO_TEST_CASE( ComparedObjectWithDifferendResourcesObjectsResultInFalse )
{
    OperationResource* resource1 = new OperationResource();
    resource1->SetFullName("Res1");
    resource1->SetTimestamp(boost::posix_time::second_clock::local_time());
    resource1->AddRequestedEquipment("THL");
    resource1->AddRequestedEquipment("Rauchverschluss");
    
    Operation operation;
    operation.SetId(1);
    operation.SetAbsender("Klaus");
    operation.SetTermin("morgen");
    operation.SetEinsatzortZusatz("Hinterhof");
    operation.SetEinsatzortPlannummer("443");
    operation.SetEinsatzortStation("Bahnhof");
    operation.SetOperationNumber("3");
    operation.SetMessenger("Heinz");
    operation.SetPriority("9");
    operation.SetComment("Eilt");
    operation.SetZielortStation("Männa");
    operation.SetZielortZusatz("Sackgasse");
    operation.AddResource(resource1);
    
    PropertyLocation& einsatzort = operation.GetEinsatzortInternal();
    einsatzort.SetStreet("Hauptstraße");
    einsatzort.SetStreetNumber("1a");
    einsatzort.SetZipCode("91472");
    einsatzort.SetCity("Ipsheim");
    
    PropertyLocation& zielort = operation.GetZielortInternal();
    zielort.SetStreet("Hauptstraße");
    zielort.SetStreetNumber("1a");
    zielort.SetZipCode("91472");
    zielort.SetCity("Ipsheim");
    
    OperationKeywords& keywords = operation.GetKeywordsInternal();
    keywords.SetB("B");
    keywords.SetR("R");
    keywords.SetS("S");
    keywords.SetT("T");
    keywords.SetKeyword("THL");
    keywords.SetEmergencyKeyword("Crash");
    
    Operation copy;
    copy = operation;
    
    // modify copy
    (*copy.GetResources().front().get()).SetFullName("Res2");
    
    BOOST_CHECK(operation != copy);
}

BOOST_AUTO_TEST_CASE( VerifyAssignmentOperatorWithSamePointerShouldHandleSelfAssignment )
{
    Operation operation1;
    operation1.SetId(1);
    
    operation1 = operation1;
    
    BOOST_CHECK(addressof<Operation>(operation1) == addressof<Operation>(operation1));
}

static string GetDataAsJson(IOperation* operation);

BOOST_AUTO_TEST_CASE( SendIsCalledCorrectly )
{
    Operation op;
    op.SetComment("comment1");
    op.SetPriority("1");
    op.SetTimestamp(boost::posix_time::ptime(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1,2,3)));
    op.SetEinsatzortZusatz("Garage");
    
    PropertyLocation& einsatzort = op.GetEinsatzortInternal();
    einsatzort.SetStreet("Meiserstraße");
    einsatzort.SetStreetNumber("2");
    einsatzort.SetZipCode("90144");
    einsatzort.SetCity("Nürnberg");
    einsatzort.SetGeoLatitude(1.2);
    einsatzort.SetGeoLongitude(2.3);
    
    OperationKeywords& keywords = op.GetKeywordsInternal();
    keywords.SetKeyword("Keyword1");
    keywords.SetEmergencyKeyword("THL1");
    keywords.SetB("b");
    keywords.SetR("r");
    keywords.SetS("s");
    keywords.SetT("t");
    
    // note: do not delete pointer! It is deleted internally
    OperationResource* res1 = new OperationResource();
    res1->SetFullName("LF8");
    res1->AddRequestedEquipment("Wasser 600l");
    res1->AddRequestedEquipment("THL");
    
    // note: do not delete pointer! It is deleted internally
    OperationResource* res2 = new OperationResource();
    res2->SetFullName("MTW");
    res2->AddRequestedEquipment("Überdruckbelüftung");
    
    op.AddResource(res1);
    op.AddResource(res2);
    
    string result = GetDataAsJson(&op);
    cerr << result << endl;
}

static string FormatDateTimeForJson(boost::posix_time::ptime timestamp)
{
    using namespace boost::posix_time;
    
    static std::locale loc(std::cout.getloc(), new time_facet("%Y-%m-%dT%H:%M:%SZ"));
    
    ostringstream is;
    is.imbue(loc);
    is << timestamp;
    
    string result = is.str();
    return result;
}

static void GetResourceAsJson(const vector<unique_ptr<OperationResource>>& resources, stringstream& ss)
{
    ss << "  \"resources\": [" << endl;

    int resourcesCount = resources.size();
    int resourcesCurrentIndex = 0;
    for(auto const& resource : resources)
    {
        ss << "    {" << endl;
        ss << "      \"name\": \"" << resource.get()->GetFullName() << "\"," << endl;
        ss << "      \"equipments\": ";

        int equipmentCount = resource.get()->GetRequestedEquipment().size();
        if(equipmentCount == 0)
        {
            ss << "[]" << endl;
        }
        else
        {
            ss << "[" << endl;

            int equipmentCurrentIndex = 0;
            for(auto const& equipment : resource.get()->GetRequestedEquipment())
            {
                ss << "        {" << endl;
                ss << "          \"name\": \"" << equipment << "\"" << endl;
                ss << "        }";

                if(equipmentCurrentIndex < equipmentCount - 1)
                {
                    ss << ",";
                }

                ss << endl;

                equipmentCurrentIndex++;
            }

            ss << "      ]" << endl;
        }

        ss << "    }";

        if(resourcesCurrentIndex < resourcesCount - 1)
        {
            ss << ",";
        }

        ss << endl;
        resourcesCurrentIndex++;
    }

    ss << "  ]" << endl;
}

static string GetDataAsJson(IOperation* operation)
{
    ILocation& location = operation->GetEinsatzort();
    IKeywords& keywords =  operation->GetKeywords();

    stringstream ss;
    ss << "{" << endl;
    ss << "  \"time\": \"" << FormatDateTimeForJson(operation->GetTimestamp()) << "\"," << endl;
    ss << "  \"comment\": \"" << operation->GetComment() << "\"," << endl;
    ss << "  \"keywords\": {" << endl;
    ss << "    \"keyword\": \"" << keywords.GetKeyword() << "\"," << endl;
    ss << "    \"emergencyKeyword\": \"" << keywords.GetEmergencyKeyword() << "\"," << endl;
    ss << "    \"b\": \"" << keywords.GetB() << "\"," << endl;
    ss << "    \"r\": \"" << keywords.GetR() << "\"," << endl;
    ss << "    \"s\": \"" << keywords.GetS() << "\"," << endl;
    ss << "    \"t\": \"" << keywords.GetT() << "\"" << endl;
    ss << "  }," << endl;
    ss << "  \"placeOfAction\": {" << endl;
    ss << "    \"street\": \"" << location.GetStreet() << "\"," << endl;
    ss << "    \"houseNumber\": \"" << location.GetStreetNumber() << "\"," << endl;
    ss << "    \"addition\": \"" << operation->GetEinsatzortZusatz() << "\"," << endl;
    ss << "    \"city\": \"" << location.GetCity() << "\"," << endl;
    
    if(location.HasGeoCoordinates())
    {
        ss << "    \"geoPosition\": {" << endl;
        ss << "      \"x\": \"" << location.GetGeoLatitude() << "\"," << endl;
        ss << "      \"y\": \"" << location.GetGeoLongitude() << "\"" << endl;
        ss << "    }" << endl;
    }

    ss << "  }," << endl;
    ss << "  \"priority\": " << operation->GetPriority() << "," << endl;

    GetResourceAsJson(operation->GetResources(), ss);

    ss << "}";

    string s = ss.str();

    return s;
}

