#include "Utilities.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <tuple>

std::string readTextFromFile(const std::string& fileName, const std::string& beginToken, const std::string& endToken)
{
    std::string line;
    std::string resultString;
    bool startCopying = false;
    std::ifstream inputFileStream(fileName);
    if (inputFileStream.is_open())
    {
        while (std::getline(inputFileStream, line))
        {
            if (line.find(beginToken) != std::string::npos)
                startCopying = true;
            else if (line.find(endToken) != std::string::npos)
                startCopying = false;
            else if (startCopying == true)
                resultString += line + "\n";
        }
    }
    inputFileStream.close();
    return resultString;
}

std::tuple <float*, unsigned int> parseFloatArrayFromString(const std::string& fileName, const std::string& arrayName)
{
    std::string dataAsString = readTextFromFile(fileName, arrayName + " begin", arrayName + " end");
    unsigned int count = std::count(dataAsString.begin(), dataAsString.end(), '.');
    float* array = new float[count];
    std::stringstream stringStream(dataAsString);
    for (unsigned int i = 0; i < count; i++)
        stringStream >> array[i];
    return {array, count*sizeof(float)};
}

