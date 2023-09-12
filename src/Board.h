#pragma once

#include <array>
#include <string>
#include <vector>

#include "Move.h"

class Board
{
private:
    static const unsigned short EMPTY;
    static const unsigned short WHITE;
    static const unsigned short BLACK;

    static const unsigned short PAWN;
    static const unsigned short KNIGHT;
    static const unsigned short BISHOP;
    static const unsigned short ROOK;
    static const unsigned short QUEEN;
    static const unsigned short KING;

    // Holds references to bitboards - e.g. 0 (a1) will point to the white rook bitboard with the start position
    std::array<unsigned short, 64> boardLUT;

    // Lucky 13 - empty, 6 white pieces, 6 black pieces
    std::array<unsigned long long, 13> bitboards;

    unsigned long long whitePieces;
    unsigned long long blackPieces;
    unsigned long long allPieces;

    bool whiteToMove;

    std::array<bool, 4> castlingRights;

    unsigned long long enPassantIndex;

    unsigned short halfMoveClock;
    unsigned short fullMoveNumber;

    Board( std::array<unsigned long long, 13> bitboards,
           bool whiteToMove,
           std::array<bool, 4> castlingRights,
           unsigned long long enPassantIndex,
           unsigned short halfMoveClock,
           unsigned short fullMoveNumber ) :
        bitboards( bitboards ),
        whiteToMove( whiteToMove ),
        castlingRights( castlingRights ),
        enPassantIndex( enPassantIndex ),
        halfMoveClock( halfMoveClock ),
        fullMoveNumber( fullMoveNumber )
    {
        whitePieces = blackPieces = 0;

        // Build a lookup table that goes from board index (0-63) to bitboardArrayIndex (0-11 or USHRT_MAX)
        for ( unsigned short loop = 0; loop < 64; loop++ )
        {
            boardLUT[ loop ] = 0;
        }

        for ( size_t loop = WHITE; loop < bitboards.size(); loop++ )
        {
            if ( loop < BLACK )
            {
                whitePieces |= bitboards[ loop ];
            }
            else
            {
                blackPieces |= bitboards[ loop ];
            }

            unsigned long index;
            unsigned long long bitboard = bitboards[ loop ];
            while ( _BitScanForward64( &index, bitboard ) )
            {
                bitboard ^= 1ull << index;
                boardLUT[ index ] = static_cast<unsigned short>( loop );
            }
        }

        allPieces = whitePieces | blackPieces;
    }

    // Instance methods
    inline bool isEmpty( unsigned long long bit ) const;

    inline unsigned long long emptySquares() const
    {
        return bitboards[ EMPTY ];
    }

    /// <summary>
    /// Find which bitboard array has bit set and return its index
    /// </summary>
    /// <param name="bit"></param>
    /// <returns></returns>
    inline unsigned short bitboardArrayIndexFromBit( unsigned long long bit ) const;

    // Static methods

    /// <summary>
    /// Piece letteer from bitboard array index
    /// </summary>
    /// <param name="arrayIndex"></param>
    /// <returns></returns>
    inline static const char pieceFromBitboardArrayIndex( unsigned short arrayIndex );

    /// <summary>
    /// Bitboard array index from piece letter
    /// </summary>
    /// <param name="piece"></param>
    /// <returns></returns>
    inline static unsigned short bitboardArrayIndexFromPiece( const char piece );

    /// <summary>
    /// Convert Move representation of piece to bitboard array index
    /// </summary>
    /// <param name="promotion"></param>
    /// <returns></returns>
    inline static unsigned long bitboardArrayIndexFromPromotion( unsigned long promotion )
    {
        return promotion == Move::KNIGHT ? 
            KNIGHT : promotion == Move::BISHOP ? 
            BISHOP : promotion == Move::ROOK ? 
            ROOK : QUEEN;
    }

    void getPawnMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces );
    void getKnightMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares );
    void getBishopMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces, const unsigned long long& blockingPieces );
    void getRookMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces, const unsigned long long& blockingPieces );
    void getQueenMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces, const unsigned long long& blockingPieces );
    void getKingMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares );

    typedef unsigned long long ( *DirectionMask )( const unsigned long );
    typedef unsigned char ( *BitScanner )( unsigned long*, unsigned long long );

    inline static unsigned char scanForward( unsigned long* index, unsigned long long mask )
    {
        return _BitScanForward64( index, mask );
    }

    inline static unsigned char scanReverse( unsigned long* index, unsigned long long mask )
    {
        return _BitScanReverse64( index, mask );
    }

    void getDirectionalMoves( std::vector<Move>& moves, const unsigned long& index, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces, const unsigned long long& blockingPieces, DirectionMask directionMask, BitScanner bitScanner );

public:
    static Board* createBoard( const std::string& fen );

    std::string toString() const;

    void getMoves( std::vector<Move>& moves );

    class State
    {
    private:
        std::array<unsigned long long, 13> bitboards;
        bool whiteToMove;
        std::array<bool, 4> castlingRights;
        unsigned long long enPassantIndex;
        unsigned short halfMoveClock;
        unsigned short fullMoveNumber;

        std::array<unsigned short, 64> boardLUT;

        unsigned long long whitePieces;
        unsigned long long blackPieces;
        unsigned long long allPieces;

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

