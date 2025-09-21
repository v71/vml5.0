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

//////////////////////////////////////////////////////////////////////////////////////
// math conversion

namespace vml
{
	namespace math
	{
		namespace conversion
		{
			// -----------------------------------------------------------------------

			#define COLOR_TO_RGBA(r,g,b,a) ((unsigned int)((((a)&0xFF)<<24)|(((b)&0xFF)<<16)|(((g)&0xFF)<<8)|((r)&0xFF)))
			
			// -----------------------------------------------------------------------
			// Clamp:

			template<typename T>
			static T Clamp(T x, T mn, T mx)
			{
				if (x <= mn)
					return mn;
				else if (x >= mx)
					return mx;
				return x;
			}
			
			// -----------------------------------------------------------------------

			static unsigned char ByteFromHex(const char* s)
			{
				unsigned char ret = 0;
				if (strlen(s) >= 2)
				{
					const char hi = s[0];
					const char lo = s[1];

					if (hi >= '0' && hi <= '9') ret = ((hi - '0')) << 4;
					else if (hi >= 'a' && hi <= 'f') ret = ((hi - 'a') + 10) << 4;
					else if (hi >= 'A' && hi <= 'F') ret = ((hi - 'A') + 10) << 4;

					if (lo >= '0' && lo <= '9') ret |= (lo - '0');
					else if (lo >= 'a' && lo <= 'f') ret |= (lo - 'a') + 10;
					else if (lo >= 'A' && lo <= 'F') ret |= (lo - 'A') + 10;
				}
				return ret;
			}

			// -----------------------------------------------------------------------
			// Single floating-point to integer:

			static unsigned char  UnormToUint8(float x) { return unsigned char(x * UCHAR_MAX + 0.5f); }
			static unsigned short UnormToUint16(float x) { return unsigned short(x * USHRT_MAX + 0.5f); }
			static char			  SnormToSint8(float x) { return char((x >= 0) ? x * SCHAR_MAX + 0.5f : x * SCHAR_MAX - 0.5f); }
			static short		  SnormToSint16(float x) { return short((x >= 0) ? x * SHRT_MAX + 0.5f : x * SHRT_MAX - 0.5f); }
			
			// -----------------------------------------------------------------------
			// Single integer to Floating-point:

			static float Uint8ToUnorm(unsigned char x) { return x * (1.f / UCHAR_MAX); }
			static float Uint16ToUnorm(unsigned short x) { return x * (1.f / USHRT_MAX); }
			static float Sint8ToSnorm(char x) { if (x == SCHAR_MIN) x = SCHAR_MIN + 1; return x * (1.f / SCHAR_MAX); }
			static float Sint16ToSnorm(short x) { if (x == SHRT_MIN) x = SHRT_MIN + 1; return x * (1.f / SHRT_MAX); }

			// -----------------------------------------------------------------------
			// Array of floating-points to integers:

			static void UnormToUint8(const float* pIn, unsigned char* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = UnormToUint8(pIn[i]); }
			static void UnormToUint16(const float* pIn, unsigned short* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = UnormToUint16(pIn[i]); }
			static void SnormToSint8(const float* pIn, char* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = SnormToSint8(pIn[i]); }
			static void SnormToSint16(const float* pIn, short* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = SnormToSint16(pIn[i]); }

			// -----------------------------------------------------------------------
			// Array of integers to floating-points:

			static void Uint8ToUnorm(const unsigned char* pIn, float* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = Uint8ToUnorm(pIn[i]); }
			static void Uint16ToUnorm(const unsigned short* pIn, float* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = Uint16ToUnorm(pIn[i]); }
			static void Sint8ToSnorm(const char* pIn, float* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = Sint8ToSnorm(pIn[i]); }
			static void Sint16ToSnorm(const short* pIn, float* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = Sint16ToSnorm(pIn[i]); }

			// -----------------------------------------------------------------------
			// Integers to integers:

