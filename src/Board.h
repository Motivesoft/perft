#pragma once

#include <string>
#include <vector>

class Board
{
private:
    bool whiteToMove;
    bool castlingRights[ 4 ];
    unsigned short enPassantIndex;
    unsigned short halfMoveClock;
    unsigned short fullMoveNumber;

    Board( bool whiteToMove,
           bool castlingRights[ 4 ],
           unsigned short enPassantIndex,
           unsigned short halfMoveClock,
           unsigned short fullMoveNumber ) :
        whiteToMove( whiteToMove ),
        enPassantIndex( enPassantIndex ),
        halfMoveClock( halfMoveClock ),
        fullMoveNumber( fullMoveNumber )
    {
        for ( size_t loop = 0; loop < 4; loop++ )
        {
            this->castlingRights[ loop ] = castlingRights[ loop ];
        }
    }

public:
    static Board* createBoard( const std::string& fen );

    // State* makeMove( const Move& move )
    // undoMove( state )
    // Where state is undo/redo set of settings - identical to Board, to the point where Board might just use it natively?
};

