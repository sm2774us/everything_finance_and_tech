#ifndef _FIX_SEQUENCE_STORE_
#define _FIX_SEQUENCE_STORE_

#include <string>
#include <core/file_utility.h>
#include <core/string_utility.h>

namespace fix
{

class FixSequenceStore
{
    public :

        static void  getSequenceStoreFileName(const std::string& senderCompId, const std::string& targetCompid, std::string& fileName)
        {
            fileName = senderCompId + "_" + targetCompid + "_sequence.txt";
        }

        static void loadFromSequenceStore(const std::string& senderCompId, const std::string& targetCompid, int& outgoingSequenceNumber, int& incomingSequenceNumber)
        {
            std::string fileName;

            getSequenceStoreFileName(senderCompId, targetCompid, fileName);

            if (core::doesFileExist(fileName) == false)
            {
                outgoingSequenceNumber = 1;
                incomingSequenceNumber = 0;
            }
            else
            {
                std::string fileContent;
                core::readAllFile(fileName, fileContent);
                auto numbers = core::split(fileContent, ',');
                outgoingSequenceNumber = std::stoi(numbers[0]);
                incomingSequenceNumber = std::stoi(numbers[1]);
            }
        }

        static void updateSequenceStore(const std::string& senderCompId, const std::string& targetCompid, int outgoingSequenceNumber, int incomingSequenceNumber)
        {
            std::string fileName;
            std::stringstream newContent;

            getSequenceStoreFileName(senderCompId, targetCompid, fileName);

            newContent << outgoingSequenceNumber << "," << incomingSequenceNumber;
            core::deleteFile(fileName);
            core::appendTextToFile(fileName, newContent.str(), true);
        }
};

}

#endif