#pragma once

#include <string>

class Move
{
private:
    unsigned long moveBits;

public:
    Move( unsigned long from, unsigned long to ) :
        moveBits( (from << 6) | to ) 
    {
        
    }

    unsigned short getFrom() const
    {
        return (moveBits >> 6) & 0b0000000000111111;
    }

    unsigned short getTo() const
    {
        return moveBits & 0b0000000000111111;
    }

    std::string toString() const;
};

