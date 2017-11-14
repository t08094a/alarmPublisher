#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "../include/ParserUtility.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE( ReadFaxTimestampResultsInCorrectOne )
{
    std::string line = "ILS Ansbac   +49 (981) 65050-410   0(9846)979725  1/1   24.07.2015   12:38";
    boost::posix_time::ptime expected(boost::gregorian::date(2015, 7, 24), boost::posix_time::time_duration(12, 38, 0));
    boost::posix_time::ptime fallback(boost::gregorian::date(2002, 1, 10), boost::posix_time::time_duration(1, 2, 3));

    boost::posix_time::ptime result = ParserUtility::ReadFaxTimestamp(line, fallback);
    
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( ReadFaxTimestampResultsInFallback )
{
    std::string line = "ILS Ansbac   +49 (981) 65050-410   0(9846)979725  1/1";
    boost::posix_time::ptime fallback(boost::gregorian::date(2002, 1, 10), boost::posix_time::time_duration(1, 2, 3));

    boost::posix_time::ptime result = ParserUtility::ReadFaxTimestamp(line, fallback);

    BOOST_CHECK(fallback == result);
}

BOOST_AUTO_TEST_CASE( AppendLineToString )
{
    std::string value = "abc";
    
    ParserUtility::AppendLine(value, "def");

    std::string expected = "abc\ndef";
    BOOST_CHECK(expected == value);
}


BOOST_AUTO_TEST_CASE( AppendEmptyStringShouldResultInNotModifiedString )
{
    std::string value = "abc";
    
    ParserUtility::AppendLine(value, "");

    std::string expected = "abc";
    BOOST_CHECK(expected == value);
}

BOOST_AUTO_TEST_CASE( LineStartsWithKeywordResultsInTrue )
{
    std::string line = "VU 1";
    std::vector<std::string> keywords { "THL", "VU" };
    std::string foundKeyword;

    bool result = ParserUtility::StartsWithKeyword(line, keywords, foundKeyword);

    BOOST_CHECK(true == result);
    BOOST_CHECK("VU" == foundKeyword);
}

BOOST_AUTO_TEST_CASE( LineStartsNotWithKeywordResultsInFalse )
{
    std::string line = "Technische Hilfe klein";
    std::vector<std::string> keywords { "THL", "VU" };
    std::string foundKeyword;

    bool result = ParserUtility::StartsWithKeyword(line, keywords, foundKeyword);

    BOOST_CHECK(false == result);
    BOOST_CHECK("" == foundKeyword);
}

BOOST_AUTO_TEST_CASE( GetMessageTextResultsInVuMitPkw )
{
    std::string line = "Schlagw. : VU mit PKW";
    std::string keyword = "Schlagw.";
    std::string expected = "VU mit PKW";

    std::string result = ParserUtility::GetMessageText(line, keyword);

    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( GetMessageTextWithoutKeywordResultsInVuMitPkw )
{
    std::string line = "VU mit PKW";
    std::string expected = "VU mit PKW";

    std::string result = ParserUtility::GetMessageText(line);
    
    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( GetMessageTextWithMissingKeywordResultsInVuMitPkw )
{
    std::string line = "VU mit PKW";
    std::string keyword = "Schlagw.";
    std::string expected = "VU mit PKW";

    std::string result = ParserUtility::GetMessageText(line, keyword);

    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( GetMessageTextWithKeywordAndTimestampResultsInTimestamp )
{
    std::string line = "Alarmiert                       : 13.09.2015 11:36";
    std::string keyword = "Alarmiert";
    std::string expected = "13.09.2015 11:36";

    std::string result = ParserUtility::GetMessageText(line, keyword);

    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( GetMessageTextWithEmptyKeywordAndTimestampResultsInTimestamp )
{
    std::string line = "Alarmiert                       : 13.09.2015 11:36";
    std::string keyword = "";
    std::string expected = "13.09.2015 11:36";

    std::string result = ParserUtility::GetMessageText(line, keyword);

    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( ReadZipCodeFromCityResultsIn91472 )
{
    std::string city = "91472 Ipsheim";

    std::string result = ParserUtility::ReadZipCodeFromCity(city);

    BOOST_CHECK("91472" == result);
}

BOOST_AUTO_TEST_CASE( ReadZipCodeFromCityWithLeadingWhitespaceResultsIn91472 )
{
    std::string city = "   91472 Ipsheim";

    std::string result = ParserUtility::ReadZipCodeFromCity(city);

    BOOST_CHECK("91472" == result);
}

BOOST_AUTO_TEST_CASE( ReadZipCodeFromCityWithWrongContentResultsInEmptyResult )
{
    std::string city = "Ipsheim";

    std::string result = ParserUtility::ReadZipCodeFromCity(city);

    BOOST_CHECK("" == result);
}

BOOST_AUTO_TEST_CASE( GetTimestampFromMessage )
{
    std::string timestamp = "13.09.2015 11:36";
    boost::posix_time::ptime fallback(boost::gregorian::date(2002, 1, 10), boost::posix_time::time_duration(1, 2, 3));
    boost::posix_time::ptime expected(boost::gregorian::date(2015, 9, 13), boost::posix_time::time_duration(11, 36, 0));

    boost::posix_time::ptime result = ParserUtility::TryGetTimestampFromMessage(timestamp, fallback);

    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( GetTimestampFromMessageOnlyWithDateResultsInCorrectDate )
{
    std::string timestamp = "13.09.2015";
    boost::posix_time::ptime fallback(boost::gregorian::date(2002, 1, 10), boost::posix_time::time_duration(1, 2, 3));
    boost::posix_time::ptime expected(boost::gregorian::date(2015, 9, 13));

    boost::posix_time::ptime result = ParserUtility::TryGetTimestampFromMessage(timestamp, fallback);

    BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE( GetTimestampFromMessageWithAbcResultsInFallback )
{
    std::string timestamp = "abc";
    boost::posix_time::ptime fallback(boost::gregorian::date(2002, 1, 10), boost::posix_time::time_duration(1, 2, 3));

    boost::posix_time::ptime result = ParserUtility::TryGetTimestampFromMessage(timestamp, fallback);

    BOOST_CHECK(fallback == result);
}

BOOST_AUTO_TEST_CASE( GetTimestampFromMessageWithEmptyStringResultsInFallback )
{
    std::string timestamp = "";
    boost::posix_time::ptime fallback(boost::gregorian::date(2002, 1, 10), boost::posix_time::time_duration(1, 2, 3));

    boost::posix_time::ptime result = ParserUtility::TryGetTimestampFromMessage(timestamp, fallback);

    BOOST_CHECK(fallback == result);
}

BOOST_AUTO_TEST_CASE( RemoveTrailingNewlineWithLineFeedResultsInDeletedLineFeed )
{
    std::string line = "VU mit PKW\n";
    std::string expected = "VU mit PKW";
    
    ParserUtility::RemoveTrailingNewline(line);

    BOOST_CHECK(expected == line);
}

BOOST_AUTO_TEST_CASE( RemoveTrailingNewlineWithoutLineFeedResultsInSameValue )
{
    std::string line = "VU mit PKW";
    std::string expected = "VU mit PKW";

    ParserUtility::RemoveTrailingNewline(line);

    BOOST_CHECK(expected == line);
}

BOOST_AUTO_TEST_CASE( RemoveTrailingNewlineWithInnerLineFeedResultsInSameValue )
{
    std::string line = "VU mit\n PKW";
    std::string expected = "VU mit\n PKW";

    ParserUtility::RemoveTrailingNewline(line);

    BOOST_CHECK(expected == line);
}

BOOST_AUTO_TEST_CASE( RemoveTrailingNewlineWithOnlySpacesResultsInEmptyValue )
{
    std::string line = "  ";
    std::string expected = "  ";

    ParserUtility::RemoveTrailingNewline(line);

    BOOST_CHECK(expected == line);
}

BOOST_AUTO_TEST_CASE( AnalyzeStreetLineResultsInCorrectStreet )
{
    std::string message = "Straße       : Oberndorfer Straße           Haus-Nr.: 12a";
    std::string street;
    std::string streetNumber;
    std::string appendix;

    ParserUtility::AnalyzeStreetLine(message, street, streetNumber, appendix);

    BOOST_CHECK("Oberndorfer Straße" == street);
    BOOST_CHECK("12a" == streetNumber);
    BOOST_CHECK("" == appendix);
}

BOOST_AUTO_TEST_CASE( AnalyzeStreetLineWithFreewayResultsInCorrectStreet )
{
    std::string message = "Straße       : B470           Haus-Nr./Km: 3";
    std::string street;
    std::string streetNumber;
    std::string appendix;

    ParserUtility::AnalyzeStreetLine(message, street, streetNumber, appendix);

    BOOST_CHECK("B470" == street);
    BOOST_CHECK("3" == streetNumber);
    BOOST_CHECK("" == appendix);
}

BOOST_AUTO_TEST_CASE( AnalyzeStreetLineWithFreewayWithoutHausNrResultsInCorrectStreet )
{
    std::string message = "Straße       : B470  ";
    std::string street;
    std::string streetNumber;
    std::string appendix;

    ParserUtility::AnalyzeStreetLine(message, street, streetNumber, appendix);

    BOOST_CHECK("B470" == street);
    BOOST_CHECK("" == streetNumber);
    BOOST_CHECK("" == appendix);
}

BOOST_AUTO_TEST_CASE( AnalyzeStreetLineWithoutHausNrResultsInCorrectStreet )
{
    std::string message = "Straße       : Oberndorfer Straße       ";
    std::string street;
    std::string streetNumber;
    std::string appendix;

    ParserUtility::AnalyzeStreetLine(message, street, streetNumber, appendix);

    BOOST_CHECK("Oberndorfer Straße" == street);
    BOOST_CHECK("1" == streetNumber);
    BOOST_CHECK("" == appendix);
}

BOOST_AUTO_TEST_CASE( AnalyzeStreetLineB470WithKmValueResultsInCorrectStreet )
{
    std::string message = "Straße       : B470    366 km";
    std::string street;
    std::string streetNumber;
    std::string appendix;

    ParserUtility::AnalyzeStreetLine(message, street, streetNumber, appendix);

    BOOST_CHECK("B470" == street);
    BOOST_CHECK("366" == streetNumber);
    BOOST_CHECK("" == appendix);
}

BOOST_AUTO_TEST_CASE( AnalyzeStreetLineWithHausNrAppendixResultsInCorrectStreet )
{
    std::string message = "Straße       : Oberndorfer Straße 1a Erdgeschoss";
    std::string street;
    std::string streetNumber;
    std::string appendix;

    ParserUtility::AnalyzeStreetLine(message, street, streetNumber, appendix);

    BOOST_CHECK("Oberndorfer Straße" == street);
    BOOST_CHECK("1a" == streetNumber);
    BOOST_CHECK("Erdgeschoss" == appendix);
}

BOOST_AUTO_TEST_CASE( A7ResultsInHighwayIsTrue )
{
    std::string line = "Straße       : A7";

    bool result = ParserUtility::IsHighway(line);

    BOOST_CHECK(true == result);
}

BOOST_AUTO_TEST_CASE( B470ResultsInHighwayIsTrue )
{
    std::string line = "Straße       : B470";

    bool result = ParserUtility::IsHighway(line);

    BOOST_CHECK(true == result);
}

BOOST_AUTO_TEST_CASE( St2252ResultsInHighwayIsTrue )
{
    std::string line = "Straße       : St2252";

    bool result = ParserUtility::IsHighway(line);

    BOOST_CHECK(true == result);
}

BOOST_AUTO_TEST_CASE( ST2252ResultsInHighwayIsTrue )
{
    std::string line = "Straße       : ST2252";

    bool result = ParserUtility::IsHighway(line);

    BOOST_CHECK(true == result);
}

BOOST_AUTO_TEST_CASE( NormalStreetResultsInHighwayIsFalse )
{
    std::string line = "Straße       : Oberndorfer Straße           Haus-Nr.: 12a";

    bool result = ParserUtility::IsHighway(line);

    BOOST_CHECK(false == result);
}

BOOST_AUTO_TEST_CASE( TrimStringVectorResultsInTrimmedLines )
{
    std::vector<std::string> lines;
    lines.push_back(" abc   ");
    lines.push_back("    def  ");
    lines.push_back("hij");

    ParserUtility::Trim(lines);

    BOOST_CHECK("abc" == lines.at(0));
    BOOST_CHECK("def" == lines.at(1));
    BOOST_CHECK("hij" == lines.at(2));
}

BOOST_AUTO_TEST_CASE( CoordinatesWithIntegersAreParsedCorrectly )
{
    string value = " X:   4389245 y:    5487632       ";

    GaussKruegerCoordinate coordinate = ParserUtility::ReadCoordinate(value);

    BOOST_CHECK("4389245" == coordinate.GetX());
    BOOST_CHECK("5487632" == coordinate.GetY());
    
    BOOST_CHECK(false == coordinate.IsEmpty());
}

BOOST_AUTO_TEST_CASE( CoordinatesWithFloatsAreParsedCorrectly )
{
    string value = " X:   4389245.59 y:    5487632.3974       ";

    GaussKruegerCoordinate coordinate = ParserUtility::ReadCoordinate(value);

    BOOST_CHECK("4389245.59" == coordinate.GetX());
    BOOST_CHECK("5487632.3974" == coordinate.GetY());
    
    BOOST_CHECK(false == coordinate.IsEmpty());
}

BOOST_AUTO_TEST_CASE( CoordinatesWithEmptyValuesAreParsedToZero )
{
    string value = " X: 0 y: 0       ";

    GaussKruegerCoordinate coordinate = ParserUtility::ReadCoordinate(value);

    BOOST_CHECK("0" == coordinate.GetX());
    BOOST_CHECK("0" == coordinate.GetY());
    
    BOOST_CHECK(true == coordinate.IsEmpty());
}
