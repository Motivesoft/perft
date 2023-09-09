#include <fstream>
#include <iostream>

#include "Fen.h"
#include "Test.h"

bool Test::perftDepth( int depth, const std::string& fen )
{
    if ( depth < 1 )
    {
        std::cerr << "Invalid depth: " << depth << std::endl;
        return false;
    }
    else if ( fen.empty() )
    {
        std::cerr << "Missing FEN string" << std::endl;
        return false;
    }

    return true;
}

bool Test::perftFen( const std::string& fenWithResults )
{
    if ( fenWithResults.empty() )
    {
        std::cerr << "Missing FEN string" << std::endl;
        return false;
    }

    // This FEN string is expected to have expected results at the end
    // Support two formats: comma and semicolon

    size_t semicolon = fenWithResults.find_first_of( ';', 0 );
    size_t comma = fenWithResults.find_first_of( ',', 0 );
    if ( semicolon != SIZE_MAX )
    {
        std::string fen = fenWithResults.substr( 0, semicolon );
        std::string results = fenWithResults.substr( semicolon + 1 );


    }
    else if ( comma != SIZE_MAX )
    {
        std::string fen = fenWithResults.substr( 0, comma );
        std::string results = fenWithResults.substr( comma + 1 );

        int depth = 1;

        size_t pos = 0;
        std::string delimiter( "," );
        std::string token;
        while ( ( pos = results.find( delimiter ) ) != std::string::npos )
        {
            token = results.substr( 0, pos );
            std::cout << token << std::endl;
            results.erase( 0, pos + delimiter.length() );
        }
        std::cout << results << std::endl;
    }
    else
    {
        std::cerr << "Missing expected results" << std::endl;
        return false;
    }

    return true;
}

bool Test::perftFile( const std::string& filename )
{
    std::fstream file;
    file.open( filename, std::ios::in );

    if ( !file.is_open() )
    {
        std::cerr << "File was not opened: " << filename << std::endl;
        return false;
    }

    std::string line;
    while ( std::getline( file, line ) )
    {
        if ( line.empty() || line[ 0 ] == '#' )
        {
            // Skipping a formatting/comment line
            continue;
        }

        // This just happens to do the processing we want, although we are not providing a depth this way
        perftFen( line );
    }

    return true;
}

unsigned int Test::perftRun( int depth, const std::string& fen, bool divide )
{
    return 0;
}
