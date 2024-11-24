#pragma once


std::string WStringToString(const std::wstring& wideStr) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0)
        throw std::runtime_error("WideCharToMultiByte failed.");

    std::string str(bufferSize - 1, '\0'); // -1 because bufferSize includes null terminator
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, str.data(), bufferSize, nullptr, nullptr);
    return str;
}
