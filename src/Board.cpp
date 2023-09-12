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

    const unsigned long long& blockingPieces = whiteToMove ? whitePieces : blackPieces;
    const unsigned long long& attackPieces = whiteToMove ? blackPieces : whitePieces;
    const unsigned long long& accessibleSquares = emptySquares | attackPieces;

    // normal piece logic for all pieces
    // ep? done
    // castling? done
    // promotion? done
    // TODO move legality test

    // Pawn (including ep capture, promotion)
    getPawnMoves( moves, bitboardPieceIndex + PAWN, accessibleSquares, attackPieces );

    // Knight
    getKnightMoves( moves, bitboardPieceIndex + KNIGHT, accessibleSquares );

    // Bishop + Queen
    getBishopMoves( moves, bitboardPieceIndex + BISHOP, accessibleSquares, attackPieces, blockingPieces );

    // Rook (including castling flag set) + Queen
    getRookMoves( moves, bitboardPieceIndex + ROOK, accessibleSquares, attackPieces, blockingPieces );

    // Queen
    getQueenMoves( moves, bitboardPieceIndex + QUEEN, accessibleSquares, attackPieces, blockingPieces );

    // King (including castling, castling flag set)
    getKingMoves( moves, bitboardPieceIndex + KING, accessibleSquares );
}

