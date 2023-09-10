#include <fstream>
#include <iostream>

#include "Fen.h"
#include "Test.h"

bool Test::perftDepth( int depth, const std::string& fen )
{
    if ( depth < 1 )
    {
        std::cout << "Invalid depth: " << depth << std::endl;
        return false;
    }
    else if ( fen.empty() )
    {
        std::cout << "Missing FEN string" << std::endl;
        return false;
    }

    std::cout << fen << std::endl;

    unsigned int actualResult = perftRun( depth, fen );
    std::cout << "  Depth: " << depth << ". Actual: " << actualResult << std::endl;

    return true;
}

bool Test::perftFen( const std::string& fenWithResults )
{
    if ( fenWithResults.empty() )
    {
        std::cout << "Missing FEN string" << std::endl;
        return false;
    }

    // This FEN string is expected to have expected results at the end
    // Support two formats: comma and semicolon

    size_t semicolon = fenWithResults.find_first_of( ';', 0 );
    size_t comma = fenWithResults.find_first_of( ',', 0 );
    if ( semicolon != SIZE_MAX )
    {
        std::string fen = fenWithResults.substr( 0, semicolon );
        std::string results = fenWithResults.substr( semicolon + 2 );

        int depth;
        unsigned int actualResult;

        std::cout << fen << std::endl;

        // Split by ";D" and extract depth and expected result
        size_t pos = 0;
        std::string delimiter( ";D" );
        std::string token;
        while ( ( pos = results.find( delimiter ) ) != std::string::npos )
        {
            token = results.substr( 0, pos );

            size_t split = token.find_first_of( ' ', 0 );
            if ( split != SIZE_MAX )
            {
                depth = atoi( token.substr( 0, split ).c_str() );
                actualResult = perftRun( depth, fen );
                report( depth, atoi( token.substr( split + 1 ).c_str() ), actualResult );
            }

            results.erase( 0, pos + delimiter.length() );
        }

        // Get the last one
        token = results;
        size_t split = token.find_first_of( ' ', 0 );
        if ( split != SIZE_MAX )
        {
            depth = atoi( token.substr( 0, split ).c_str() );
            actualResult = perftRun( depth, fen );
            report( depth, atoi( token.substr( split + 1 ).c_str() ), actualResult );
        }
    }
    else if ( comma != SIZE_MAX )
    {
        std::string fen = fenWithResults.substr( 0, comma );
        std::string results = fenWithResults.substr( comma + 1 );

        int depth = 1;
        unsigned int actualResult;

        std::cout << fen << std::endl;

        // Split by comma and infer the depth
        size_t pos = 0;
        std::string delimiter( "," );
        std::string token;
        while ( ( pos = results.find( delimiter ) ) != std::string::npos )
        {
            token = results.substr( 0, pos );

            actualResult = perftRun( depth, fen );
            report( depth, atoi( token.c_str() ), actualResult );

            results.erase( 0, pos + delimiter.length() );
            depth++;
        }

        // Get the last one
        token = results;
        actualResult = perftRun( depth, fen );
        report( depth, atoi( token.c_str() ), actualResult );
    }
    else
    {
        std::cout << "Missing expected results" << std::endl;
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
        std::cout << "File was not opened: " << filename << std::endl;
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
    // Prep here

    Board* board = Board::createBoard( fen );
    std::cout << board->toString() << std::endl;
    return 0;

    // Run the test

    clock_t start = clock();

    unsigned int nodes = perftLoop( depth, board, true );

    clock_t end = clock();

    // Tidy up and report

    float elapsed = static_cast<float>( end - start ) / CLOCKS_PER_SEC;
    float nps = elapsed == 0 ? 0 : static_cast<float>( nodes ) / elapsed;

    // This will give 0 if elapsed is close to zero - but not sure what to do with that other than continue
    long lnps = std::lround( nps );

    std::cout << "  Found " << nodes << " nodes in " << elapsed << "s (" << lnps << " nps)" << std::endl;

    return nodes;
}

unsigned int Test::perftLoop( int depth, Board* board, bool divide )
{
    unsigned int nodes = 0;

    if ( depth == 0 )
    {
        return 1;
    }

    //std::vector<Move> moves = board->getMoves();

    //for ( std::vector<Move>::iterator it = moves.begin(); it != moves.end(); it++ )
    //{
    //    Move& move = *it;
    //    Board tBoard = board->makeMove( move );

    //    if ( divide )
    //    {
    //        unsigned long moveNodes = perftLoop( depth - 1, tBoard );
    //        nodes += moveNodes;

    //        std::cout << "  " << move.toString() << " : " << moveNodes << " " << tBoard.toFENString() << std::endl;
    //    }
    //    else
    //    {
    //        nodes += perftLoop( depth - 1, tBoard );
    //    }
    //}

    return nodes;
}
void Test::report( int depth, unsigned int expected, unsigned int actual )
{
    if ( expected != actual )
    {
        std::cout << "  **ERROR**";
    }

    std::cout << "  Depth: " << depth << ". Expected: " << expected << ". Actual: " << actual << std::endl;
}