			static short Sint8ToSint16(char x) { return x * SHRT_MAX / SCHAR_MAX; }
			static char  Sint16ToSint8(short x) { return x * SCHAR_MAX / SHRT_MAX; }
			static void  Sint8ToSint16(const char* pIn, short* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = Sint8ToSint16(pIn[i]); }
			static void  Sint16ToSint8(const short* pIn, char* pOut, int count) { for (size_t i = 0; i < count; ++i) pOut[i] = Sint16ToSint8(pIn[i]); }

			// -----------------------------------------------------------------------
			// 4 bits per channel:

			static unsigned short Uint8x4ToUint4x4(unsigned int in)
			{
				int x[4] = { (in >> 28) & 0xF, (in >> 20) & 0xF, (in >> 12) & 0xF, (in >> 4) & 0xF };
				return (x[0] << 12) | (x[1] << 8) | (x[2] << 4) | (x[3]);
			}

			// -----------------------------------------------------------------------

			static unsigned int Uint4x4ToUint8x4(unsigned short in, bool scaleTo255)
			{
				int x[4] = { ((in >> 12) & 0xF) << 4,  ((in >> 8) & 0xF) << 4, ((in >> 4) & 0xF) << 4, ((in >> 0) & 0xF) << 4 };

				if (scaleTo255)
				{
					x[0] = x[0] * 255 / 0xF0;
					x[1] = x[1] * 255 / 0xF0;
					x[2] = x[2] * 255 / 0xF0;
					x[3] = x[3] * 255 / 0xF0;
				}

				return (x[0] << 24) | (x[1] << 16) | (x[2] << 8) | (x[3] << 0);
			}

			// -----------------------------------------------------------------------

			static float SRGBToLinear(float color)
			{
				return color < 0.04045f ? color * (1 / 12.92f) : pow((color + 0.055f) * (1 / 1.055f), 2.4f);
			}

			// -----------------------------------------------------------------------

			static float LinearToSRGB(float color)
			{
				return color < 0.0031308f ? 12.92f * color : 1.055f * pow(color, 1 / 2.4f) - 0.055f;
			}

			// -----------------------------------------------------------------------

			static void SRGBToLinear(float color[])
			{
				color[0] = SRGBToLinear(color[0]);
				color[1] = SRGBToLinear(color[1]);
				color[2] = SRGBToLinear(color[2]);
			}

			// -----------------------------------------------------------------------

			static void LinearToSRGB(float color[])
			{
				color[0] = LinearToSRGB(color[0]);
				color[1] = LinearToSRGB(color[1]);
				color[2] = LinearToSRGB(color[2]);
			}

			// -----------------------------------------------------------------------

			static unsigned int Color16To32(unsigned short in)
			{
				const int b = ((in >> 0) & 0x1F) << 3;
				const int g = ((in >> 5) & 0x3F) << 2;
				const int r = ((in >> 11) & 0x1F) << 3;
			
				return COLOR_TO_RGBA(r, g, b, 255);
			}

			// -----------------------------------------------------------------------

			static void ColorInt32ToFloat(unsigned int in, float* out, bool sRGB)
			{
				out[0] = ((in >> 0) & 0xFF) * (1 / 255.f);
				out[1] = ((in >> 8) & 0xFF) * (1 / 255.f);
				out[2] = ((in >> 16) & 0xFF) * (1 / 255.f);
				out[3] = ((in >> 24) & 0xFF) * (1 / 255.f);
				
				if (sRGB)
				{
					out[0] = SRGBToLinear(out[0]);
					out[1] = SRGBToLinear(out[1]);
					out[2] = SRGBToLinear(out[2]);
				}
			}

			// -----------------------------------------------------------------------

			static unsigned int ColorFloatToInt32(const float* in, bool sRGB)
			{
				float out[4] = { in[0],in[1],in[2],in[3] };

				if (sRGB)
				{
					out[0] = LinearToSRGB(out[0]);
					out[1] = LinearToSRGB(out[1]);
					out[2] = LinearToSRGB(out[2]);
				}

				const int r = Clamp(static_cast<int>(out[0] * 255 + 0.5f), 0, 255);
				const int g = Clamp(static_cast<int>(out[1] * 255 + 0.5f), 0, 255);
				const int b = Clamp(static_cast<int>(out[2] * 255 + 0.5f), 0, 255);
				const int a = Clamp(static_cast<int>(out[3] * 255 + 0.5f), 0, 255);

				return COLOR_TO_RGBA(r, g, b, a);
			}