Board::State Board::makeMove( const Move& move )
{
    Board::State state( *this );

    // TODO the actual make move stuff here
    //   basic piece move 
    //      capture
    //      ep
    //      castling 
    //      promotion
    //   whiteToMove
    //   castling
    //   ep index
    //   halfMove
    //   fullMove

    const unsigned short bitboardPieceIndex = whiteToMove ? 0 : 6;
    const unsigned short opponentBitboardPieceIndex = whiteToMove ? 6 : 0;

    const unsigned short from = move.getFrom();
    const unsigned short to = move.getTo();
    const unsigned long promotion = move.getPromotion();

    const unsigned long long fromBit = 1ull << move.getFrom();
    const unsigned long long toBit = 1ull << move.getTo();

    unsigned short pieceIndex = boardLUT[ from ];
    unsigned short destination = boardLUT[ to ];

    //std::cerr << "Making Move: " << move.toString() << " for " << (char*)(whiteToMove? "white" : "black" ) << " with a " << pieceFromBitboardArrayIndex( pieceIndex ) << std::endl;

    // Find which piece is moving and move it, with any required side-effects
    //  - promotion
    //  - capture
    //  - capture through promotion
    //  - ep capture
    //  - ep flag update
    //  - castling
    //  - castling flag update

    // With a regular move, it is just a case of moving from->to in a single bitboard and then refreshing the other masks
    
    //(bitboards[ pieceIndex ] ^= fromBit) |= toBit;

    if ( promotion )
    {

    }

    if ( destination == USHRT_MAX )
    {

    }

    //for ( pieceIndex = 0; pieceIndex < 6; pieceIndex++ )
    //{
    //    if ( bitboards[ bitboardPieceIndex + pieceIndex ] & fromBit )
    //    {
    //        std::cerr << "  Piece: " << pieceFromBitboardArrayIndex( bitboardPieceIndex + pieceIndex ) << std::endl;

    //        // Remove the piece from
    //        std::cerr << "Removing from " << from;
    //        bitboards[ bitboardPieceIndex + pieceIndex ] ^= fromBit;

    //        if ( promotion )
    //        {
    //            std::cerr << " promoting at " << to << std::endl;
    //            bitboards[ bitboardPieceIndex + bitboardArrayIndexFromPromotion( promotion ) ] |= toBit;
    //        }
    //        else
    //        {
    //            std::cerr << " placing at " << to << std::endl;
    //            BitBoard::dumpBitBoard( bitboards[ bitboardPieceIndex + pieceIndex ], " Before" );
    //            bitboards[ bitboardPieceIndex + pieceIndex ] |= toBit;
    //            BitBoard::dumpBitBoard( bitboards[ bitboardPieceIndex + pieceIndex ], " After" );
    //        }

    //        break;
    //    }
    //}

    //// Deal with a capture - ep or otherwise
    //if ( toBit == enPassantIndex && pieceIndex == PAWN )
    //{
    //    std::cerr << "EP capture" << std::endl;

    //    // Clear enemy pawn from where it actually is
    //    bitboards[ opponentBitboardPieceIndex + PAWN ] ^= (whiteToMove ? toBit << 8 : toBit >> 8);
    //    BitBoard::dumpBitBoard( bitboards[ bitboardPieceIndex + pieceIndex ], " Mine" );
    //    BitBoard::dumpBitBoard( bitboards[ opponentBitboardPieceIndex + PAWN ], " Theirs" );
    //}
    //else
    //{
    //    unsigned short capturedPiece = bitboardArrayIndexFromBit( toBit );
    //    if ( capturedPiece |= USHRT_MAX )
    //    {
    //        bitboards[ capturedPiece ] ^= toBit;
    //    }

    //}

    whiteToMove = !whiteToMove;

    //resetMasks();

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
    std::array<unsigned long long, 12> bitboards = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

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
    std::array<bool, 4> castlingRights = { false, false, false, false };

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
    if ( !( castlingRights[ 0 ] || castlingRights[ 1 ] || castlingRights[ 2 ] || castlingRights[ 3 ] ) )
    {
        fen << "-";
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

unsigned short Board::bitboardArrayIndexFromBit( unsigned long long bit ) const
{
    for ( unsigned short loop = 0; loop < 12; loop++ )
    {
        if ( bitboards[ loop ] & bit )
        {
            return loop;
        }
    }

    return USHRT_MAX;
}

const char Board::pieceFromBitboardArrayIndex( unsigned short arrayIndex )
{
    return "PNBRQKpnbrqk"[ arrayIndex ];
}

unsigned short Board::bitboardArrayIndexFromPiece( const char piece )
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
    bitboards( board.bitboards ),
    whiteToMove( board.whiteToMove ),
    castlingRights( board.castlingRights ),
    enPassantIndex( board.enPassantIndex ),
    halfMoveClock( board.halfMoveClock ),
    fullMoveNumber( board.fullMoveNumber ),
    boardLUT( board.boardLUT ),
    whitePieces( board.whitePieces ),
    blackPieces( board.blackPieces ),
    allPieces( board.allPieces ),
    emptySquares( board.emptySquares )
{
}

void Board::State::apply( Board& board ) const
{
    board.bitboards = bitboards;
    board.whiteToMove = whiteToMove;
    board.castlingRights = castlingRights;
    board.enPassantIndex = enPassantIndex;
    board.halfMoveClock = halfMoveClock;
    board.fullMoveNumber = fullMoveNumber;
    board.boardLUT = boardLUT;
    board.whitePieces = whitePieces;
    board.blackPieces = blackPieces;
    board.allPieces = allPieces;
    board.emptySquares = emptySquares;
}

void Board::getPawnMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces )
{
    const unsigned short promotionRankFrom = whiteToMove ? 6 : 1;
    const unsigned short homeRankFrom = whiteToMove ? 1 : 6;

    unsigned long long pieces;
    unsigned long index;
    unsigned long destination;
    unsigned long long baselinePawns = 0;

    unsigned short rankFrom;
    unsigned long long possibleMoves;

    pieces = bitboards[ pieceIndex ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        rankFrom = ( index >> 3 ) & 0b00000111;

        possibleMoves = whiteToMove ? BitBoard::getWhitePawnNormalMoveMask( index ) : BitBoard::getBlackPawnNormalMoveMask( index );

        possibleMoves &= emptySquares;

        while ( _BitScanForward64( &destination, possibleMoves ) )
        {
            possibleMoves ^= 1ull << destination;

            // Check whether any are elegible to make the extended move (e.g. e2e4) or promote
            if ( rankFrom == homeRankFrom )
            {
                baselinePawns |= 1ull << index;
                moves.push_back( Move( index, destination ) );
            }
            else if ( rankFrom == promotionRankFrom )
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

    // Of the pawns that could make a single move, which can also make the double move?
    pieces = baselinePawns;
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        possibleMoves = whiteToMove ? BitBoard::getWhitePawnExtendedMoveMask( index ) : BitBoard::getBlackPawnExtendedMoveMask( index );

        possibleMoves &= emptySquares;

        while ( _BitScanForward64( &destination, possibleMoves ) )
        {
            possibleMoves ^= 1ull << destination;

            // No need to check promotion here as this is only for pawns on their home rank
            moves.push_back( Move( index, destination ) );
        }
    }

    // Captures, including ep
    pieces = bitboards[ pieceIndex ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        rankFrom = ( index >> 3 ) & 0b00000111;
        possibleMoves = whiteToMove ? BitBoard::getWhitePawnAttackMoveMask( index ) : BitBoard::getBlackPawnAttackMoveMask( index );

        possibleMoves &= ( attackPieces | enPassantIndex );

        while ( _BitScanForward64( &destination, possibleMoves ) )
        {
            possibleMoves ^= 1ull << destination;

            if ( rankFrom == promotionRankFrom )
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
}

void Board::getKnightMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares )
{
    unsigned long long pieces;
    unsigned long index;
    unsigned long destination;

    pieces = bitboards[ pieceIndex ];
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
}

void Board::getBishopMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces, const unsigned long long& blockingPieces )
{
    unsigned long long pieces;
    unsigned long index;

    pieces = bitboards[ pieceIndex ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getNorthEastMoveMask, &scanForward );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getNorthWestMoveMask, &scanForward );

        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getSouthWestMoveMask, &scanReverse );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getSouthEastMoveMask, &scanReverse );
    }
}

