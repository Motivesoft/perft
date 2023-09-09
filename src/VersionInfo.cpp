#include "VersionInfo.h"

#include <windows.h>
#include <winver.h>

std::unique_ptr<VersionInfo> VersionInfo::getVersionInfo()
{
    std::unique_ptr<VersionInfo> versionInfo = std::make_unique<VersionInfo>();

    char buffer[ MAX_PATH ];
    ::GetModuleFileNameA( nullptr, buffer, MAX_PATH );

    versionInfo->populate( buffer );

    return versionInfo;
}

void VersionInfo::populate( const char* module )
{
    DWORD sizeHandle;
    DWORD size = ::GetFileVersionInfoSizeA( module, &sizeHandle );

    available = size > 0;

    if ( available )
    {
        char* buffer = new char[ size ];

        available = ::GetFileVersionInfoA( module, 0, size, buffer );

        if ( available )
        {
            LPVOID valuePointer;
            UINT valueLength;

            if ( ::VerQueryValueA( buffer, "\\StringFileInfo\\080904b0\\CompanyName", &valuePointer, &valueLength ) )
            {
                companyName = std::string( (char*) valuePointer );
            }

            if ( ::VerQueryValueA( buffer, "\\StringFileInfo\\080904b0\\ProductName", &valuePointer, &valueLength ) )
            {
                productName = std::string( (char*) valuePointer );
            }

            if ( ::VerQueryValueA( buffer, "\\StringFileInfo\\080904b0\\ProductVersion", &valuePointer, &valueLength ) )
            {
                productVersion = std::string( (char*) valuePointer );
            }
        }

        delete[] buffer;
    }
}

