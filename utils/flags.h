#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#include <vml5.0/os/common.h>

namespace vml
{
    namespace utils
    {
        /////////////////////////////////////////////////////////////////////////
        // Flags class, packs a fixed amout of flags (1024) into a byte array

        class Flags
        {
            protected:
        
                // max number of flags
                static const uint32_t MAX_FLAGS = 1024;

                // max number of bytes
                static const uint32_t MAX_BYTES = MAX_FLAGS / 8;

                // array holding bytes
                unsigned char Array[MAX_BYTES];

                // bit values
                static const inline unsigned char BitValues[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

            public:

                // -----------------------------------------------
                // set flag to true

                void SetToTrue(uint32_t flagpos)
                {
                    // compute wich index in the byte 
                    // array the flag must be set
                    uint32_t bytepos = flagpos / 8;
                    // once we have the byte in the array
                    // compute the relative flag addres in the
                    // array position
                    uint32_t relpos = flagpos - bytepos * 8;
                    // store value
                    if (bytepos < MAX_BYTES && relpos < 8)
                        Array[bytepos] |= BitValues[relpos];
                    else
                        vml::os::Message::Error("SetToTrue : Array index out of range");
                }

                // -----------------------------------------------
                // set flag to false

                void SetToFalse(uint32_t flagpos)
                {
                    // compute wich index in the byte 
                    // array the flag must be set
                    uint32_t bytepos = flagpos / 8;
                    // once we have the byte in the array
                    // compute the relative flag addres in the
                    // array position
                    uint32_t relpos = flagpos - bytepos * 8;
                    // store value
                    if (bytepos < MAX_BYTES && relpos < 8)
                        Array[bytepos] &= ~BitValues[relpos];
                    else
                        vml::os::Message::Error("SetToFalse : Array index out of range");
                }

                // -----------------------------------------------
                // gets value of single bit

                [[nodiscard]] bool Get(uint32_t flagpos) const
                {
                    // compute wich index in the byte 
                    // array the flag must be set
                    uint32_t bytepos = flagpos / 8;
                    // once we have the byte in the array
                    // compute the relative flag addres in the
                    // array position
                    uint32_t relpos = flagpos - bytepos * 8;
                    // return value
                    if (bytepos < MAX_BYTES && relpos < 8)
                        return ((Array[bytepos] & BitValues[relpos]) != 0);
                    else
                        vml::os::Message::Error("Get : Array index out of range");
                    return false;
                }

                // -----------------------------------------------
                // get 

                static [[nodiscard]] size_t GetFlagsCount() 
                {
                    return MAX_FLAGS;
                }

                // -----------------------------------------------

                static [[nodiscard]] size_t GetElementsCount() 
                {
                    return MAX_BYTES;
                }

                // -----------------------------------------------

                friend vml::utils::Flags operator | (const vml::utils::Flags& f1, const vml::utils::Flags& f2)
                {
                    vml::utils::Flags result;
                    for (size_t j = 0; j < MAX_BYTES; ++j)
                        result.Array[j] = f1.Array[j] | f2.Array[j];
                    return result;
                }

                // -----------------------------------------------

                friend vml::utils::Flags operator &(const vml::utils::Flags& f1, const vml::utils::Flags& f2)
                {
                    vml::utils::Flags result;
                    for (size_t j = 0; j < MAX_BYTES; ++j)
                        result.Array[j] = f1.Array[j] & f2.Array[j];
                    return result;
                }

                // -----------------------------------------------

                friend vml::utils::Flags operator ^(const vml::utils::Flags& f1, const vml::utils::Flags& f2)
                {
                    vml::utils::Flags result;
                    for (size_t j = 0; j < MAX_BYTES; ++j)
                        result.Array[j] = f1.Array[j] ^ f2.Array[j];
                    return result;
                }

                // -----------------------------------------------

                friend bool operator == (const Flags& flags1, const Flags& flags2) { return (memcmp(flags1.Array, flags2.Array, MAX_BYTES) == 0); }
                friend bool operator != (const Flags& flags1, const Flags& flags2) { return (memcmp(flags1.Array, flags2.Array, MAX_BYTES) != 0); }

                // -----------------------------------------------
                // clear array

                void Clear() const
                {
                    memset((unsigned char*)Array, 0, MAX_BYTES);
                }

                // -----------------------------------------------
                // dump array

                void Dump() const
                {
                    for (size_t j = 0; j < MAX_BYTES; ++j)
                    {
                        for (size_t i = 0; i != 8; i++)
                        {
                            unsigned char bit = (Array[j] & (1 << i)) != 0;
                            std::cout << (size_t)bit << " ";
                        }
                    }
                    std::cout << std::endl;
                }

                // -----------------------------------------------

                Flags& operator=(const Flags& other)
                {
                    if (this != &other)
                        memcpy((unsigned char*)Array, (unsigned char*)other.Array, MAX_BYTES);
                    return *this;
                }

                Flags(const Flags& other)
                {
                    memcpy((unsigned char*)Array, (unsigned char*)other.Array, MAX_BYTES);
                }

                // -----------------------------------------------
                // ctor / dtor

                Flags()
                {
                    memset((unsigned char*)Array, 0, MAX_BYTES);
                }

                Flags(std::initializer_list<uint32_t> il)
                {
                    memset((unsigned char*)Array, 0, MAX_BYTES);
                    for (auto it = il.begin(); it != il.end(); ++it)
                        SetToTrue(*it);
                }

                ~Flags()
                {
                }

        };
    }
}
