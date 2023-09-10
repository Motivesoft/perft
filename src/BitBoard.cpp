#include "BitBoard.h"

#include <bitset>
#include <iostream>

unsigned short BitBoard::RANKFILE_MASK = 0b0000000000000111;
unsigned long long BitBoard::ULL_MASK = 0b0000000000000000000000000000000000000000000000000000000000000001;

unsigned long long BitBoard::northMoves[ 64 ];
unsigned long long BitBoard::southMoves[ 64 ];

unsigned long long BitBoard::northEastMoves[ 64 ];
unsigned long long BitBoard::southWestMoves[ 64 ];

unsigned long long BitBoard::pawnMovesNormalWhite[ 64 ];
unsigned long long BitBoard::pawnMovesNormalBlack[ 64 ];
unsigned long long BitBoard::pawnMovesAttackWhite[ 64 ];
unsigned long long BitBoard::pawnMovesAttackBlack[ 64 ];

void BitBoard::initialize()
{
    for ( unsigned short square = 0; square < 64; square++ )
    {
        unsigned short rank = BitBoard::rank( square );
        unsigned short file = BitBoard::file( square );

        // Sliders
        northMoves[ square ] = createNorthMask( square );
        southMoves[ square ] = createSouthMask( square );

        //northEastMoves[ square ] = createNorthEastMask( square );
        //southWestMoves[ square ] = createSouthWestMask( square );

        // Pawns
        pawnMovesNormalWhite[ square ] = 0;
        pawnMovesNormalBlack[ square ] = 0;
        pawnMovesAttackWhite[ square ] = 0;
        pawnMovesAttackBlack[ square ] = 0;

        // Pawns don't move from first or last ranks
        if ( rank < 7 && rank > 0 )
        {
            // White pawn
            pawnMovesNormalWhite[ square ] = ULL_MASK << ( square + 8 );

            // Initial double move
            if ( rank == 1 )
            {
                pawnMovesNormalWhite[ square ] |= ULL_MASK << ( square + 16 );
            }

            // Capture
            if ( file == 7 )
            {
                pawnMovesAttackWhite[ square ] = ULL_MASK << ( square + 7 );
            }
            else if ( file == 0 )
            {
                pawnMovesAttackWhite[ square ] = ULL_MASK << ( square + 9 );
            }
            else
            {
                pawnMovesAttackWhite[ square ] = (ULL_MASK << ( square + 7 )) | (ULL_MASK << ( square + 9 ));
            }

            // Black pawn
            pawnMovesNormalBlack[ square ] = ULL_MASK << ( square - 8 );

            // Initial double move
            if ( rank == 6 )
            {
                pawnMovesNormalBlack[ square ] |= ULL_MASK << ( square - 16 );
            }

            // Capture
            if ( file == 7 )
            {
                pawnMovesAttackBlack[ square ] = ULL_MASK << ( square - 9 );
            }
            else if ( file == 0 )
            {
                pawnMovesAttackBlack[ square ] = ULL_MASK << ( square - 7 );
            }
            else
            {
                pawnMovesAttackBlack[ square ] = ( ULL_MASK << ( square - 7 ) ) | ( ULL_MASK << ( square - 9 ) );
            }
        }

        std::cerr << "Square " << square << " " << (char) ( 'a' + file ) << (char) ( '1' + rank ) << std::endl;
        //dumpBitBoard( northMoves[ square ], " North" );
        //dumpBitBoard( southMoves[ square ], " South" );
        //dumpBitBoard( northMoves[square] | southMoves[ square ], " Combined" );
        //dumpBitBoard( pawnMovesNormalWhite[ square ], " White Pawn" );
        dumpBitBoard( pawnMovesAttackWhite[ square ], " White Pawn Attack" );
        //dumpBitBoard( pawnMovesNormalBlack[ square ], " Black Pawn" );
        dumpBitBoard( pawnMovesAttackBlack[ square ], " Black Pawn Attack" );
        //dumpBitBoard( northEastMoves[square] | southWestMoves[ square ], " Combined" );
    }
}

void BitBoard::dumpBitBoard( unsigned long long mask, const char* title )
{
    for ( unsigned short loop = 8; loop > 0; loop-- )
    {
        std::cerr << std::bitset<8>( mask >> ( ( loop - 1 ) << 3 ) ) << (loop == 8 ? title : "" ) << std::endl;
    }
    std::cerr << std::endl;
}
