#pragma once
#include <string>
#include <tuple>

std::string readTextFromFile(const std::string& fileName, const std::string& beginToken, const std::string& endToken);
std::tuple <float*, unsigned int> parseFloatArrayFromString(const std::string& fileName, const std::string& arrayName);