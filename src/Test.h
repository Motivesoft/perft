#pragma once

#include <string>

#include "Board.h"

class Test
{
private:
    static unsigned int perftRun( int depth, const std::string& fen, bool divide = true );
    static unsigned int perftLoop( int depth, Board* board, bool divide = false );

    static void report( int depth, unsigned int expected, unsigned int actual );

public:
    /// <summary>
    /// Do a depth search with the provided FEN string and report the results
    /// </summary>
    /// <param name="depth">the search depth</param>
    /// <param name="fen">the FEN string</param>
    /// <returns></returns>
    static bool perftDepth( int depth, const std::string& fen );

    /// <summary>
    /// Read a FEN string and the expected results and perform a search to check for matching results
    /// </summary>
    /// <param name="fen">the FEN string, with expected results</param>
    /// <returns></returns>
    static bool perftFen( const std::string& fenWithResults );

    /// <summary>
    /// Read a file of FEN strings and pass them to <code>perftFen</code>
    /// </summary>
    /// <param name="filename">the file to read</param>
    /// <returns><code>false</code> if the file fails to open</returns>
    static bool perftFile( const std::string& filename );
};
