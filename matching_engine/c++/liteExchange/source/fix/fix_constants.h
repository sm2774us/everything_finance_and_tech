#ifndef _FIX_CONSTANTS_
#define _FIX_CONSTANTS_

#include <string>
#include <core/string_utility.h>

namespace fix
{

namespace FixConstants
{

// Not using C++11 enum classes here in order to avoid a type cast during access to static const char* tables
// If it was strongly typed enums , then had to use std::underlying_type cast

// VERSIONS
enum FixVersion
{
    FIX_VERSION_NONE,
    FIX_4_0,
    FIX_4_1,
    FIX_4_2,
    FIX_4_3,
    FIX_4_4,
    FIX_5_0,
    FIX_5_0SP1,
    FIX_5_0SP2
};

static constexpr char* FIX_VERSION_STRINGS[] =
{
    "NONE",
    "FIX.4.0",
    "FIX.4.1",
    "FIX.4.2",
    "FIX.4.3",
    "FIX.4.4",
    "FIX.5.0",
    "FIX.5.0SP1",
    "FIX.5.0SP2"
};

// MESSAGE TYPES
enum MessageType
{
    MESSAGE_TYPE_NONE,
    LOGON,
    LOGOFF,
    TEST_REQUEST,
    RESEND_REQUEST,
    ADMIN_REJECT,
    SEQUENCE_RESET,
    HEARTBEAT,
    USER_LOGON,
    USER_RESPONSE,
    NEW_ORDER,
    CANCEL,
    EXECUTION_REPORT,
    BUSINESS_REJECT
};

static constexpr char* FIX_MESSAGE_TYPE_STRINGS[] =
{
    "NONE",
    "A",
    "5",
    "1",
    "2",
    "3",
    "4",
    "0",
    "BE",
    "BF",
    "D",
    "F",
    "8",
    "j"
};

// GENERAL
static constexpr char FIX_EQUALS = '=';
static constexpr char FIX_DELIMITER = ((char)1);
// TAGS
static constexpr int FIX_TAG_AVERAGE_PRICE = 6;
static constexpr int FIX_TAG_VERSION = 8;
static constexpr int FIX_TAG_BODY_LENGTH = 9;
static constexpr int FIX_TAG_BODY_CHECKSUM = 10;
static constexpr int FIX_TAG_CLIENT_ORDER_ID = 11;
static constexpr int FIX_TAG_CUMULATIVE_QUANTITY = 14;
static constexpr int FIX_TAG_EXEC_ID = 17;
static constexpr int FIX_TAG_EXEC_INST = 18;
static constexpr int FIX_TAG_EXEC_TRANSTYPE = 20;
static constexpr int FIX_TAG_HAND_INST = 21;
static constexpr int FIX_TAG_LAST_PRICE = 31;
static constexpr int FIX_TAG_LAST_QUANTITY = 32;
static constexpr int FIX_TAG_SEQUENCE_NUMBER = 34;
static constexpr int FIX_TAG_MESSAGE_TYPE = 35;
static constexpr int FIX_TAG_ORDER_ID = 37;
static constexpr int FIX_TAG_ORDER_QUANTITY = 38;
static constexpr int FIX_TAG_ORDER_STATUS = 39;
static constexpr int FIX_TAG_ORDER_TYPE = 40;
static constexpr int FIX_TAG_ORIG_CLIENT_ORDER_ID = 41;
static constexpr int FIX_TAG_ORDER_PRICE = 44;
static constexpr int FIX_TAG_REF_SEQ_NUM = 45;
static constexpr int FIX_TAG_SECURITY_ID = 48;
static constexpr int FIX_TAG_SENDER_COMPID = 49;
static constexpr int FIX_TAG_SENDER_SUBID = 50;
static constexpr int FIX_TAG_SENDING_TIME = 52;
static constexpr int FIX_TAG_ORDER_SIDE = 54;
static constexpr int FIX_TAG_SYMBOL = 55;
static constexpr int FIX_TAG_TARGET_COMPID = 56;
static constexpr int FIX_TAG_TARGET_SUBID = 57;
static constexpr int FIX_TAG_FREE_TEXT = 58;
static constexpr int FIX_TAG_TIME_IN_FORCE = 59;
static constexpr int FIX_TAG_TRANSACTION_TIME = 60;
static constexpr int FIX_TAG_ENCRYPT_METHOD = 98;
static constexpr int FIX_TAG_HEARBEAT_INTERVAL = 108;
static constexpr int FIX_TAG_TEST_REQUEST_ID = 112;
static constexpr int FIX_TAG_EXEC_TYPE = 150;
static constexpr int FIX_TAG_LEAVES_QTY = 151;
static constexpr int FIX_TAG_USERNAME = 553;
static constexpr int FIX_TAG_PASSWORD = 554;
static constexpr int FIX_TAG_REF_TAG = 371;
static constexpr int FIX_TAG_REF_MSG_TYPE = 372;
static constexpr int FIX_TAG_REJECT_REASON = 373;
static constexpr int FIX_TAG_USER_REQUEST_ID = 923;
static constexpr int FIX_TAG_USER_PASSWORD = 924;
// ORDER STATUS
static constexpr int FIX_ORDER_STATUS_NEW = 0;
static constexpr int FIX_ORDER_STATUS_PARTIALLY_FILLED = 1;
static constexpr int FIX_ORDER_STATUS_FILLED = 2;
static constexpr int FIX_ORDER_STATUS_DONE_FOR_DAY = 3;
static constexpr int FIX_ORDER_STATUS_CANCELED = 4;
static constexpr int FIX_ORDER_STATUS_REPLACED = 5;
static constexpr int FIX_ORDER_STATUS_PENDING_CANCEL = 6;
static constexpr int FIX_ORDER_STATUS_STOPPED = 7;
static constexpr int FIX_ORDER_STATUS_REJECTED = 8;
// ORDER TYPE
static constexpr int FIX_ORDER_TYPE_MARKET = 1;
static constexpr int FIX_ORDER_TYPE_LIMIT = 2;
// SIDE
static constexpr int FIX_ORDER_SIDE_BUY = 1;
static constexpr int FIX_ORDER_SIDE_SELL = 2;
// TIME IN FORCE
static constexpr int FIX_ORDER_TIF_DAY = 0;
// ENCRYPTION METHODS
static constexpr int FIX_ENCRYPTION_NONE = 0;

inline int getFixVersionFromString(const std::string& version)
{
    int ret{ -1 };

    if (core::compare(version, "FIX.4.0"))
    {
        ret = FixConstants::FixVersion::FIX_4_0;
    }
    else if (core::compare(version, "FIX.4.1"))
    {
        ret = FixConstants::FixVersion::FIX_4_1;
    }
    else if (core::compare(version, "FIX.4.2"))
    {
        ret = FixConstants::FixVersion::FIX_4_2;
    }
    else if (core::compare(version, "FIX.4.3"))
    {
        ret = FixConstants::FixVersion::FIX_4_3;
    }
    else if (core::compare(version, "FIX.4.4"))
    {
        ret = FixConstants::FixVersion::FIX_4_4;
    }
    else if (core::compare(version, "FIX.5.0"))
    {
        ret = FixConstants::FixVersion::FIX_5_0;
    }
    else if (core::compare(version, "FIX.5.0SP1"))
    {
        ret = FixConstants::FixVersion::FIX_5_0SP1;
    }
    else if (core::compare(version, "FIX.5.0SP2"))
    {
        ret = FixConstants::FixVersion::FIX_5_0SP2;
    }
    else
    {
        ret = FixVersion::FIX_VERSION_NONE;
    }

    return ret;
}

inline int getMessageTypeFromString(const std::string& messageType)
{
    int ret{ -1 };

    if (core::compare(messageType, "A"))
    {
        ret = MessageType::LOGON;
    }
    else if (core::compare(messageType, "0"))
    {
        ret = MessageType::HEARTBEAT;
    }
    else if (core::compare(messageType, "1"))
    {
        ret = MessageType::TEST_REQUEST;
    }
    else if (core::compare(messageType, "2"))
    {
        ret = MessageType::RESEND_REQUEST;
    }
    else if (core::compare(messageType, "4"))
    {
        ret = MessageType::SEQUENCE_RESET;
    }
    else if (core::compare(messageType, "5"))
    {
        ret = MessageType::LOGOFF;
    }
    else if (core::compare(messageType, "BE"))
    {
        ret = MessageType::USER_LOGON;
    }
    else if (core::compare(messageType, "BF"))
    {
        ret = MessageType::USER_RESPONSE;
    }
    else if (core::compare(messageType, "D"))
    {
        ret = MessageType::NEW_ORDER;
    }
    else if (core::compare(messageType, "F"))
    {
        ret = MessageType::CANCEL;
    }
    else if (core::compare(messageType, "8"))
    {
        ret = MessageType::EXECUTION_REPORT;
    }
    else if (core::compare(messageType, "3"))
    {
        ret = MessageType::ADMIN_REJECT;
    }
    else if (core::compare(messageType, "j"))
    {
        ret = MessageType::BUSINESS_REJECT;
    }
    else
    {
        ret = MessageType::MESSAGE_TYPE_NONE;
    }

    return ret;
}

};

}

#endif