#define BOOST_TEST_DYN_LINK

#include <functional>
#include <iostream>
#include <fstream>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "../include/PropertyLocation.h"
#include "../include/IlsAnsbach.h"
#include "../include/IOperation.h"

using namespace std;
using namespace boost::unit_test;

// forward declarations
static const string GetFaxContent1();
static const string GetFaxContent2();
static const string CreateTemporaryFile(function<string()> getContent);

/*
BOOST_AUTO_TEST_CASE( ParseFileWithEmptyFilename )
{
    IlsAnsbach* parser = new IlsAnsbach();
    IOperation* result = parser->Parse("");
    
    BOOST_CHECK(result == nullptr);
    
    delete parser;
}

BOOST_AUTO_TEST_CASE( ParseFileWithNotExistingFile )
{
    IlsAnsbach* parser = new IlsAnsbach();
    IOperation* result = parser->Parse("/tmp/sowieso");
    
    BOOST_CHECK(result == nullptr);
    
    delete parser;
}

BOOST_AUTO_TEST_CASE( ParseFileWithExistingFile )
{
    const string filename = CreateTemporaryFile(GetFaxContent1);
    
    IlsAnsbach parser;
    IOperation* result = parser.Parse(filename);
    
    ILocation& einsatzort = result->GetEinsatzort();
    ILocation& zielort = result->GetZielort();
    IKeywords& keywords = result->GetKeywords();
    
    if(boost::filesystem::exists(filename))
    {
        boost::filesystem::remove(filename);
    }
    
    cout << *result << endl;
    
    boost::posix_time::ptime expectedTimestamp(boost::gregorian::date(2015, boost::gregorian::Aug, 29), boost::posix_time::time_duration(14, 35, 0));
    boost::posix_time::ptime lowestTimestampIncome(boost::posix_time::second_clock::local_time() - boost::posix_time::seconds(1));
    boost::posix_time::ptime highestTimestampIncome(boost::posix_time::second_clock::local_time() + boost::posix_time::seconds(1));
        
    BOOST_CHECK(result != nullptr);
    BOOST_CHECK(expectedTimestamp == result->GetTimestamp());
    BOOST_CHECK("ILS ANSBACH" == result->GetAbsender());
    BOOST_CHECK("" == result->GetComment());
    
    BOOST_CHECK("Ipsheim" == einsatzort.GetCity());
    BOOST_CHECK("" == einsatzort.GetGeoLatitude());
    BOOST_CHECK("" == einsatzort.GetGeoLongitude());
    BOOST_CHECK("" == einsatzort.GetIntersection());
    BOOST_CHECK("" == einsatzort.GetLocation());
    BOOST_CHECK("5.1.3 NEA Feuerwehrgerätehaus Ipsheim" == einsatzort.GetProperty());
    BOOST_CHECK("Marktplatz" == einsatzort.GetStreet());
    BOOST_CHECK("0" == einsatzort.GetStreetNumber());
    BOOST_CHECK("91472" == einsatzort.GetZipCode());
    BOOST_CHECK(false == einsatzort.HasGeoCoordinates());
    BOOST_CHECK(true == einsatzort.IsMeaningful());
    
    BOOST_CHECK("" == result->GetEinsatzortPlannummer());
    BOOST_CHECK("" == result->GetEinsatzortStation());
    BOOST_CHECK("FW" == result->GetEinsatzortZusatz());
    BOOST_CHECK(0 == result->GetId());
        
    BOOST_CHECK("Technische Hilfe klein !!!" == keywords.GetKeyword());
    BOOST_CHECK("" == keywords.GetEmergencyKeyword());
    BOOST_CHECK("" == keywords.GetB());
    BOOST_CHECK("" == keywords.GetR());
    BOOST_CHECK("" == keywords.GetS());
    BOOST_CHECK("THL 1" == keywords.GetT());
    
    BOOST_CHECK("" == result->GetMessenger());
    BOOST_CHECK("T 5.1 150829 1383" == result->GetOperationNumber());
    BOOST_CHECK("1" == result->GetPriority());
    BOOST_CHECK(0 == result->GetResources().size());
    BOOST_CHECK("." == result->GetTermin());
    BOOST_CHECK(expectedTimestamp == result->GetTimestamp());
    BOOST_CHECK(lowestTimestampIncome <= result->GetTimestampIncome());
    BOOST_CHECK(highestTimestampIncome >= result->GetTimestampIncome());
    
    BOOST_CHECK("" == zielort.GetCity());
    BOOST_CHECK("" == zielort.GetGeoLatitude());
    BOOST_CHECK("" == zielort.GetGeoLongitude());
    BOOST_CHECK("" == zielort.GetIntersection());
    BOOST_CHECK("" == zielort.GetLocation());
    BOOST_CHECK("" == zielort.GetProperty());
    BOOST_CHECK("" == zielort.GetStreet());
    BOOST_CHECK("1" == zielort.GetStreetNumber()); // ??? sollte eigentlich leer sein
    BOOST_CHECK("" == zielort.GetZipCode());
    BOOST_CHECK(false == zielort.HasGeoCoordinates());
    BOOST_CHECK(false == zielort.IsMeaningful());
    
    BOOST_CHECK("" == result->GetZielortStation());
    BOOST_CHECK("" == result->GetZielortZusatz());
    
    delete result;
}
*/
BOOST_AUTO_TEST_CASE( ParseFileWithMultipleEinsatzmittel )
{
    const string filename = CreateTemporaryFile(GetFaxContent2);
    
    IlsAnsbach parser;
    IOperation* result = parser.Parse(filename);
    
    ILocation& einsatzort = result->GetEinsatzort();
    ILocation& zielort = result->GetZielort();
    IKeywords& keywords = result->GetKeywords();
    
    if(boost::filesystem::exists(filename))
    {
        boost::filesystem::remove(filename);
    }
    
    cout << *result << endl;
}

