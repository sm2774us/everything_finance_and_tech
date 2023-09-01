#include "configuration.h"
#include <fstream>
#include <algorithm>
#include <ctype.h>
#include <cstddef>
#include <core/pretty_exception.h>
#include <core/string_utility.h>
using namespace std;

namespace core
{

void Configuration::addAttribute(const std::string& attribute, const std::string& value)
{
    m_dictionary.insert(std::make_pair(attribute, value));
}

void Configuration::loadFromFile(const string& fileName, Configuration& config)
{
    // For reusability
    config.m_dictionary.clear();

    ifstream file(fileName); // ifstream dtor also closes the file so no need for using a smart ptr to close the file

    if ( ! file.good())
    {
        THROW_PRETTY_RUNTIME_EXCEPTION(core::format("File %s could not be opened", fileName))
    }

    file.seekg(0, std::ios::beg);
    string line;
    unsigned long lineNumber(0);

    while ( std::getline(file, line) )
    {
        lineNumber++;

        core::trim(line);
        auto lineLength = line.length();

        if (line.c_str()[0] == '#' || lineLength == 0 ) // Skip comment lines and empty lines
        {
            continue;
        }

        if ( lineLength < 3)
        {
            THROW_PRETTY_RUNTIME_EXCEPTION( core::format("Line is too short , line number : %d", lineNumber) )
        }

        size_t equalsPos = line.find("=", 0);

        if (equalsPos  == std::string::npos)
        {
            THROW_PRETTY_RUNTIME_EXCEPTION( core::format("Line doesn`t contain equals sign , line number : %d", lineNumber))
        }

        if (line.find("=", equalsPos+1 ) != std::string::npos)
        {
            THROW_PRETTY_RUNTIME_EXCEPTION( core::format("Line contains more than one equals sign , line number : %d", lineNumber) )
        }

        auto tokens = core::split(line, '=');
        string attribute = tokens[0];
        string value = tokens[1];
        config.addAttribute(attribute, value);
    }

    file.close();
}

bool Configuration::doesAttributeExist(const std::string& attribute) const
{
    auto element = m_dictionary.find(attribute);
    if (element == m_dictionary.end())
    {
        return false;
    }
    return true;
}

const string Configuration::getStringValue(const string& attribute, string defaultVal) const
{
    if (doesAttributeExist(attribute) == false)
    {
        return defaultVal;
    }

    auto element = m_dictionary.find(attribute);
    return element->second;
}

bool Configuration::getBoolValue(const string& attribute, bool defaultVal) const
{
    if (doesAttributeExist(attribute) == false)
    {
        return defaultVal;
    }

    auto stringVal = getStringValue(attribute);
    stringVal = core::toLower(stringVal);
    return (stringVal == "true") ? true : false;
}

int Configuration::getIntValue(const string& attribute, int defaultVal) const
{
    if (doesAttributeExist(attribute) == false)
    {
        return defaultVal;
    }

    return std::stoi(getStringValue(attribute));
}

vector<string> Configuration::getArray(const string& attribute)
{
    vector<string> ret;
    string actualAttribute = attribute + "[]";

    auto range = m_dictionary.equal_range(actualAttribute);

    for_each(
        range.first,
        range.second,
        [&](unordered_multimap<string, string>::value_type& element){ ret.push_back(element.second); }
    );

    return ret;
}

Configuration Configuration::getSubConfiguration(const string& attributeSubString)
{
    Configuration subConfiguration;

    for (const auto& attribute : m_dictionary)
    {
        if (core::contains(attribute.first, attributeSubString))
        {
            subConfiguration.addAttribute(attribute.first, attribute.second);
        }
    }

    return subConfiguration;
}

}//namespace