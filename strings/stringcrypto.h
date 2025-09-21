#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
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

namespace vml
{
	namespace strings
	{

			// -------------------------------------------------------------------------
			// md5 hashing class

			class MD5
			{

				private:

					// -------------------------------------------------------------------------
					// md5 context data struct

					typedef struct
					{
						unsigned long int state[4];         // state (ABCD) 
						unsigned long int count[2];         // number of bits, modulo 2^64 (lsb first) 
						unsigned char buffer[64];           // input buffer 
					} MD5_CTX;

					// ----------------------------------------------------------------------
					// Constants for MD5Transform routine.

					static const int S11= 7;
					static const int S12=12;
					static const int S13=17;
					static const int S14=22;
					static const int S21= 5;
					static const int S22= 9;
					static const int S23=14;
					static const int S24=20;
					static const int S31= 4;
					static const int S32=11;
					static const int S33=16;
					static const int S34=23;
					static const int S41= 6;
					static const int S42=10;
					static const int S43=15;
					static const int S44=21;

					// ----------------------------------------------------------------------------------
					// low level logic operations
					// F, G, H and I are basic MD5 functions.

					__forceinline unsigned int F(unsigned int x, unsigned int y, unsigned int z) { return x&y | ~x&z; }
					__forceinline unsigned int G(unsigned int x, unsigned int y, unsigned int z) { return x&z | y&~z; }
					__forceinline unsigned int H(unsigned int x, unsigned int y, unsigned int z) { return x^y^z; }
					__forceinline unsigned int I(unsigned int x, unsigned int y, unsigned int z) { return y ^ (x | ~z); }

					// ----------------------------------------------------------------------------------
					// rotate_left rotates x left n bits.

					__forceinline unsigned int rotate_left(unsigned int x, int n) 
					{
							return (x << n) | (x >> (32-n)); 
					}

					// ----------------------------------------------------------------------------------
					// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
					// Rotation is separate from addition to prevent recomputation.
		
					__forceinline void FF(unsigned long int &a, 
										  unsigned long int b, 
										  unsigned long int c, 
										  unsigned long int d, 
										  unsigned long int x, 
										  unsigned long int s, 
										  unsigned long int ac) 
					{
						  a = rotate_left(a+ F(b,c,d) + x + ac, s) + b;
					}
		
					__forceinline void GG(unsigned long int &a, 
										  unsigned long int b, 
										  unsigned long int c, 
										  unsigned long int d, 
										  unsigned long int x, 
										  unsigned long int s, 
										  unsigned long int ac)
					{
						 a = rotate_left(a + G(b,c,d) + x + ac, s) + b;
					}
		
					__forceinline void HH(unsigned long int &a, 
										  unsigned long int b, 
										  unsigned long int c, 
										  unsigned long int d, 
										  unsigned long int x, 
										  unsigned long int s, 
										  unsigned long int ac) 
					{
						 a = rotate_left(a + H(b,c,d) + x + ac, s) + b;
					}
 
					__forceinline void II(unsigned long int &a, 
										  unsigned long int b, 
										  unsigned long int c, 
										  unsigned long int d, 
										  unsigned long int x, 
										  unsigned long int s, 
										  unsigned long int ac) 
					{
						  a = rotate_left(a + I(b,c,d) + x + ac, s) + b;
					}

					// ----------------------------------------------------------------------
					// Decodes input (unsigned char) into output (unsigned long int). Assumes len is
					// a multiple of 4.
		 
					void Decode(unsigned long int *output, unsigned char *input, unsigned int len)
					{
						  unsigned int i, j;

						for(i = 0, j = 0; j < len; i++, j += 4)
							output[i] = ((unsigned long int)input[j]) |
									(((unsigned long int)input[j+1]) << 8) |
									(((unsigned long int)input[j+2]) << 16) |
									(((unsigned long int)input[j+3]) << 24);
					}
		
					// ----------------------------------------------------------------------
					// Note: Replace "for loop" with standard memcpy if possible.
		
					void MD5_memcpy(unsigned char * output, unsigned char * input, unsigned int len)
					{
						for( unsigned int i = 0; i < len; i++) output[i] = input[i];
					}

					// ----------------------------------------------------------------------
					// Note: Replace "for loop" with standard memset if possible.
		 
					void MD5_memset(unsigned char * output,int value,unsigned int len)
					{
						for( unsigned int i = 0; i < len; i++) ((char*)output)[i] = (char)value;
					}

					// ----------------------------------------------------------------------
					// Encodes input (unsigned long int) into output (unsigned char). Assumes len is
					// a multiple of 4.
 
					void Encode(unsigned char *output, unsigned long int *input, unsigned int len)
					{
						unsigned int i, j;

						for(i = 0, j = 0; j < len; i++, j += 4)
						{
							output[j  ] = (unsigned char) (input[i]		   & 0xff);
							output[j+1] = (unsigned char)((input[i] >>  8) & 0xff);
							output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
							output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
						}
					}

					// -----------------------------------------------------------------------------------
					// MD5 basic transformation. Transforms state based on block.
 
