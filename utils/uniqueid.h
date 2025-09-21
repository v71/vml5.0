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


////////////////////////////////////////////////////////////////////////////////////
// bits utils

namespace vml
{
	namespace utils
	{

		class UUIDGenerator
		{
			private:

				struct UUID
				{
					unsigned Node;					 // 16 bits
					unsigned long long TimeEpoch;    // 52 bits
					unsigned long long RandomNumber; // 60 bits
				};

			private:

				// --------------------------------------------------------------
				//Set the current epoch in nanoseconds from January 1 1970

				static void SetTimeEpoch(UUID& uuid)
				{
					std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch());
					uuid.TimeEpoch = ns.count() & 0xFFFFFFFFFFFFF;
				}

				// --------------------------------------------------------------
				// set random number generator

				static void SetRandomNumber(UUID& uuid, std::mt19937& PRNG)
				{
					std::uniform_int_distribution<uint32_t> uint_dist_long(0, UINT32_MAX);
					uuid.RandomNumber = uint_dist_long(PRNG);
					uuid.RandomNumber <<= 32;
					uuid.RandomNumber |= uint_dist_long(PRNG);
					uuid.RandomNumber &= 0xFFFFFFFFFFFFFFF;
				}

				// --------------------------------------------------------------
				// This number is currently being randomly generated. In a distributed environment this would be known before-hand 

				static void SetNodeNumber(UUID& uuid, std::mt19937& PRNG)
				{
					std::uniform_int_distribution<unsigned> uint_dist_long(0, UINT16_MAX);
					uuid.Node = uint_dist_long(PRNG) & 0xFFFF;
				}

			public:

				// --------------------------------------------------------------
				// generate an uinque string id

				static std::string GenerateUUID(bool bPrettyPrint = true)
				{
					static std::random_device seed;
					static std::mt19937 PRNG(seed());

					//Initialize various fields of UUID structure

					UUID uuid = { 0 };

					SetRandomNumber(uuid, PRNG);
					SetTimeEpoch(uuid);
					SetNodeNumber(uuid, PRNG);

					std::stringstream ssUUID;

					ssUUID << std::hex << std::setfill('0');

					ssUUID << std::setw(15) << uuid.RandomNumber; // 15 hex digits = 60 bit binary number
					ssUUID << std::setw(13) << uuid.TimeEpoch;
					ssUUID << std::setw(4) << uuid.Node;

					return bPrettyPrint ? (ssUUID.str().insert(8, 1, '-').insert(13, 1, '-').insert(18, 1, '-').insert(23, 1, '-')) : ssUUID.str();
				}

				// --------------------------------------------------------------
				// ctor / dtor

				UUIDGenerator()
				{
				}

				~UUIDGenerator()
				{
				}

		};

	}	// end of utils namesapce

} // end of namespace vml

