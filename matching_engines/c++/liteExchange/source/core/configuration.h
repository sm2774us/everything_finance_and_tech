#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include<string>
#include<vector>
#include<unordered_map>

namespace core
{

class Configuration
{
    public :

        static void loadFromFile(const std::string& fileName, Configuration& config);

        bool doesAttributeExist(const std::string& attribute) const;
        void addAttribute(const std::string& attribute, const std::string& value);

        const std::string getStringValue(const std::string& attribute, std::string defaultVal="") const;
        bool getBoolValue(const std::string& attribute, bool defaultVal=false) const;
        int getIntValue(const std::string& attribute, int defaultVal=0) const;
        std::vector<std::string> getArray(const std::string& attribute);

        Configuration getSubConfiguration(const std::string& attributeSubString);

    private:

        mutable std::unordered_multimap<std::string, std::string> m_dictionary;
};

}//namespace
#endif