static const string GetFaxContent1()
{
    string a = " \n"
"\n"
"EM n.s ensbac Booms (esii asoso-no ämo(eszuaiwwes ein e es.os.eois äci4=as E\n"
"---------------------- -- Alarmfax der ILS Ansbach ------------------------\n"
"\n"
"Absender : ILS ANSBACH\n"
"Fax :49 (981) 65050-410 Rufnummer:49 (981) 9776240\n"
"Termin .\n"
"\n"
"Einsatznummer: T 5.1 150829 1383\n"
"\n"
"---------------------------- -- EINSATZORT ------------------------------\n"
"Straße : Marktplatz Haus-Nr.: 0 FW\n"
"\n"
"Ort : 91472 Ipsheim - Ipsheim Ipsheim\n"
"\n"
"Objekt : 5.1.3 NEA Feuerwehrgerätehaus Ipsheim\n"
"\n"
"Plannummer:\n"
"\n"
"Station\n"
"\n"
"---------------------------- -- ZIELORT ---------------------------------\n"
"Straße : Haus-Nr.:\n"
"\n"
"Ort :\n"
"\n"
"Objekt\n"
"\n"
"Station\n"
"\n"
"---------------------------- -- EINSATZGRUND ----------------------------\n"
"\n"
"Schlagw.: Technische Hilfe klein !!!\n"
"Stichwort B:\n"
"\n"
"Stichwort R:\n"
"\n"
"Stichwort S:\n"
"\n"
"Stichwort T: THL 1\n"
"\n"
"Prio. : 1\n"
"\n"
"(1 = Notfall / 2 = dringend / 3 = nicht zeitkritisch)\n"
"---------------------------- -- EINSATZMITTEL ----------------------------\n"
"Einsatzmittel : 5.1.3 NEA FF Ipsheim\n"
"\n"
"Alarmiert : 29.08.2015 14:35\n"
"\n"
"Geforderte Ausstattung\n"
"\n"
"---------------------------- -- BEMERKUNG -------------------------------\n"
"---------------------------- -- ENDE FAX -------------------------------\n"
"\n"
"Zur Übernahme eines Einsatzes Kontakt mit der ILS aufnehmen!\n"
"\n"
"Das Alarmfax darf nur zum internen Dienstgebrauch verwendet werden.\n"
"Der Empfänger hat sicherzustellen, dass unbefugte bzw. nicht beteiligte\n"
"Dritte keinen Zugang zu den übermittelten Daten haben - ILS Ansbach -\n"
"\n";

    return a;
}

