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

#include "ParserUtility.h"
#include <algorithm>
#include <cctype>
#include <exception>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/iostreams/filter/newline.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

void ParserUtility::AppendLine(string& value, const string& line)
{
    if(line.empty() == false)
    {
        value = value + boost::iostreams::newline::LF + line;
    }
}

boost::posix_time::ptime ParserUtility::ReadFaxTimestamp(const string& line, const boost::posix_time::ptime& fallback)
{
    const boost::regex dateRegex{"(0[1-9]|[12][0-9]|3[01])[- /.](0[1-9]|1[012])[- /.](19|20)\\d\\d"};
    const boost::regex timeRegex("([01]?[0-9]|2[0-3]):[0-5][0-9]");

    boost::posix_time::ptime date = fallback;

    boost::posix_time::time_duration timestamp = date.time_of_day();

    boost::smatch date_matches;
    boost::smatch time_matches;

    bool matchesDate = boost::regex_search(line, date_matches, dateRegex);
    bool matchesTime = boost::regex_search(line, time_matches, timeRegex);

    try
    {
        if(matchesDate)
        {
            const std::locale format = std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%d.%m.%Y"));

            std::string content = date_matches.str();

            std::istringstream is(content);
            is.imbue(format);
            is >> date;
        }

        if(matchesTime)
        {
            const std::locale format = std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%H:%M"));

            std::string content = time_matches.str();

            std::istringstream is(content);
            is.imbue(format);
            is >> timestamp;
        }
    }
    catch(const exception& ex)
    {
        std::cerr << "An exception occurred: " << ex.what() << std::endl;
        return fallback;
    }
    catch(...)
    {
        std::cerr << "whoops!" << std::endl;
        return fallback;
    }

    boost::posix_time::ptime t(date.date(), timestamp);

    return t;
}

bool ParserUtility::StartsWithKeyword(const string &line, const vector<string> keywords, string &keyword)
{
    for(auto &kwd : keywords)
    {
        string lowerLine = boost::to_lower_copy(line);
        string lowerKeyword = boost::to_lower_copy(kwd);

        if(boost::starts_with(lowerLine, lowerKeyword))
        {
            keyword = kwd;
            return true;
        }
    }

    keyword = "";
    return false;
}

string ParserUtility::GetMessageText(const string &line, const string &prefix)
{
    std::string text = line;

    if(prefix.empty() == false)
    {
        boost::algorithm::erase_first(text, prefix);
        boost::algorithm::trim(text);
    }
    else
    {
        std::string::size_type colonIndex = text.find(":");

        if(colonIndex != std::string::npos)
        {
            text.erase(0, colonIndex + 1);
        }
    }

    if(boost::starts_with(text, ":"))
    {
        text.erase(0, 1);
    }

    boost::algorithm::trim(text);

    return text;
}

string ParserUtility::ReadZipCodeFromCity(const string& cityText)
{
    string zipCode;

    string trimmedCityText = boost::algorithm::trim_copy(cityText);

    for(char c : trimmedCityText)
    {
        try
        {
            boost::lexical_cast<short>(c);
            zipCode += c;
        }
        catch(boost::bad_lexical_cast& e)
        {
            break;
        }
    }

    return zipCode;
}

boost::posix_time::ptime ParserUtility::TryGetTimestampFromMessage(const string &message, const boost::posix_time::ptime &fallback)
{
    boost::posix_time::ptime dt = fallback;
    const std::locale format = std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%d.%m.%Y %H:%M"));

    try
    {
        std::istringstream is(message);
        is.imbue(format);
        is >> dt;

        std::cout << "Parsed \"" << message << "\" to \"" << dt << "\" (Fallback: " << fallback << ")" << std::endl;

        return dt;
    }
    catch(exception& ex)
    {
        std::cerr << "An exception occurred: " << ex.what() << std::endl;
    }

    return dt;
}

void ParserUtility::RemoveTrailingNewline(std::string &value)
{
    if(IsStringEmptyOrWhitespace(value) == false &&
       boost::algorithm::ends_with(value, "\n"))
    {
        value.erase(value.length() - 1);
        boost::algorithm::trim(value);
    }
}

