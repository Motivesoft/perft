// perft.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>

#include "Fen.h"
#include "Test.h"
#include "VersionInfo.h"

void dumpCommandLine( int argc, const char** argv );
bool processCommandLine( int argc, const char** argv );

int main( int argc, const char** argv )
{
    std::unique_ptr<VersionInfo> versionInfo = VersionInfo::getVersionInfo();

    if ( versionInfo->isAvailable() )
    {
        std::cout << versionInfo->getCompanyName() << " " << versionInfo->getProductName() << " version " << versionInfo->getProductVersion() << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cerr << "No version info available" << std::endl;
    }

    //dumpCommandLine( argc, argv );

    bool commandLineOK = false;

    if ( argc > 1 )
    {
        commandLineOK = processCommandLine( argc, argv );
    }

    if( !commandLineOK )
    {
        std::cerr << std::endl;
        std::cerr << "Usage:" << std::endl;
        std::cerr << "  perft [depth]         - perform a search using a depth and the standard start position" << std::endl;
        std::cerr << "  perft [depth] [fen]   - perform a search using a depth and FEN string" << std::endl;
        std::cerr << "  perft fen [fen]       - perform a search using a FEN string with expected results" << std::endl;
        std::cerr << "  perft file [filename] - perform searches read from a file as FEN strings with expected results" << std::endl;
        std::cerr << "  perft help            - this information" << std::endl;
    }
}

bool processCommandLine( int argc, const char** argv )
{
    // Work out what we are doing
    bool executed = false;

    // Be careful when looking for arguments
    if ( argc > 1 )
    {
        std::string arg = argv[ 1 ];

        if ( isdigit( arg[ 1 ] ) )
        {
            // [depth] or [depth] [fen]

            int depth = atoi( arg.c_str() );

            if ( argc < 3 )
            {
                executed = Test::perftDepth( depth, Fen::startingPosition );
            }
            else
            {
                std::stringstream fen;

                for ( int loop = 2; loop < argc; loop++ )
                {
                    if ( loop > 2 )
                    {
                        fen << " ";
                    }

                    fen << argv[ loop ];
                }

                executed = Test::perftDepth( depth, fen.str().c_str() );
            }
        }
        else if ( arg == "fen" )
        {
            std::stringstream fen;

            for ( int loop = 2; loop < argc; loop++ )
            {
                if ( loop > 2 )
                {
                    fen << " ";
                }

                fen << argv[ loop ];
            }

            executed = Test::perftFen( fen.str().c_str() );
        }
        else if ( arg == "file" )
        {
            if ( argc > 2 )
            {
                std::string filename = argv[ 2 ];

                executed = Test::perftFile( filename.c_str() );
            }
        }
    }

    return executed;
}

void dumpCommandLine( int argc, const char** argv )
{
    std::cerr << "Command line argument count: " << argc << std::endl;
    for ( int loop = 0; loop < argc; loop++ )
    {
        std::cerr << "  " << argv[ loop ] << std::endl;
    }
}
