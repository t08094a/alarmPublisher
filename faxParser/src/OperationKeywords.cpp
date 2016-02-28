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

#include "OperationKeywords.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

OperationKeywords::OperationKeywords()
{
    // nop
}

OperationKeywords::OperationKeywords (const OperationKeywords& other)
{
    this->keyword = other.keyword;
    this->emergencyKeyword = other.emergencyKeyword;
    this->b = other.b;
    this->r = other.r;
    this->s = other.s;
    this->t = other.t;
}

OperationKeywords::~OperationKeywords()
{
    // nop
}

OperationKeywords& OperationKeywords::operator= (const OperationKeywords& other)
{
    if (this == &other)
    {
        return *this; // handle self assignment
    }
    
    this->keyword = other.keyword;
    this->emergencyKeyword = other.emergencyKeyword;
    this->b = other.b;
    this->r = other.r;
    this->s = other.s;
    this->t = other.t;
    
    return *this;
}

bool OperationKeywords::operator== (const OperationKeywords& other) const
{
    return this->keyword == other.keyword &&
           this->emergencyKeyword == other.emergencyKeyword &&
           this->b == other.b &&
           this->r == other.r &&
           this->s == other.s &&
           this->t == other.t;
}

bool OperationKeywords::operator!= (const OperationKeywords& other) const
{
    return ! operator==(other);
}

string OperationKeywords::GetKeyword() const
{
    return keyword;
}

void OperationKeywords::SetKeyword(const string keyword)
{
    this->keyword = boost::trim_copy(keyword);
}

string OperationKeywords::GetEmergencyKeyword() const
{
    return emergencyKeyword;
}

void OperationKeywords::SetEmergencyKeyword(const string keyword)
{
    emergencyKeyword = boost::trim_copy(keyword);
}

string OperationKeywords::GetB() const
{
    return b;
}

void OperationKeywords::SetB(const string b)
{
    this->b = boost::trim_copy(b);
}

string OperationKeywords::GetR() const
{
    return r;
}

void OperationKeywords::SetR(const string r)
{
    this->r = boost::trim_copy(r);
}

string OperationKeywords::GetS() const
{
    return s;
}

void OperationKeywords::SetS(const string s)
{
    this->s = boost::trim_copy(s);
}

string OperationKeywords::GetT() const
{
    return t;
}

void OperationKeywords::SetT(const string t)
{
    this->t = boost::trim_copy(t);
}

string OperationKeywords::ToString() const
{
    vector<string> parts;

    if(keyword.empty() == false)
    {
        parts.push_back("Stichwort: " + keyword);
    }

    if(emergencyKeyword.empty() == false)
    {
        parts.push_back("Stichwort: " + emergencyKeyword);
    }

    if(b.empty() == false)
    {
        parts.push_back("B: " + b);
    }

    if(r.empty() == false)
    {
        parts.push_back("R: " + r);
    }

    if(s.empty() == false)
    {
        parts.push_back("S: " + s);
    }

    if(t.empty() == false)
    {
        parts.push_back("T: " + t);
    }

    string result = boost::algorithm::join(parts, ", ");

    return result;
}
