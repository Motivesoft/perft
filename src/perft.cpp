// perft.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>

#include "BitBoard.h"
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
        std::cout << "No version info available" << std::endl;
    }

#if _DEBUG
    dumpCommandLine( argc, argv );
#endif

    bool commandLineOK = false;

    if ( argc > 1 )
    {
        commandLineOK = processCommandLine( argc, argv );
    }

    if( !commandLineOK )
    {
        std::cout << std::endl;
        std::cout << "Usage:" << std::endl;
        std::cout << "  perft [depth]         - perform a search using a depth and the standard start position" << std::endl;
        std::cout << "  perft [depth] [fen]   - perform a search using a depth and FEN string" << std::endl;
        std::cout << "  perft fen [fen]       - perform a search using a FEN string with expected results" << std::endl;
        std::cout << "  perft file [filename] - perform searches read from a file as FEN strings with expected results" << std::endl;
        std::cout << "  perft help            - this information" << std::endl;
    }
}

bool processCommandLine( int argc, const char** argv )
{
    BitBoard::initialize();

    // Work out what we are doing
    bool executed = false;

    std::string arg = argv[ 1 ];

    // If the first arg is a depth (all digits, not a FEN string), then process it accordingly
    bool isDepth = true;
    for ( std::string::const_iterator it = arg.cbegin(); it != arg.cend(); it++ )
    {
        if ( !isdigit( *it ) )
        {
            isDepth = false;
        }
    }

    if ( isDepth )
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
