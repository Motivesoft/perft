#include "Move.h"

#include <sstream>

std::string Move::toString() const
{
    std::stringstream move;

    unsigned char fromRank = ( moveBits >> 9 ) & 0b00000111;
    unsigned char fromFile = ( moveBits >> 6 ) & 0b00000111;
    unsigned char toRank = ( moveBits >> 3 ) & 0b00000111;
    unsigned char toFile = ( moveBits ) & 0b00000111;

    move << (char) ( 'a' + fromRank ) << (char) ( '1' + fromFile ) << (char) ( 'a' + toRank ) << (char) ( '1' + toFile );

    return move.str();
}
