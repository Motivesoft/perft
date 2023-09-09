#include <fstream>
#include <iostream>

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

bool Test::perftFen( const std::string& fen )
{
    if ( fen.empty() )
    {
        std::cerr << "Missing FEN string" << std::endl;
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
