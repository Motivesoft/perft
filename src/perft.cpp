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
    //dumpCommandLine( argc, argv );
#endif

    bool commandLineOK = false;

    if ( argc > 1 )
    {
        BitBoard::initialize();

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
    std::vector<std::string> args;
    bool divide = false;

    for ( size_t loop = 1; loop < argc; loop++ )
    {
        std::string arg = argv[ loop ];
        if ( arg == "-divide" )
        {
            divide = true;
        }
        else
        {
            args.push_back( arg );
        }
    }

    // Work out what we are doing
    bool executed = false;

    std::string arg = args[ 0 ];

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

        if ( args.size() == 1 )
        {
            executed = Test::perftDepth( depth, Fen::startingPosition, divide );
        }
        else
        {
            std::stringstream fen;

            for ( int loop = 1; loop < args.size(); loop++ )
            {
                if ( loop > 1 )
                {
                    fen << " ";
                }

                fen << args[ loop ];
            }

            executed = Test::perftDepth( depth, fen.str().c_str(), divide );
        }
    }
    else if ( arg == "fen" )
    {
        std::stringstream fen;

        for ( int loop = 1; loop < args.size(); loop++ )
        {
            if ( loop > 1 )
            {
                fen << " ";
            }

            fen << args[ loop ];
        }

        executed = Test::perftFen( fen.str().c_str(), divide );
    }
    else if ( arg == "file" )
    {
        if ( arg.size() > 1 )
        {
            std::string filename = args[ 2 ];

            executed = Test::perftFile( filename.c_str(), divide );
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
