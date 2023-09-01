#include "fix_message.h"
#include "fix_constants.h"
#include "fix_parser.h"
#include <core/string_utility.h>
#include <core/pretty_exception.h>
#include <cstring>
#include <fstream>
using namespace std;

namespace fix
{

FixMessage::FixMessage() : m_fixVersion{ FixConstants::FixVersion::FIX_VERSION_NONE }, m_sequenceNumber{1}
{
    m_bodyTagValuePairs.reserve(16);
}

void FixMessage::reset()
{
    m_fixVersion = FixConstants::FixVersion::FIX_VERSION_NONE;
    m_messageType = (char)NULL;
    m_sequenceNumber = 1;
    m_senderCompId = "";
    m_targetCompId = "";
    m_sendingTime = "";
    m_bodyTagValuePairs.clear();
}

bool FixMessage::isAdminMessage() const
{
    auto type = getMessageType();

    if (type == FixConstants::MessageType::LOGOFF || type == FixConstants::MessageType::LOGON
        || type == FixConstants::MessageType::HEARTBEAT || type == FixConstants::MessageType::TEST_REQUEST
        || type == FixConstants::MessageType::USER_LOGON || type == FixConstants::MessageType::ADMIN_REJECT)
    {
        return true;
    }

    return false;
}

int FixMessage::calculateHeaderLength() const
{
    const int lengthExcludingValue{ 4 };
    int headerLength{ 20 };

    // 20 = 4 * 5
    // 4 = delimiter + equals sign + tag length for all header tags
    // 5 = 35 34 49 52 56

    // FIX MESSAGE TYPE 35
    headerLength += strlen(FixConstants::FIX_MESSAGE_TYPE_STRINGS[m_messageType]);

    // FIX SEQUENCE NUMBER 34
    headerLength += std::to_string(m_sequenceNumber).length();

    // FIX SENDER COMPID 49
    headerLength += m_senderCompId.length();

    // FIX SENDING TIME 52
    headerLength += m_sendingTime.length();

    // FIX TARGET COMPID 56
    headerLength += m_targetCompId.length();

    return headerLength;
}

int FixMessage::calculateBodyLength() const
{
    int bodyLength{ 0 };
    const int seperatorAndEqualsLength{ 2 };


    for (const auto& tagValue : m_bodyTagValuePairs)
    {
        int tagValueKey = tagValue.first;
        auto value = tagValue.second;
        int keyLength = std::to_string(tagValueKey).length();

        bodyLength += keyLength + seperatorAndEqualsLength + value.length(); // +2 is because of = and delimiter
    }

    bodyLength += calculateHeaderLength();

    return bodyLength;
}

void FixMessage::removeTag(int tag)
{
    m_bodyTagValuePairs.erase(tag);
}

int FixMessage::getMessageType() const
{
    return m_messageType;
}

void FixMessage::initialiseHeader()
{
    if (hasTag(FixConstants::FIX_TAG_VERSION))
    {
        setFixVersion(FixConstants::getFixVersionFromString(getTagValue(FixConstants::FIX_TAG_VERSION)));
        removeTag(FixConstants::FIX_TAG_VERSION);
    }

    if (hasTag(FixConstants::FIX_TAG_MESSAGE_TYPE))
    {
        setMessageType(FixConstants::getMessageTypeFromString(getTagValue(FixConstants::FIX_TAG_MESSAGE_TYPE)));
        removeTag(FixConstants::FIX_TAG_MESSAGE_TYPE);
    }

    if (hasTag(FixConstants::FIX_TAG_SEQUENCE_NUMBER))
    {
        setSequenceNumber(getTagValueAsInt(FixConstants::FIX_TAG_SEQUENCE_NUMBER));
        removeTag(FixConstants::FIX_TAG_SEQUENCE_NUMBER);
    }

    if (hasTag(FixConstants::FIX_TAG_SENDING_TIME))
    {
        setSendingTime(getTagValue(FixConstants::FIX_TAG_SENDING_TIME));
        removeTag(FixConstants::FIX_TAG_SENDING_TIME);
    }

    if (hasTag(FixConstants::FIX_TAG_SENDER_COMPID))
    {
        setSenderCompId(getTagValue(FixConstants::FIX_TAG_SENDER_COMPID));
        removeTag(FixConstants::FIX_TAG_SENDER_COMPID);
    }

    if (hasTag(FixConstants::FIX_TAG_TARGET_COMPID))
    {
        setTargetCompId(getTagValue(FixConstants::FIX_TAG_TARGET_COMPID));
        removeTag(FixConstants::FIX_TAG_TARGET_COMPID);
    }
}

bool FixMessage::hasTag(int tag) const
{
    auto element = m_bodyTagValuePairs.find(tag);
    if (element == m_bodyTagValuePairs.end())
    {
        return false;
    }
    return true;
}

void FixMessage::setTag(int tag, const string& value)
{
    m_bodyTagValuePairs[tag] = value;
}

void FixMessage::setTag(int tag, int value)
{
    setTag(tag, std::to_string(value));
}

void FixMessage::setTag(int tag, char value)
{
    string temp;
    temp.push_back(value);
    setTag(tag, temp);
}

void FixMessage::setTag(int tag, double value)
{
    setTag(tag, std::to_string(value));
}

void FixMessage::setTag(int tag, long value)
{
    setTag(tag, std::to_string(value));
}


void FixMessage::setSendingAndTransactionTimeAsCurrent(core::Subseconds precision)
{
    auto dateTime = core::getUtcDatetime(precision);
    setTag(FixConstants::FIX_TAG_SENDING_TIME, dateTime);
    setTag(FixConstants::FIX_TAG_TRANSACTION_TIME, dateTime);
}

const string& FixMessage::getTagValue(int tag) const
{
    return m_bodyTagValuePairs[tag];
}

char  FixMessage::getTagValueAsChar(int tag) const
{
    auto valueAsString = getTagValue(tag);
    return valueAsString[0];
}

int  FixMessage::getTagValueAsInt(int tag) const
{
    return std::stoi(getTagValue(tag));
}

long FixMessage::getTagValueAsLong(int tag) const
{
    return std::stol(getTagValue(tag));
}

double FixMessage::getTagValueAsDouble(int tag) const
{
    return std::stod(getTagValue(tag));
}

void FixMessage::loadFromFile(const string& input, vector<FixMessage>& messages)
{
    ifstream file(input); // ifstream dtor also closes the file so no need for using a smart ptr to close the file

    if (!file.good())
    {
        THROW_PRETTY_RUNTIME_EXCEPTION(core::format("File %s could not be opened", input))
    }

    file.seekg(0, std::ios::beg);
    string line;

    while (std::getline(file, line))
    {
        if (core::startsWith(line, '#') == false)
        {
            FixMessage message;
            FixParser::parseFromString(message, line);
            messages.emplace_back(message);
        }
    }

    file.close();
}

void FixMessage::toString(string& stringOutput)
{
    auto appendTag = [&stringOutput, this](int tag)
                            {
                                auto value = getTagValue(tag);
                                stringOutput += std::to_string(tag) + FixConstants::FIX_EQUALS + value + FixConstants::FIX_DELIMITER;
                            };

    auto appendTagStringValue = [&stringOutput, this](int tag, string value)
    {
        stringOutput += std::to_string(tag) + FixConstants::FIX_EQUALS + value + FixConstants::FIX_DELIMITER;
    };

    ////////////////////////////////////////////
    // HEADER TAGS

    // FIX VERSION
    appendTagStringValue(FixConstants::FIX_TAG_VERSION, FixConstants::FIX_VERSION_STRINGS[m_fixVersion]);

    // FIX BODY LENGTH
    appendTagStringValue(FixConstants::FIX_TAG_BODY_LENGTH, std::to_string(calculateBodyLength()));

    // FIX MESSAGE TYPE
    appendTagStringValue(FixConstants::FIX_TAG_MESSAGE_TYPE, FixConstants::FIX_MESSAGE_TYPE_STRINGS[m_messageType]);

    // FIX SEQUENCE NUMBER
    appendTagStringValue(FixConstants::FIX_TAG_SEQUENCE_NUMBER, std::to_string(m_sequenceNumber));

    // FIX SENDER COMPID
    appendTagStringValue(FixConstants::FIX_TAG_SENDER_COMPID, m_senderCompId);

    // FIX SENDING TIME
    appendTagStringValue(FixConstants::FIX_TAG_SENDING_TIME, m_sendingTime);

    // FIX TARGET COMPID
    appendTagStringValue(FixConstants::FIX_TAG_TARGET_COMPID, m_targetCompId);

    ////////////////////////////////////////////
    // BODY TAGS
    for (const auto& tagValue : m_bodyTagValuePairs)
    {
        auto tagValueKey = tagValue.first;
        appendTag(tagValueKey);
    }

    ////////////////////////////////////////////
    // CHECKSUM
    string checksum;
    FixMessage::calculateCheckSum(stringOutput, checksum);
    appendTagStringValue(FixConstants::FIX_TAG_BODY_CHECKSUM, checksum);
}

void FixMessage::calculateCheckSum(const string& message, string& checksum)
{
    int sum{ 0 };

    for(char c : message)
    {
        sum += (int)c;
    }

    sum = sum % 256;

    checksum = std::to_string(sum);
    core::padLeft(checksum, 3, '0');
}

}