/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  Jochen Fähnlein <jochen.faehnlein@ff-ipsheim.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace std;

// singleton
class ConfigReader : private boost::noncopyable
{
private:
    bool initialized;
    boost::property_tree::ptree pt;
    
    ConfigReader();
        
    void Initialize();
    
public:
    ~ConfigReader();
    
    // make sure they are unacceptable otherwise we may accidentally get copies of our singleton appearing
    ConfigReader(ConfigReader const&) = delete;
    void operator=(ConfigReader const&) = delete;
    
    static ConfigReader& GetInstance()
    {
        static ConfigReader instance; // Guaranteed to be destroyed
        return instance; // Instantiated on first use.
    }

    string Get( const string& path ) const;
    string GetTelephonNumbers() const;
};

#endif // CONFIGREADER_H