static const string GetFaxContent2()
{
    string a = " \n"
"\n"
"EM n.s ensbac Booms (esii asoso-no ämo(eszuaiwwes ein e es.os.eois äci4=s7 E\n"
"---------------------- -- Alarmfax der ILS Ansbach ------------------------\n"
"\n"
"Absender : ILS ANSBACH\n"
"Fax : +49 (981) 65050-410 Rufnummer: +49 (981) 9776240\n"
"Termin .\n"
"\n"
"Einsatznummer: T 5.1 160308  275\n"
"\n"
"---------------------------- -- EINSATZORT ------------------------------\n"
"Straße : Mailheim    Haus-Nr.: \n"
"\n"
"Ort : 91472 Mailheim - Ipsheim Ipsheim\n"
"\n"
"Objekt : \n"
"\n"
"Plannummer:\n"
"\n"
"Station\n"
"\n"
"---------------------------- -- ZIELORT ---------------------------------\n"
"Straße : Haus-Nr.:\n"
"\n"
"Ort :\n"
"\n"
"Objekt\n"
"\n"
"Station\n"
"\n"
"---------------------------- -- EINSATZGRUND ----------------------------\n"
"\n"
"Schlagw.: VU mit PKW (bei Austritt von Betriebsstoffen) !!!\n"
"Stichwort B:\n"
"\n"
"Stichwort R:\n"
"\n"
"Stichwort S:\n"
"\n"
"Stichwort T: VU 1\n"
"\n"
"Prio. : 1\n"
"\n"
"(1 = Notfall / 2 = dringend / 3 = nicht zeitkritisch)\n"
"---------------------------- -- EINSATZMITTEL ----------------------------\n"
"Einsatzmittel : 5.1.3 NEA FF Ipsheim\n"
"Alarmiert : 08.03.2016 07:24\n"
"Geforderte Ausstattung : \n"
"Einsatzmittel : 5.1.3 FL IPS 42/1\n"
"Alarmiert : 08.03.2016 07:24\n"
"Geforderte Ausstattung : Gruppe (Takt. Einheit, Dispo)\n"
"Einsatzmittel : 5.1.3 Infoalarm FW-Führung NEA\n"
"Alarmiert : 08.03.2016 07:24\n"
"Geforderte Ausstattung : \n"
"\n"
"---------------------------- -- BEMERKUNG -------------------------------\n"
"\n"
"3 PKW\n"
"\n"
"---------------------------- -- ENDE FAX -------------------------------\n"
"\n"
"Zur Übernahme eines Einsatzes Kontakt mit der ILS aufnehmen!\n"
"\n"
"Das Alarmfax darf nur zum internen Dienstgebrauch verwendet werden.\n"
"Der Empfänger hat sicherzustellen, dass unbefugte bzw. nicht beteiligte\n"
"Dritte keinen Zugang zu den übermittelten Daten haben - ILS Ansbach -\n"
"\n";

    return a;
}

static const string CreateTemporaryFile(function<string()> getContent)
{
    boost::filesystem::path tmpFilenamePath = boost::filesystem::unique_path();
    tmpFilenamePath.replace_extension("txt");
    
    boost::filesystem::path tmpDirPath = boost::filesystem::temp_directory_path();
    boost::filesystem::path absoluteFilenamePath = tmpDirPath / tmpFilenamePath;
     
    const string tmpFilename = absoluteFilenamePath.native();
    
    ofstream file;
    file.open(tmpFilename, ios::out | ios::trunc | ios::binary);
    file << getContent();
    file.close();
    
    return tmpFilename;
}
