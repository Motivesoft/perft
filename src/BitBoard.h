#pragma once

class BitBoard
{
private:
    static unsigned short RANKFILE_MASK;
    static unsigned long long ULL_MASK;

    // Masks for sliders
    static unsigned long long northMoves[ 64 ];
    static unsigned long long southMoves[ 64 ];
   
    static unsigned long long eastMoves[ 64 ];
    static unsigned long long westMoves[ 64 ];

    static unsigned long long northEastMoves[ 64 ];
    static unsigned long long southEastMoves[ 64 ];

    static unsigned long long northWestMoves[ 64 ];
    static unsigned long long southWestMoves[ 64 ];

    // Masks for non-sliders
    static unsigned long long pawnMovesNormalWhite[ 64 ];
    static unsigned long long pawnMovesNormalBlack[ 64 ];
    static unsigned long long pawnMovesExtendedWhite[ 64 ];
    static unsigned long long pawnMovesExtendedBlack[ 64 ];
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

    static unsigned long long createNorthEastMask( unsigned short square )
    {
        unsigned long long mask = 0ull;

        int next = square;
        for ( unsigned short loop = 0; loop < 7; loop++ )
        {
            next += 9;

            // Check for wrap around
            if ( next > 63 )
            {
                break;
            }
            if ( rank( next ) > rank( square ) )
            {
                if ( file( next ) > file( square ) )
                {
                    mask |= ULL_MASK << next;
                }
            }
        }

        return mask;
    }

    static unsigned long long createSouthWestMask( unsigned short square )
    {
        unsigned long long mask = 0ull;

        int next = square;
        for ( unsigned short loop = 0; loop < 7; loop++ )
        {
            next -= 9;

            // Check for wrap around
            if ( next < 0 )
            {
                break;
            }
            if ( rank( next ) < rank( square ) )
            {
                if ( file( next ) < file( square ) )
                {
                    mask |= ULL_MASK << next;
                }
            }
        }

        return mask;
    }

    static unsigned long long createNorthWestMask( unsigned short square )
    {
        unsigned long long mask = 0ull;

        int next = square;
        for ( unsigned short loop = 0; loop < 7; loop++ )
        {
            next += 7;

            // Check for wrap around
            if ( next > 63 )
            {
                break;
            }
            if ( rank( next ) > rank( square ) )
            {
                if ( file( next ) < file( square ) )
                {
                    mask |= ULL_MASK << next;
                }
            }
        }

        return mask;
    }

    static unsigned long long createSouthEastMask( unsigned short square )
    {
        unsigned long long mask = 0ull;

        int next = square;
        for ( unsigned short loop = 0; loop < 7; loop++ )
        {
            next -= 7;

            // Check for wrap around
            if ( next < 0 )
            {
                break;
            }
            if ( rank( next ) < rank( square ) )
            {
                if ( file( next ) > file( square ) )
                {
                    mask |= ULL_MASK << next;
                }
            }
        }

        return mask;
    }

    static unsigned long long createSouthMask( unsigned short square )
    {
        // Zero at the end as that is the home square
        unsigned long long mask = 0b0000000010000000100000001000000010000000100000001000000010000000;
        
        return mask >> (63-square);
    }

    static unsigned long long createEastMask( unsigned short square )
    {
        unsigned long long mask = 0ull;

        int next = square;
        for ( unsigned short loop = 0; loop < 7; loop++ )
        {
            next--;

            // Check for wrap around
            if ( file( next ) < file( square ) )
            {
                mask |= ULL_MASK << next;
            }
        }

        return mask;
    }

    static unsigned long long createWestMask( unsigned short square )
    {
        unsigned long long mask = 0ull;

        int next = square;
        for ( unsigned short loop = 0; loop < 7; loop++ )
        {
            next++;

            // Check for wrap around
            if ( file( next ) > file( square ) )
            {
                mask |= ULL_MASK << next;
            }
        }

        return mask;
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

    inline static unsigned long long getWhitePawnNormalMoveMask( unsigned long index )
    {
        return pawnMovesNormalWhite[ index ];
    }

    inline static unsigned long long getBlackPawnNormalMoveMask( unsigned long index )
    {
        return pawnMovesNormalBlack[ index ];
    }

    inline static unsigned long long getWhitePawnExtendedMoveMask( unsigned long index )
    {
        return pawnMovesExtendedWhite[ index ];
    }

    inline static unsigned long long getBlackPawnExtendedMoveMask( unsigned long index )
    {
        return pawnMovesExtendedBlack[ index ];
    }

    inline static unsigned long long getKnightMoveMask( unsigned long index )
    {
        return kingMoves[ index ];
    }

    inline static unsigned long long getKingMoveMask( unsigned long index )
    {
        return kingMoves[ index ];
    }
};

