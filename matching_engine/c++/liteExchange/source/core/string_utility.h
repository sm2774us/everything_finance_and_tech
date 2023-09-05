#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <cctype>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdarg>
#include <memory>
#include <cstdio>
#include <stdexcept>

namespace core
{

inline bool replaceInString(std::string& target, const std::string& from, const std::string& to)
{
    auto startPos = target.find(from);

    if (startPos == std::string::npos)
    {
        return false;
    }

    target.replace(startPos, from.length(), to);
    return true;
}

inline void replaceCharacterInString(std::string& target, char oldChar, char newChar)
{
    std::replace(target.begin(), target.end(), oldChar, newChar);
}

inline bool contains(const std::string& haystack, const std::string& needle)
{
    if (haystack.find(needle) != std::string::npos)
    {
        return true;
    }

    return false;
}

inline std::vector<std::string> split(const std::string& input, char seperator)
{
    std::vector<std::string> ret;
    if (input.length() > 0)
    {
      std::istringstream stream(input);
      std::string token;
      while (std::getline(stream, token, seperator))
      {
        ret.push_back(token);
      }
    }
    return ret;
}

inline void padLeft(std::string& input, std::size_t number, char padChar)
{
    auto inputSize = input.size();
    if (number > inputSize)
    {
        input.insert(0, number - inputSize, padChar);
    }
}

template <typename T>
inline T toLower(const T& input)
{
    T ret = input;
    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}

template <typename T>
inline T toUpper(const T& input)
{
    T ret = input;
    std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
    return ret;
}

inline bool compare(const std::string& first, const std::string& second)
{
    bool same = false;

    auto firstLowerCase = toLower(first);
    auto secondLowerCase = toLower(second);

    if (!strcmp(firstLowerCase.c_str(), secondLowerCase.c_str()))
    {
      same = true;
    }

    return same;
}

inline bool startsWith(const std::string& inputString, char c)
{
    if (inputString.length() > 0)
    {
        if (inputString[0] == c)
        {
            return true;
        }
    }
    return false;
}

inline void trimLeft(std::string& inputString)
{
    // Removes spaces on the left side
    auto iter = std::find_if(inputString.begin(), inputString.end(), [](int ch){ return !std::isspace(ch); });
    inputString.erase(inputString.begin(), iter);
}

inline void trimRight(std::string& inputString)
{
    // Removes spaces on the right side
    auto iter = std::find_if(inputString.rbegin(), inputString.rend(), [](int ch){ return !std::isspace(ch); });
    inputString.erase(iter.base(), inputString.end());
}

inline void trim(std::string& inputString)
{
    trimLeft(inputString);
    trimRight(inputString);
}

inline void toWideString(const std::string& source, std::wstring& destination)
{
    std::wstringstream wideStringStream;
    wideStringStream << source.c_str();
    destination = wideStringStream.str();
}

inline void toNarrowString(const std::wstring& source, std::string& destination)
{
    std::string narrowString(source.begin(), source.end());
    destination = narrowString;
}

namespace details
{
    // C String formatting functions are not compatible with std::string by design ,
    // we need to use c_str() of std::string, therefore we use function overloads as below
    template <typename... Args>
    std::unique_ptr<char[]> formatImplementation(std::size_t bufferSize, const char* formatStr,    Args&&... args)
    {
        std::unique_ptr<char[]> buf(new char[bufferSize]);

#if _MSC_VER
        std::size_t length = sprintf_s(buf.get(), bufferSize, formatStr, args...);
#else
        std::size_t length = snprintf(buf.get(), bufferSize, formatStr, args...);
#endif

        if (length < 0 && length >= bufferSize)
        {
            throw std::runtime_error("String formatter , not enough buffer size");
        }

        return buf;
    }

    inline char const* ifStringThenConvertToCharBuffer(std::string const& str)
    {
        return str.c_str();
    }

    template <typename T>
    T ifStringThenConvertToCharBuffer(T const& t)
    {
        return t;
    }
}

template <typename... Args>
std::string format(std::string const& formatString, Args&&... args)
{
    const std::size_t MAX_BUFFER_SIZE = 2048; // Will throw exception if not enough
    std::unique_ptr<char[]> chars = details::formatImplementation(MAX_BUFFER_SIZE, formatString.c_str(), details::ifStringThenConvertToCharBuffer(args)...);
    return std::string(chars.get());
}

}// namespace

#endif