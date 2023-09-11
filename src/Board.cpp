#include "Board.h"

#include <iostream>
#include <sstream>

#include "BitBoard.h"

// Indices into bitboards
const unsigned short Board::PAWN = 0;
const unsigned short Board::KNIGHT = 1;
const unsigned short Board::BISHOP = 2;
const unsigned short Board::ROOK = 3;
const unsigned short Board::QUEEN = 4;
const unsigned short Board::KING = 5;

void Board::getMoves( std::vector<Move>& moves )
{
    const unsigned short bitboardPieceIndex = whiteToMove ? 0 : 6;
    const unsigned short promotionBaseline = whiteToMove ? 7 : 0;

    const unsigned long long& blockingPieces = whiteToMove ? whitePieces : blackPieces;
    const unsigned long long& attackPieces = whiteToMove ? blackPieces : whitePieces;
    const unsigned long long& accessibleSquares = emptySquares | attackPieces;

    // Working variables
    unsigned long long pieces;
    unsigned long index;
    unsigned long destination;
    unsigned long long baselinePawns = 0;

    // Have a movement and capture mask?
    // - find all free moves (in a direction from a square) 
    //   - and then look at the next square to see whether its an attacker

    // Pawn (including ep capture, promotion)

    pieces = bitboards[ bitboardPieceIndex + PAWN ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        unsigned long long possibleMoves = whiteToMove? BitBoard::getWhitePawnNormalMoveMask( index ) : BitBoard::getBlackPawnNormalMoveMask( index );

        possibleMoves &= emptySquares;

        while ( _BitScanForward64( &destination, possibleMoves ) )
        {
            possibleMoves ^= 1ull << destination;

            // Remember those that could move at this point as they may also be able to make the extended move
            baselinePawns |= 1ull << index;

            moves.push_back( Move( index, destination ) );
        }
    }

    // Of the pawns that could make a single move, which can also make the double move?
    pieces = baselinePawns;
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        unsigned short rank = ( index >> 3 ) & 0b00000111;
        unsigned long long possibleMoves = whiteToMove ? BitBoard::getWhitePawnExtendedMoveMask( index ) : BitBoard::getBlackPawnExtendedMoveMask( index );

        possibleMoves &= emptySquares;

        while ( _BitScanForward64( &destination, possibleMoves ) )
        {
            possibleMoves ^= 1ull << destination;

            if ( rank == promotionBaseline )
            {
                moves.push_back( Move( index, destination, Move::KNIGHT ) );
                moves.push_back( Move( index, destination, Move::BISHOP ) );
                moves.push_back( Move( index, destination, Move::ROOK ) );
                moves.push_back( Move( index, destination, Move::QUEEN ) );
            }
            else
            {
                moves.push_back( Move( index, destination ) );
            }
        }
    }

    pieces = bitboards[ bitboardPieceIndex + PAWN ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        unsigned short rank = ( index >> 3 ) & 0b00000111;
        unsigned long long possibleMoves = whiteToMove ? BitBoard::getWhitePawnAttackMoveMask( index ) : BitBoard::getBlackPawnAttackMoveMask( index );

        possibleMoves &= (attackPieces | enPassantIndex);

        while ( _BitScanForward64( &destination, possibleMoves ) )
        {
            possibleMoves ^= 1ull << destination;

            if ( rank == promotionBaseline )
            {
                moves.push_back( Move( index, destination, Move::KNIGHT ) );
                moves.push_back( Move( index, destination, Move::BISHOP ) );
                moves.push_back( Move( index, destination, Move::ROOK ) );
                moves.push_back( Move( index, destination, Move::QUEEN ) );
            }
            else
            {
                moves.push_back( Move( index, destination ) );
            }
        }
    }

    // Knight
    pieces = bitboards[ bitboardPieceIndex + KNIGHT ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        unsigned long long possibleMoves = BitBoard::getKnightMoveMask( index );

        possibleMoves &= accessibleSquares;

        while ( _BitScanForward64( &destination, possibleMoves ) )
        {
            possibleMoves ^= 1ull << destination;

            moves.push_back( Move( index, destination ) );
        }
    }

    // Bishop + Queen

    // Rook (including castling flag set) + Queen

    pieces = bitboards[ bitboardPieceIndex + ROOK ] | bitboards[ bitboardPieceIndex + QUEEN ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        //////////////// TODO clever sliding logic here
        //unsigned long long possibleMoves = BitBoard::getEastMoveMask( index );

        //possibleMoves &= accessibleSquares;

        //while ( _BitScanForward64( &destination, possibleMoves ) )
        //{
        //    possibleMoves ^= 1ull << destination;

        //    moves.push_back( Move( index, destination ) );
        //}
    }

    // Queen - already covered

    // King (including castling, castling flag set)

    pieces = bitboards[ bitboardPieceIndex + KING ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        unsigned long long possibleMoves = BitBoard::getKingMoveMask( index );

        possibleMoves &= accessibleSquares;

        while ( _BitScanForward64( &destination, possibleMoves ) )
        {
            possibleMoves ^= 1ull << destination;

            moves.push_back( Move( index, destination ) );
        }

        // Check whether castling is a possibility
        unsigned long long castlingMask;
        if ( whiteToMove )
        {
            if ( castlingRights[ 0 ] )
            {
                castlingMask = BitBoard::getWhiteKingsideCastlingMask();

                if ( ( emptySquares & castlingMask ) == castlingMask )
                {
                    moves.push_back( Move( index, index + 2 ) );
                }
            }
            if ( castlingRights[ 1 ] )
            {
                castlingMask = BitBoard::getWhiteQueensideCastlingMask();

                if ( ( emptySquares & castlingMask ) == castlingMask )
                {
                    moves.push_back( Move( index, index - 2 ) );
                }
            }
        }
        else
        {
            if ( castlingRights[ 2 ] )
            {
                castlingMask = BitBoard::getBlackKingsideCastlingMask();

                if ( ( emptySquares & castlingMask ) == castlingMask )
                {
                    moves.push_back( Move( index, index + 2 ) );
                }
            }
            if ( castlingRights[ 3 ] )
            {
                castlingMask = BitBoard::getBlackQueensideCastlingMask();

                if ( ( emptySquares & castlingMask ) == castlingMask )
                {
                    moves.push_back( Move( index, index - 2 ) );
                }
            }
        }
    }
}

