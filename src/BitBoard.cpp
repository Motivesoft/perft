#include "BitBoard.h"

#include <bitset>
#include <iostream>

unsigned short BitBoard::RANKFILE_MASK = 0b0000000000000111;
unsigned long long BitBoard::ULL_MASK = 0b0000000000000000000000000000000000000000000000000000000000000001;

unsigned long long BitBoard::northMoves[ 64 ];
unsigned long long BitBoard::southMoves[ 64 ];

unsigned long long BitBoard::pawnMovesNormalWhite[ 64 ];
unsigned long long BitBoard::pawnMovesNormalBlack[ 64 ];

void BitBoard::initialize()
{
    for ( unsigned short square = 0; square < 64; square++ )
    {
        unsigned short rank = BitBoard::rank( square );
        unsigned short file = BitBoard::file( square );

        // Sliders
        northMoves[ square ] = createNorthMask( square );
        southMoves[ square ] = createSouthMask( square );

        // Pawns
        pawnMovesNormalWhite[ square ] = 0;
        pawnMovesNormalBlack[ square ] = 0;

        if ( rank < 7 && rank > 0 )
        {
            pawnMovesNormalWhite[ square ] = ULL_MASK << ( square + 8 );

            if ( rank == 1 )
            {
                pawnMovesNormalWhite[ square ] |= ULL_MASK << ( square + 16 );
            }
        }
        if ( rank > 0 && rank < 7 )
        {
            pawnMovesNormalBlack[ square ] = ULL_MASK << ( square - 8 );

            if ( rank == 6 )
            {
                pawnMovesNormalBlack[ square ] |= ULL_MASK << ( square - 16 );
            }
        }

        std::cerr << "Square " << square << " " << (char) ( 'a' + file ) << (char) ( '1' + rank ) << std::endl;
        //dumpBitBoard( northMoves[ square ], " North" );
        //dumpBitBoard( southMoves[ square ], " South" );
        //dumpBitBoard( northMoves[square] | southMoves[ square ], " Combined" );
        //dumpBitBoard( pawnMovesNormalWhite[ square ], " White Pawn" );
        //dumpBitBoard( pawnMovesNormalBlack[ square ], " Black Pawn" );
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