			// -----------------------------------------------------------------------

			static void ColorTextToFloat4(const char* sz, float* out, bool sRGB)
			{
				if (sz && '0' == sz[0] && 'x' == sz[1])
					sz += 2;
			
				int color[4] = {};

				if (!sz)
				{
					// nothignn here to do 
				}
				else if (6 == strlen(sz) && !strchr(sz, ' '))
				{
					color[0] = ByteFromHex(sz + 0);
					color[1] = ByteFromHex(sz + 2);
					color[2] = ByteFromHex(sz + 4);
					color[3] = 255;
				}
				else if (8 == strlen(sz) && !strchr(sz, ' '))
				{
					color[0] = ByteFromHex(sz + 0);
					color[1] = ByteFromHex(sz + 2);
					color[2] = ByteFromHex(sz + 4);
					color[3] = ByteFromHex(sz + 6);
				}
				else
				{
					const int count = sscanf_s(sz, "%d %d %d %d", color + 0, color + 1, color + 2, color + 3);
					if (3 == count)
						color[3] = 255;
				}

				out[0] = color[0] * (1 / 255.f);
				out[1] = color[1] * (1 / 255.f);
				out[2] = color[2] * (1 / 255.f);
				out[3] = color[3] * (1 / 255.f);

				if (sRGB)
				{
					out[0] = SRGBToLinear(out[0]);
					out[1] = SRGBToLinear(out[1]);
					out[2] = SRGBToLinear(out[2]);
				}
			}

			// -----------------------------------------------------------------------

			static unsigned int ColorTextToInt32(const char* sz)
			{
				if (sz && '0' == sz[0] && 'x' == sz[1])
					sz += 2;
				
				int color[4] = {};
				
				if (!sz)
				{
				}
				else if (6 == strlen(sz) && !strchr(sz, ' '))
				{
					color[0] = ByteFromHex(sz + 0);
					color[1] = ByteFromHex(sz + 2);
					color[2] = ByteFromHex(sz + 4);
					color[3] = 255;
				}
				else if (8 == strlen(sz) && !strchr(sz, ' '))
				{
					color[0] = ByteFromHex(sz + 0);
					color[1] = ByteFromHex(sz + 2);
					color[2] = ByteFromHex(sz + 4);
					color[3] = ByteFromHex(sz + 6);
				}
				else
				{
					sscanf_s(sz, "%d %d %d %d", color + 0, color + 1, color + 2, color + 3);
				}
				
				return COLOR_TO_RGBA(color[0], color[1], color[2], color[3]);
			}

			// -----------------------------------------------------------------------

			static unsigned short QuantizeFloat(float f, float mn, float mx)
			{
				const float range = mx - mn;
				return unsigned short(Clamp<float>((f - mn) / range * USHRT_MAX + 0.5f, 0, USHRT_MAX));
			}

			// -----------------------------------------------------------------------

			static float DequantizeFloat(unsigned short i, float mn, float mx)
			{
				const float range = mx - mn;
				return float(i) / USHRT_MAX * range + mn;
			}

			// -----------------------------------------------------------------------

			static unsigned char QuantizeFloatToByte(float f, float mn, float mx)
			{
				const float range = mx - mn;
				return unsigned char(Clamp<float>((f - mn) / range * UCHAR_MAX + 0.5f, 0, UCHAR_MAX));
			}

			// -----------------------------------------------------------------------

			static float DequantizeFloatFromByte(unsigned char i, float mn, float mx)
			{
				const float range = mx - mn;
				return float(i) / UCHAR_MAX * range + mn;
			}
			
		} // end of conversion namespace

	} // end of math namespace

} // end of vml namespace
