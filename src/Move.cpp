#include "Move.h"

#include <iostream>
#include <sstream>

Move::Move( unsigned long from, unsigned long to ) :
    moveBits( ( from << 6 ) | to )
{
    std::cerr << "From: " << std::hex << from << ". To: " << std::hex << to << ". Move: " << toString() << std::endl;
}

std::string Move::toString() const
{
    std::stringstream move;

    unsigned char fromRank = ( moveBits >> 9 ) & 0b00000111;
    unsigned char fromFile = ( moveBits >> 6 ) & 0b00000111;
    unsigned char toRank = ( moveBits >> 3 ) & 0b00000111;
    unsigned char toFile = ( moveBits ) & 0b00000111;

    move << (char) ( 'a' + fromFile ) << (char) ( '1' + fromRank ) << (char) ( 'a' + toFile ) << (char) ( '1' + toRank );

    return move.str();
}