Board::State Board::makeMove( const Move& move )
{
    Board::State state( *this );

    // TODO the actual make move stuff here
    //   piece move (including special moves - capture, ep, castling, promotion)
    //   whiteToMove
    //   castling
    //   ep index
    //   halfMove
    //   fullMove

    whiteToMove = !whiteToMove;

    return state;
}

void Board::unmakeMove( const Board::State& state )
{
    state.apply( *this );
}

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

    // Two sets of bitboards, white and black
    unsigned long long bitboards[ 12 ];
    for ( size_t loop = 0; loop < 12; loop++ )
    {
        bitboards[ loop ] = 0;
    }

    // Unpack FEN board representation
    unsigned long long mask = 1ull << 56;
    size_t rank = 7;

    // Split by '/' and assume the input data is valid
    size_t pos = 0;
    std::string delimiter( "/" );
    std::string token;
    while ( ( pos = pieces.find( delimiter ) ) != std::string::npos )
    {
        token = pieces.substr( 0, pos );

        for ( std::string::const_iterator it = token.cbegin(); it != token.cend(); it++ )
        {
            if ( isdigit( *it ) )
            {
                char distance[ 2 ];
                distance[ 0 ] = *it;
                distance[ 1 ] = '\0';
                mask <<= atoi( distance );
            }
            else
            {
                bitboards[ bitboardArrayIndexFromPiece( *it ) ] |= mask;
                mask <<= 1;
            }
        }

        // Move to next line
        rank--;
        mask = 1ull << (rank << 3);

        pieces.erase( 0, pos + delimiter.length() );
    }

    // Handle the last line
    token = pieces;

    for ( std::string::const_iterator it = token.cbegin(); it != token.cend(); it++ )
    {
        if ( isdigit( *it ) )
        {
            char distance[ 2 ];
            distance[ 0 ] = *it;
            distance[ 1 ] = '\0';
            mask <<= atoi( distance );
        }
        else
        {
            bitboards[ bitboardArrayIndexFromPiece( *it ) ] |= mask;
            mask <<= 1;
        }
    }

    bool whiteToPlay = color == "w";

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

    return new Board( bitboards,
                      whiteToPlay,
                      castlingRights,
                      ep,
                      atoi( halfMoveClock.c_str() ),
                      atoi( fullMoveNumber.c_str() ) );
}

