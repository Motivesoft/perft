#pragma once

#include <memory>
#include <string>

class VersionInfo
{
private:
    bool available;
    std::string companyName;
    std::string productName;
    std::string productVersion;

    void populate( const char* module );

public:
    static std::unique_ptr<VersionInfo> getVersionInfo();

    VersionInfo() :
        available( false )
    {

    }

    bool isAvailable() const
    {
        return available;
    }

    const char* getCompanyName() const
    {
        return companyName.c_str();
    }

    const char* getProductName() const
    {
        return productName.c_str();
    }

    const char* getProductVersion() const
    {
        return productVersion.c_str();
    }
};

