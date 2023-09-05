#ifndef _FIX_MESSAGE_
#define _FIX_MESSAGE_

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <string>
#include <core/datetime_utility.h>

namespace fix
{

class FixMessage
{
    public:
        FixMessage();
        bool isAdminMessage() const;

        int calculateHeaderLength() const;
        int calculateBodyLength() const;

        void reset();
        void initialiseHeader();

        const std::string& getTagValue(int tag) const;
        char getTagValueAsChar(int tag) const;
        int getTagValueAsInt(int tag) const;
        long getTagValueAsLong(int tag) const;
        double getTagValueAsDouble(int tag) const;

        void removeTag(int tag);

        bool hasTag(int tag) const;
        void setTag(int tag, const std::string& value);
        void setTag(int tag, int value);
        void setTag(int tag, char value);
        void setTag(int tag, double value);
        void setTag(int tag, long value);

        void setSendingAndTransactionTimeAsCurrent(core::Subseconds precision);

        int getFixVersion() const { return m_fixVersion;  }
        int getMessageType() const;
        std::size_t getSequenceNumber() const{ return m_sequenceNumber;  }
        const std::string& getSenderCompId() const { return m_senderCompId; }
        const std::string& getTargetCompId() const { return m_targetCompId; }
        const std::string& getSendingTime() const { return m_sendingTime; }

        void setFixVersion(int version) { m_fixVersion = version; }
        void setMessageType(int messageType) { m_messageType = messageType; }
        void setSequenceNumber(std::size_t sequenceNumber) { m_sequenceNumber = sequenceNumber; }
        void setSenderCompId(const std::string& senderCompId) { m_senderCompId = senderCompId; }
        void setTargetCompId(const std::string& targetCompId) { m_targetCompId = targetCompId; }
        void setSendingTime(const std::string& sendingTime) { m_sendingTime = sendingTime; }

        void toString(std::string& stringOutput);

        static void loadFromFile(const std::string& input, std::vector<FixMessage>& messages);
        static void calculateCheckSum(const std::string& message, std::string& checksum);

    private:
        int m_fixVersion;
        std::size_t m_sequenceNumber;
        int m_messageType;
        std::string m_senderCompId;
        std::string m_targetCompId;
        std::string m_sendingTime;
        mutable std::unordered_map<int, std::string> m_bodyTagValuePairs;
};

}

#endif