std::string Board::toString() const
{
    std::stringstream fen;

    // Pieces
    unsigned short counter = 0;
    size_t file = 0;
    size_t rank = 7;
    for ( unsigned long long mask = 1ull << 56; ; )
    {
        if ( isEmpty( mask ) )
        {
            counter++;
        }
        else
        {
            if ( counter > 0 )
            {
                fen << counter;
                counter = 0;
            }
            fen << pieceFromBitboardArrayIndex( bitboardArrayIndexFromBit( mask ) );
        }
        mask <<= 1;
        file++;
        if ( file == 8 )
        {
            if ( counter > 0 )
            {
                fen << counter;
                counter = 0;
            }

            if ( rank == 0 )
            {
                break;
            }

            rank--;
            
            fen << "/";

            file = 0;

            // Move to the next rank down the board (h-a)
            mask = 1ull << (rank << 3);
        }
    }

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
    fen << fullMoveNumber;

    return fen.str();
}

bool Board::isEmpty( unsigned long long bit ) const
{
    return emptySquares & bit;
}

size_t Board::bitboardArrayIndexFromBit( unsigned long long bit ) const
{
    for ( size_t loop = 0; loop < 12; loop++ )
    {
        if ( bitboards[ loop ] & bit )
        {
            return loop;
        }
    }

    return -1;
}

const char Board::pieceFromBitboardArrayIndex( size_t arrayIndex )
{
    return "PNBRQKpnbrqk"[ arrayIndex ];
}

size_t Board::bitboardArrayIndexFromPiece( const char piece )
{
    switch ( piece )
    {
        case 'P':
            return 0;

        case 'N':
            return 1;

        case 'B':
            return 2;

        case 'R':
            return 3;

        case 'Q':
            return 4;

        case 'K':
            return 5;

        case 'p':
            return 6;

        case 'n':
            return 7;

        case 'b':
            return 8;

        case 'r':
            return 9;

        case 'q':
            return 10;

        case 'k':
            return 11;

        default:
            return -1;
    }
}

Board::State::State( const Board& board ) :
    whiteToMove( board.whiteToMove ),
    enPassantIndex( board.enPassantIndex ),
    halfMoveClock( board.halfMoveClock ),
    fullMoveNumber( board.fullMoveNumber )
{
    for ( size_t loop = 0; loop < 12; loop++ )
    {
        this->bitboards[ loop ] = board.bitboards[ loop ];
    }

    for ( size_t loop = 0; loop < 4; loop++ )
    {
        this->castlingRights[ loop ] = board.castlingRights[ loop ];
    }
}

void Board::State::apply( Board& board ) const
{
    board.whiteToMove = whiteToMove;
    board.enPassantIndex = enPassantIndex;
    board.halfMoveClock = halfMoveClock;
    board.fullMoveNumber = fullMoveNumber;

    board.allPieces = board.whitePieces = board.blackPieces = board.emptySquares = 0;

    for ( size_t loop = 0; loop < 12; loop++ )
    {
        board.bitboards[ loop ] = bitboards[ loop ];

        if ( loop < 6 )
        {
            board.whitePieces |= bitboards[ loop ];
        }
        else
        {
            board.blackPieces |= bitboards[ loop ];
        }
    }

    board.allPieces = board.whitePieces | board.blackPieces;
    board.emptySquares = ~board.allPieces;

    for ( size_t loop = 0; loop < 4; loop++ )
    {
        board.castlingRights[ loop ] = castlingRights[ loop ];
    }
}
