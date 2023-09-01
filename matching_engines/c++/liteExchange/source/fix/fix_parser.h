#ifndef _FIX_PARSER_
#define _FIX_PARSER_

#include <core/string_utility.h>
#include <string>
#include <cstddef>
#include <vector>
#include "fix_constants.h"
#include "fix_message.h"

namespace fix
{

class FixParser
{
    public:

        static void parseFromString(FixMessage& message, const std::string& input)
        {
            auto tagValuePairs = core::split(input, FixConstants::FIX_DELIMITER);

            for (auto& tagValuePair : tagValuePairs)
            {
                if (tagValuePair.length() == 0)
                {
                    continue;
                }

                auto tokens = core::split(tagValuePair, FixConstants::FIX_EQUALS);
                int tag = std::stoi(tokens[0]);
                std::string value = tokens[1];
                message.setTag(tag, value);
            }
            message.initialiseHeader();
        }

        static void parseFromBuffer(FixMessage& message, const char* input, std::size_t length)
        {
            std::string tag, value;
            std::string* target = &tag;

            for (std::size_t i{ 0 }; i < length; i++)
            {
                char iter = input[i];

                if (iter == FixConstants::FIX_EQUALS)
                {
                    target = &value;
                }
                else if (iter == FixConstants::FIX_DELIMITER)
                {
                    target = &tag;
                    message.setTag(std::stoi(tag), value);
                    tag = "";
                    value = "";
                }
                else
                {
                    (*target).push_back(iter);
                }
            }
            message.initialiseHeader();
        }

        static void parseFromMultipleBuffers(FixMessage& message, const char* buffer1, std::size_t length1, const char* buffer2, std::size_t length2)
        {
            std::string tag, value;
            std::string* target = &tag;
            const char* buffer = &(buffer1[0]);
            std::size_t iterations = length1;
            bool switchedToSeconBuffer{ false };

            for (std::size_t i{ 0 }; i < iterations; i++)
            {
                char iter = buffer[i];

                if (iter == FixConstants::FIX_EQUALS)
                {
                    target = &value;
                }
                else if (iter == FixConstants::FIX_DELIMITER)
                {
                    target = &tag;
                    message.setTag(std::stoi(tag), value);
                    tag = "";
                    value = "";
                }
                else
                {
                    (*target).push_back(iter);
                }

                if (i == iterations - 1)
                {
                    if (switchedToSeconBuffer)
                    {
                        break;
                    }
                    else
                    {
                        // Switch to reading other buffer
                        i = -1;
                        iterations = length2;
                        buffer = &(buffer2[0]);
                        switchedToSeconBuffer = true;
                    }
                }
            }
            message.initialiseHeader();
        }

        static std::size_t parseMultipleMessagesFromBuffer(char* buffer, std::size_t bufferSize, std::vector<FixMessage>* messages, std::size_t& remainingBytesStartIndex)
        {
            std::size_t numOfFixMessages{ 0 };

            std::string tag, value;
            std::string* target = &tag;
            FixMessage message;
            bool lastTagOfMessage{ false };
            bool fixMessageStarted{ false };

            for (std::size_t i{ 0 }; i < bufferSize; i++)
            {
                char iter = buffer[i];

                if (iter == '8')
                {
                    if (buffer[i + 1] == FixConstants::FIX_EQUALS)
                    {
                        (*target).push_back(iter);
                        fixMessageStarted = true;
                        ++i;
                        iter = buffer[i];
                    }
                }

                if (fixMessageStarted == false)
                {
                    continue;
                }

                if (iter == FixConstants::FIX_EQUALS)
                {
                    if (tag.length() == 2)
                    {
                        // CHECKSUM TAG
                        if (tag[0] == '1' && tag[1] == '0')
                        {
                            lastTagOfMessage = true;
                        }
                    }

                    target = &value;
                }
                else if (iter == FixConstants::FIX_DELIMITER)
                {
                    target = &tag;
                    message.setTag(std::stoi(tag), value);

                    if (lastTagOfMessage)
                    {
                        message.initialiseHeader();
                        messages->emplace_back(message);
                        remainingBytesStartIndex = i + 1;
                        message.reset();
                        numOfFixMessages++;
                        lastTagOfMessage = false;
                        fixMessageStarted = false;
                    }

                    tag = "";
                    value = "";
                }
                else
                {
                    if (fixMessageStarted)
                    {
                        (*target).push_back(iter);
                    }
                }

            }

            return numOfFixMessages;
        }
};

}

#endif