					void MD5Transform(unsigned long int state[4], unsigned char block[64])
					{
						unsigned long int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

						Decode (x, block, 64);

						// Round 1 

						FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
						FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
						FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
						FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
						FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
						FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
						FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
						FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
						FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
						FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
						FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
						FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
						FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
						FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
						FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
						FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

						// Round 2 
			
						GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
						GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
						GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
						GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
						GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
						GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
						GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
						GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
						GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
						GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
						GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */

						GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
						GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
						GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
						GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
						GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

						// Round 3 
			
						HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
						HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
						HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
						HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
						HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
						HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
						HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
						HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
						HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
						HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
						HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
						HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
						HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
						HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
						HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
						HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

						// Round 4 
			
						II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
						II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
						II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
						II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
						II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
						II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
						II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
						II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
						II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
						II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
						II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
						II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
						II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
						II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
						II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
						II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

						state[0] += a;
						state[1] += b;
						state[2] += c;
						state[3] += d;

						// Zeroize sensitive information.
			 
						MD5_memset((unsigned char *)x, 0, sizeof (x));
					}

				public:
        
					// -------------------------------------------------------------------
					// MD5 initialization. Begins an MD5 operation, writing a new context

					void MD5Init (MD5_CTX *context)
					{
						  context->count[0] = 0;
						  context->count[1] = 0;
						  context->state[0] = 0x67452301;
						  context->state[1] = 0xefcdab89;
						  context->state[2] = 0x98badcfe;
						  context->state[3] = 0x10325476;
					}

					// -----------------------------------------------------------------------------------
					// MD5 block update operation. Continues an MD5 message-digest
					// operation, processing another message block, and updating the
					// context.

					void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputLen)
					{
						unsigned int i, index, partLen;

						// Compute number of bytes mod 64 
						index = (unsigned int)((context->count[0] >> 3) & 0x3F);

						// Update number of bits 
						if((context->count[0] += ((unsigned long int)inputLen << 3)) < ((unsigned long int)inputLen << 3))
							context->count[1]++;

						context->count[1] += ((unsigned long int)inputLen >> 29);
						partLen = 64 - index;

						// Transform as many times as possible.
   
						if(inputLen >= partLen)
						{
    
							MD5_memcpy((unsigned char *)&context->buffer[index], (unsigned char *)input, partLen);
        
							MD5Transform(context->state, context->buffer);

							for(i = partLen; i + 63 < inputLen; i += 64)
								MD5Transform(context->state, &input[i]);

							index = 0;
						}
						  else
							i = 0;

						// Buffer remaining input 
						MD5_memcpy((unsigned char *)&context->buffer[index],(unsigned char *)&input[i],inputLen-i);
					}

					// -----------------------------------------------------------------------
					// MD5 finalization. Ends an MD5 message-digest operation, writing the
					// the message digest and zeroizing the context.
 
					void MD5Final(unsigned char digest[16], MD5_CTX *context)
					{
						unsigned char bits[8];
						unsigned int index, padLen;

						unsigned char Padding[64] =
						{
						  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
						};

						// Save number of bits 
						Encode(bits, context->count, 8);

						// Pad out to 56 mod 64.
     
						index = (unsigned int)((context->count[0] >> 3) & 0x3f);
						padLen = (index < 56) ? (56 - index) : (120 - index);
						MD5Update(context, Padding, padLen);

						// Append length (before padding) 
						MD5Update(context, bits, 8);

						// Store state in digest 
						Encode(digest, context->state, 16);

						// Zeroize sensitive information.
     
						MD5_memset((unsigned char *)context, 0, sizeof(*context));
					}

					// -----------------------------------------------------------------------

					std::string ConvToString(unsigned char *bytes) 
					{
		
							std::string hex;
				
							unsigned int value;
							unsigned int k;
							char *buffer;
							char str[3] = { 0 };
			
							hex.resize(32);
							hex[31]=0;
							k=0;

							for ( size_t i=0; i<16; ++i )
							{

								str[0]='0';
								str[1]='0';
								str[2]='0';

								buffer = str + 2;	 
			
								value=bytes[i];
					
								while ( value )
								{

									*--buffer = "0123456789abcdef"[ value % 16];
						
									value >>= 4;
					
								}
				
								hex[k++]=str[0];
								hex[k++]=str[1];

							}
				
							return hex;

						}
			

					// ----------------------------------------------------------

					std::string GetHashFromString( const std::string text )
					{
						MD5_CTX ctx;

						//init md5
    
						MD5Init(&ctx);
    
						//update with our string
    
						MD5Update(&ctx,(unsigned char*)text.c_str(),int(text.length()));

						//create the hash
    
						unsigned char buff[16] = "";
    
						MD5Final((unsigned char*)buff, &ctx);

						//converte the hash to a string and return it
    
						return ConvToString(buff);

					}

					// ----------------------------------------------------------------------
					// cor / dtor

					 MD5()
					 {
					 }

					~MD5()
					 {
					 }

			};

	} // end of strings namespace

} // end of vml namespace

