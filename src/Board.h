#pragma once

#include <string>
#include <vector>

#include "Move.h"

class Board
{
private:
    static const unsigned short PAWN;
    static const unsigned short KNIGHT;
    static const unsigned short BISHOP;
    static const unsigned short ROOK;
    static const unsigned short QUEEN;
    static const unsigned short KING;

    unsigned long long bitboards[ 12 ];
    unsigned long long whitePieces;
    unsigned long long blackPieces;
    unsigned long long allPieces;
    unsigned long long emptySquares;

    bool whiteToMove;

    bool castlingRights[ 4 ];

    unsigned long long enPassantIndex;

    unsigned short halfMoveClock;
    unsigned short fullMoveNumber;

    Board( unsigned long long bitboards[],
           bool whiteToMove,
           bool castlingRights[ 4 ],
           unsigned long long enPassantIndex,
           unsigned short halfMoveClock,
           unsigned short fullMoveNumber ) :
        whiteToMove( whiteToMove ),
        enPassantIndex( enPassantIndex ),
        halfMoveClock( halfMoveClock ),
        fullMoveNumber( fullMoveNumber )
    {
        allPieces = whitePieces = blackPieces = emptySquares = 0;

        for ( size_t loop = 0; loop < 12; loop++ )
        {
            this->bitboards[ loop ] = bitboards[ loop ];

            if ( loop < 6 )
            {
                whitePieces |= bitboards[ loop ];
            }
            else
            {
                blackPieces |= bitboards[ loop ];
            }
        }

        allPieces = whitePieces | blackPieces;
        emptySquares = ~allPieces;

        for ( size_t loop = 0; loop < 4; loop++ )
        {
            this->castlingRights[ loop ] = castlingRights[ loop ];
        }
    }

    // Instance methods
    bool isEmpty( unsigned long long bit ) const;
    size_t bitboardArrayIndexFromBit( unsigned long long bit ) const;

    // Static methods
    static const char pieceFromBitboardArrayIndex( size_t arrayIndex );
    static size_t bitboardArrayIndexFromPiece( const char piece );

    void getPawnMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces );
    void getKnightMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares );
    void getBishopMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares );
    void getRookMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares );
    void getQueenMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares );
    void getKingMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares );

public:
    static Board* createBoard( const std::string& fen );

    std::string toString() const;

    void getMoves( std::vector<Move>& moves );

    class State
    {
    private:
        unsigned long long bitboards[ 12 ];

        bool whiteToMove;

        bool castlingRights[ 4 ];

        unsigned long long enPassantIndex;

        unsigned short halfMoveClock;
        unsigned short fullMoveNumber;

    public:
        State( const Board& board );

        void apply( Board& board ) const;
    };

    Board::State makeMove( const Move& move );
    void unmakeMove( const Board::State& state );

    // State* makeMove( const Move& move )
    // undoMove( state )
    // Where state is undo/redo set of settings - identical to Board, to the point where Board might just use it natively?
};

