
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stdint.h>
#include <stddef.h>

#include <config.hpp>

struct Message
{
    uint8_t data[MTU];
    size_t size;
};

#endif

