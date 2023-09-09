// perft.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "VersionInfo.h"

int main()
{
    std::unique_ptr<VersionInfo> versionInfo = VersionInfo::getVersionInfo();

    if ( versionInfo->isAvailable() )
    {
        std::cout << "Hello "<< versionInfo->getCompanyName()<<"!" << std::endl;
        std::cout << "Product " << versionInfo->getProductName() << " " << versionInfo->getProductVersion() << "!" << std::endl;
    }
    else
    {
        std::cerr << "No version info available" << std::endl;
    }
}
