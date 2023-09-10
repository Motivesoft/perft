#include "Board.h"

#include <iostream>
#include <sstream>

Board* Board::createBoard( const std::string& fen )
{
    // Content of the FEN string
    std::string pieces;
    std::string color;
    std::string castling;
    std::string enPassant;
    std::string halfMoveClock = "0";
    std::string fullMoveNumber = "0";

    // Working variable
    std::string rest;

    size_t index = fen.find_first_of( ' ' );

    pieces = fen.substr( 0, index );
    rest = fen.substr( index + 1 );

    index = rest.find_first_of( ' ' );
    color = rest.substr( 0, index );
    rest = rest.substr( index + 1 );

    index = rest.find_first_of( ' ' );
    castling = rest.substr( 0, index );
    rest = rest.substr( index + 1 );

    index = rest.find_first_of( ' ' );
    enPassant = rest.substr( 0, index );
    rest = rest.substr( index + 1 );

    // Treat these last two values as potentiallly missing, even though they should actually be there
    if ( rest.length() > 0 )
    {
        index = rest.find_first_of( ' ' );
        halfMoveClock = rest.substr( 0, index );
        rest = rest.substr( index + 1 );

        if ( rest.length() > 0 )
        {
            index = rest.find_first_of( ' ' );
            fullMoveNumber = rest.substr( 0, index );
            rest = rest.substr( index + 1 );
        }
    }

    std::cerr << pieces << std::endl;
    std::cerr << color << std::endl;
    std::cerr << castling << std::endl;
    std::cerr << enPassant << std::endl;
    std::cerr << halfMoveClock << std::endl;
    std::cerr << fullMoveNumber << std::endl;

    size_t castlingIndex = 0;
    bool castlingRights[] = {false, false, false, false};

    if ( castling[ castlingIndex ] == 'K' )
    {
        castlingRights[ 0 ] = true;
        castlingIndex++;
    }
    if ( castling[ castlingIndex ] == 'Q' )
    {
        castlingRights[ 1 ] = true;
        castlingIndex++;
    }
    if ( castling[ castlingIndex ] == 'k' )
    {
        castlingRights[ 2 ] = true;
        castlingIndex++;
    }
    if ( castling[ castlingIndex ] == 'q' )
    {
        castlingRights[ 3 ] = true;
        castlingIndex++;
    }

    unsigned long long ep = 0;
    if ( enPassant != "-" )
    {
        // Make this a bitboard thing - the bit at (eg) e3 or 0 for no EP
        ep = 1ull << ( ( ( enPassant[ 1 ] - '1' ) << 3 ) | ( enPassant[ 0 ] - 'a' ) );
    }

    return new Board( color == "w",
                      castlingRights,
                      ep,
                      atoi( halfMoveClock.c_str() ),
                      atoi( fullMoveNumber.c_str() ) );
}

std::string Board::toString() const
{
    std::stringstream fen;

    // Pieces
    fen << " ";

    // Color
    fen << ( whiteToMove ? "w" : "b" ) << " ";

    // Castling Rights
    if ( castlingRights[ 0 ] )
    {
        fen << "K";
    }
    if ( castlingRights[ 1 ] )
    {
        fen << "Q";
    }
    if ( castlingRights[ 2 ] )
    {
        fen << "k";
    }
    if ( castlingRights[ 3 ] )
    {
        fen << "q";
    }
    fen << " ";

    // En-Passant
    unsigned long index;
    if ( _BitScanForward64( &index, enPassantIndex ) )
    {
        fen << (char) ( ( index & 7 ) + 'a' ) << (char) ( ( ( index >> 3 ) & 7 ) + '1' );
    }
    else
    {
        fen << "-";
    }
    fen << " ";

    // Half Move Clock
    fen << halfMoveClock << " ";

    // Full Move Number
    fen << fullMoveNumber << " ";

    return fen.str();
}