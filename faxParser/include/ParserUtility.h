/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Jochen Fähnlein <jochen.faehnlein@ff-ipsheim.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PARSERUTILITY_H
#define PARSERUTILITY_H

#include <string>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>
#include "GaussKruegerCoordinate.h"

using namespace std;

class ParserUtility
{
private:
    static bool IsStringEmptyOrWhitespace(const string& value);
    
public:
    /**
     * @brief Appends a given string with a new line and a given content.
     *        If the existing value is null or empty the new content gets used for the beginning.
     * @param value Existing string
     * @param line New string for new line
     */
    static void AppendLine(string &value, const string &line);

    /**
     * @brief Attempts to parse the timestamp from a line, using a well-known date and time format.
     * @param line The line of text to parse.
     * @param fallback The DateTime-instance to use as the fallback, if parsing was unsuccessful.
     * @return A DateTime-instance that contains a usable timestamp.
     */
    static boost::posix_time::ptime ReadFaxTimestamp(const string &line, const boost::posix_time::ptime &fallback);

    /**
    * @brief Determines whether or not the given line starts with a keyword from the given keyword-collection.
    * 
    * @param line const string& The line to check for keywords.
    * @param keywords const vector<string> The list of keywords to use.
    * @param keyword string& If the line starts with a keyword from the keywords-list, this parameter contains the keyword. Otherwise, null.
    * @return bool A boolean value indicating whether or not the given line starts with a keyword from the keywords-collection.
    */
    static bool StartsWithKeyword(const string& line, const vector<string> keywords, string& keyword);

    /**
    * @brief Returns the message text, which is the line text but excluding the keyword/prefix and a possible colon.
    * 
    * @param line const string& The line text to retrieve the message text from.
    * @param prefix const string& The prefix that is to be removed (optional).
    * @return std::__cxx11::string
    */
    static string GetMessageText(const string& line, const string &prefix = "");

    /**
    * @brief Attempts to read the zip code from the city, if available.
    * 
    * @param cityText const string& The city; may or may not contain the zip code prefixing the name.
    * @return std::__cxx11::string The zip code of the city. -or- null, if there was no.
    */
    static string ReadZipCodeFromCity(const string &cityText);

    /**
    * @brief Attempts to convert the given message into a datetime.
    * 
    * @param message const string& The message that presumably represents a timestamp.
    * @param fallback const boost::posix_time::ptime& The DateTime-instance to use if parsing failed.
    * @return boost::posix_time::ptime A valid <see cref="DateTime"/>, either parsed from the message or representing the fallback value.
    */
    static boost::posix_time::ptime TryGetTimestampFromMessage(const string& message, const boost::posix_time::ptime& fallback);

    /**
    * @brief Removes an existing trailing newline from the given string.
    * 
    * @param value string& The string to remove an existing trailing newline from.
    */
    static void RemoveTrailingNewline(string& value);

    /**
    * @brief Splits the provided 'streetline' into the parts street, streetnumber and appendix.
    *        It also checks whether we're maybe ordered on a highway or not. In that case the kilometer is stored in the field streetnumber.
    * 
    * @param line (in) string& The line from the alarmfax which should be splitted.
    * @param street string& (out) The street found in the line.
    * @param streetNumber string& (out) Either a house number or the kilometer on the highway.
    * @param appendix string& (out) The 'rest' behind the house number e.g. the floor or further information about the location.
    */
    static void AnalyzeStreetLine(const string& line, string& street, string& streetNumber, string& appendix);

    /**
    * @brief Gets the text between two strings. Provides the possibilty to use StringComparison..
    * 
    * @param line string The text which should be used for searching
    * @param from The start string
    * @param until The end string
    * @return std::__cxx11::string string The inner string.
    */
    static string GetTextBetween(string line, string from = nullptr, string until = nullptr/*, StringComparison comparison = StringComparison.InvariantCultureIgnoreCase*/);

    /**
    * @brief Analyzes whether the given line contains an highway declaration.
    * 
    * @param line the string to analyze.
    * @return bool True if the string contains an highway; otherwise false.
    */
    static bool IsHighway(const string& line);
    
    /**
    * @brief Reads the coordinates given from a string value in the form "X: 4389245 Y: 5487632"
    * 
    * @param value The coordinates in string form to parse.
    * @return GaussKruegerCoordinate
    */
    static GaussKruegerCoordinate ReadCoordinate(const string& value);

    /**
    * @brief Trims all lines from the array, which means that lines with zero length will be omitted.
    *        originally from: AlarmWorkflow.Shared.Core.Utilities
    * 
    * @param lines the list of lines to trim.
    */
    static void Trim(vector<string>& lines);
    
    /**
    * @brief Erases all empty items from the given vector.
    * 
    * @param lines the list of lines to cleanup.
    */
    static void EraseEmptyItems(vector<string>& lines);
};

#endif // PARSERUTILITY_H
