#pragma once

#include <string>

class Move
{
private:
    unsigned long moveBits;

public:
    Move( unsigned long from, unsigned long to );

    inline unsigned short getFrom() const
    {
        return (moveBits >> 6) & 0b0000000000111111;
    }

    inline unsigned short getTo() const
    {
        return moveBits & 0b0000000000111111;
    }

    std::string toString() const;
};