void ParserUtility::AnalyzeStreetLine(const string& line, string& street, string& streetNumber, string& appendix)
{
    std::string workingLine = line;

    // initialize the output
    street = "";
    streetNumber = "1";
    appendix = "";

    boost::regex streetRegex("Straße\\s*:\\s*");
    boost::regex nrRegex("Haus-?Nr.(\\/Km)?:");

    boost::smatch str_matches;
    bool matches = boost::regex_search(workingLine, str_matches, nrRegex);

    if(IsHighway(workingLine))
    {
        if(matches == false)
        {
            boost::regex digitRegex(" \\d+");
            boost::smatch digitMatches;

            bool matchesDigit = boost::regex_search(workingLine, digitMatches, digitRegex);

            if(matchesDigit == false)
            {
                streetNumber = "";
            }
            else
            {
                string::const_iterator begin = workingLine.begin();
                string::const_iterator end = workingLine.end();

                while (boost::regex_search(begin, end, digitMatches, digitRegex))
                {
                    boost::smatch::const_reference r = digitMatches["Group"];
                    begin = r.second;
                    
                    streetNumber = digitMatches.str();
                }

                // now digitMatches contains the last occurence
                boost::algorithm::trim(streetNumber);

                // remove all characters from found number to the end
                string::size_type foundIndex = workingLine.find(streetNumber);
                if(foundIndex != string::npos)
                {
                    workingLine.erase(foundIndex);
                }
            }
        }
        else
        {
            streetNumber = workingLine.substr(str_matches.position() + str_matches.length());
            boost::algorithm::trim(streetNumber);

            workingLine = workingLine.substr(0, str_matches.position());
            boost::algorithm::trim(workingLine);
        }

        // remove "Straße :"
        street = boost::regex_replace(workingLine, streetRegex, "", boost::format_first_only);
        boost::algorithm::trim(street);
    }
    else if(matches == false)
    {
        boost::regex streetNumberRegex("\\d+[ ]*[a-zA-Z]*");
        bool matchesStreetNumber = boost::regex_search(workingLine, str_matches, streetNumberRegex);

        if(matchesStreetNumber == false)
        {
            street = workingLine;
        }
        else
        {
            streetNumber = str_matches.str();
            boost::algorithm::trim(streetNumber);

            street = workingLine.substr(0, str_matches.position());
            boost::algorithm::trim(street);

            int length = str_matches.length() + str_matches.position();
            appendix = workingLine.substr(length);
            boost::algorithm::trim(appendix);
        }

        // remove "Straße :"
        street = boost::regex_replace(street, streetRegex, "", boost::format_first_only);
        boost::algorithm::trim(street);
    }
    else
    {
        street = workingLine.substr(0, str_matches.position());

        // remove "Straße :"
        street = boost::regex_replace(street, streetRegex, "", boost::format_first_only);
        boost::algorithm::trim(street);

        workingLine = workingLine.substr(str_matches.position());
        boost::algorithm::trim(workingLine);

        boost::regex streetNumberRegex("\\d+[ ]*[a-z]*");
        bool matchesStreetNumber = boost::regex_search(workingLine, str_matches, streetNumberRegex);

        if(matchesStreetNumber)
        {
            streetNumber = str_matches.str();
            boost::algorithm::trim(street);

            int length = str_matches.length() + str_matches.position();
            appendix = workingLine.substr(length);
            boost::algorithm::trim(appendix);
        }
    }
}

bool ParserUtility::IsHighway(const string& line)
{
    // contains line a value with "Autobahn", "Bundesstraße" or "Staatsstraße", e.g. A7, B470, ST2252
    static std::vector<string> streetTokens = { "A\\d+", "B\\d+", "S[tT]\\d+" };
    boost::smatch str_matches;

    for(string token : streetTokens)
    {
        boost::regex streetRegex(token);
        bool matches = boost::regex_search(line, str_matches, streetRegex);

        if(matches)
        {
            return true;
        }
    }

    return false;
}

void ParserUtility::Trim(std::vector<string>& lines)
{
    for(string& line : lines)
    {
        boost::algorithm::trim(line);
    }
}

bool ParserUtility::IsStringEmptyOrWhitespace(const std::string& value)
{
    if(value.empty())
    {
        return true;
    }

    // if string contains only spaces
    if(std::all_of(value.begin(), value.end(), [](char c) { return std::isspace(c, std::cin.getloc()); }))
    {
        return true;
    }

    return false;
}
