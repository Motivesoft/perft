#pragma once

class BitBoard
{
private:
    static unsigned short RANKFILE_MASK;
    static unsigned long long ULL_MASK;

    // Masks for sliders
    static unsigned long long northMoves[ 64 ];
    static unsigned long long southMoves[ 64 ];
   
    static unsigned long long northEastMoves[ 64 ];
    static unsigned long long southEastMoves[ 64 ];

    static unsigned long long northWestMoves[ 64 ];
    static unsigned long long southWestMoves[ 64 ];

    // Masks for non-sliders
    static unsigned long long pawnMovesNormalWhite[ 64 ];
    static unsigned long long pawnMovesNormalBlack[ 64 ];
    static unsigned long long pawnMovesAttackWhite[ 64 ];
    static unsigned long long pawnMovesAttackBlack[ 64 ];

    static unsigned long long knightMoves[ 64 ];

    static unsigned long long kingMoves[ 64 ];

    static unsigned long long createNorthMask( unsigned short square )
    {
        // Zero at the end as that is the home square
        unsigned long long mask = 0b0000000100000001000000010000000100000001000000010000000100000000;

        return mask << square;
    }

    static unsigned long long createSouthMask( unsigned short square )
    {
        // Zero at the end as that is the home square
        unsigned long long mask = 0b0000000010000000100000001000000010000000100000001000000010000000;
        
        return mask >> (63-square);
    }

    inline static unsigned short file( unsigned short square )
    {
        return square & RANKFILE_MASK;
    }

    inline static unsigned short rank( unsigned short square )
    {
        return ( square >> 3 ) & RANKFILE_MASK;
    }

    static void dumpBitBoard( unsigned long long mask, const char* title = "" );

public:
    static void initialize();
};