void Board::getRookMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces, const unsigned long long& blockingPieces )
{
    unsigned long long pieces;
    unsigned long index;

    pieces = bitboards[ pieceIndex ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getNorthMoveMask, &scanForward );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getWestMoveMask, &scanForward );

        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getSouthMoveMask, &scanReverse );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getEastMoveMask, &scanReverse );
    }
}

void Board::getQueenMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces, const unsigned long long& blockingPieces )
{
    unsigned long long pieces;
    unsigned long index;

    pieces = bitboards[ pieceIndex ];
    while ( _BitScanForward64( &index, pieces ) )
    {
        pieces ^= 1ull << index;

        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getNorthMoveMask, &scanForward );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getWestMoveMask, &scanForward );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getNorthEastMoveMask, &scanForward );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getNorthWestMoveMask, &scanForward );

        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getSouthMoveMask, &scanReverse );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getEastMoveMask, &scanReverse );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getSouthWestMoveMask, &scanReverse );
        getDirectionalMoves( moves, index, accessibleSquares, attackPieces, blockingPieces, &BitBoard::getSouthEastMoveMask, &scanReverse );
    }
}

void Board::getKingMoves( std::vector<Move>& moves, const unsigned short& pieceIndex, const unsigned long long& accessibleSquares )
{
    unsigned long long pieces;
    unsigned long index;
    unsigned long destination;

    // There is only one king, so we can use an if, not a when here and be sure we're only going round once
    pieces = bitboards[ pieceIndex ];
    if ( _BitScanForward64( &index, pieces ) )
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

// Pass a scanner in here so that we can look either forward or reverse to make sure we check the closest attacker/blocker and
// don't waste time checking those further away
void Board::getDirectionalMoves( std::vector<Move>& moves, const unsigned long& index, const unsigned long long& accessibleSquares, const unsigned long long& attackPieces, const unsigned long long& blockingPieces, DirectionMask directionMask, BitScanner bitScanner )
{
    unsigned long otherIndex;

    // Get the direction mask (e.g. NorthEast)
    unsigned long long possibleMoves = directionMask( index );

    // Work out the pieces along the way
    unsigned long long attackersOfInterest = attackPieces & possibleMoves;
    unsigned long long blockersOfInterest = blockingPieces & possibleMoves;

    // For each attacker, clip the path to exclude any steps beyond the attacker
    if ( bitScanner( &otherIndex, attackersOfInterest ) )
    {
        possibleMoves &= ~directionMask( otherIndex );
    }

    // For each blocker (own piece), clip the path to exclude any steps beyond the blocker (and including the blocker)
    if ( bitScanner( &otherIndex, blockersOfInterest ) )
    {
        // Add the blocker pieces back in, before we 'not' it, to take out the blocking piece itself
        possibleMoves &= ~( directionMask( otherIndex ) | blockingPieces );
    }

    // Then create a move for each step along the mask that remains
    while ( bitScanner( &otherIndex, possibleMoves ) )
    {
        possibleMoves ^= 1ull << otherIndex;

        moves.push_back( Move( index, otherIndex ) );
    }
}
