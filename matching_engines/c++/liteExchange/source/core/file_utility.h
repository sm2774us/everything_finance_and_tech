#ifndef _FILE_UTILITY_H_
#define _FILE_UTILITY_H_

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

#ifdef __linux__
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#elif _WIN32
#include <windows.h>
#endif

namespace core
{

inline std::string getDirectoryNameFromPath(const std::string& fullPath)
{
    std::string directoryName;
    std::vector<char> path( fullPath.begin(), fullPath.end());

    char* pathPtr = &path[0];

    // We are finding last occurence of // in Linux and \\ in Windows
#ifdef __linux__
    pathPtr = strrchr(pathPtr, '/');
#elif _WIN32
    pathPtr = strrchr(pathPtr, '\\');
#endif

    if(pathPtr)
    {
        pathPtr[0] = '\0';
    }

    directoryName = &path[0];
    return directoryName;
}

inline void readAllFile(const std::string& fileName, std::string& content)
{
    std::ifstream inFile;
    inFile.open(fileName);

    std::stringstream stream;
    stream << inFile.rdbuf();

    content = stream.str();
    inFile.close();
}

inline bool appendTextToFile(const std::string& fileName, const std::string& text, bool createIfNecessary = true)
{
    std::ofstream outfile;
    auto mode = createIfNecessary ? std::ios_base::app : std::ios_base::out;
    outfile.open(fileName, mode);

    if (outfile.is_open() == false)
    {
        return false;
    }

    outfile << text;
    outfile.close();
    return true;
}

inline bool doesFileExist(const std::string& fileName)
{
    std::ifstream f(fileName.c_str());
    bool retval = false;

    if (f.good())
    {
        retval = true;
    }

    f.close();
    return retval;
}

inline bool deleteFile(const std::string& fileName)
{
    bool success = true;
#ifdef __linux__
    if ( unlink(fileName.c_str()) != 0 )
    {
        success = false;
    }
#elif _WIN32
    if (DeleteFile(fileName.c_str()) == 0)
    {
        success = false;
    }
#endif

    return success;
}

inline bool backupDirectory(const std::string& existingName, const std::string& newName, const std::string& targetDirectory)
{
    bool success = true;
#ifdef __linux__
    auto actualNewName = targetDirectory + "//" + newName;
    // The input should have forward slashes , not Windows specific back slashes
    if( rename(existingName.c_str(), actualNewName.c_str()) != 0)
    {
        return false;
    }
#elif _WIN32
    auto actualNewName = targetDirectory + "\\" + newName;
    if (MoveFile(existingName.c_str(), actualNewName.c_str()) == 0)
    {
        success = false;
    }
#endif
    return success;
}

inline bool createDirectory(const std::string& dirName)
{
    bool success = true;
#ifdef __linux__
    if( mkdir(dirName.c_str(), 777) != 0)
    {
        success = false;
    }
#elif _WIN32
    if (CreateDirectory(dirName.c_str(), nullptr) == 0)
    {
        success = false;
    }
#endif

    return success;
}

}// namespace

#endif