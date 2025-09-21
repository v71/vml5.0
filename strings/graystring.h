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

///////////////////////////////////////////////////////////////////////////
// small library to generate gray binary string
// and conversions

namespace vml
{
	namespace strings
	{

		class GrayString
		{

			public:

				//--------------------------------------------------------------------
				// This function generates all n bit Gray codes and prints the
				// generated codes

				static const std::vector<std::string> GenerateGrayArray(int n, bool mode = 0)
				{
					// 'arr' will store all generated codes

					std::vector<std::string> arr;

					// base case

					if (n <= 0)
						return arr;

					// start with one-bit pattern

					if (!mode)
					{
						arr.emplace_back("0");
						arr.emplace_back("1");
					}
					else
					{
						arr.emplace_back("1");
						arr.emplace_back("0");
					}

					// Every iteration of this loop generates 2*i codes from previously
					// generated i codes.

					for (int i = 2; i < (1 << n); i = i << 1)
					{
						// Enter the prviously generated codes again in arr[] in reverse
						// order. Nor arr[] has double number of codes.

						for (int j = i - 1; j >= 0; j--)
							arr.emplace_back(arr[j]);

						// append 0 to the first half

						for (int j = 0; j < i; j++)
							arr[j] = "0" + arr[j];

						// append 1 to the second half

						for (int j = i; j < 2 * i; j++)
							arr[j] = "1" + arr[j];
					}

					// print contents of arr[]

			//		for (int i = 0; i < arr.size(); i++)
			//			std::cout << arr[i] << std::endl;

					return arr;
				}

				//--------------------------------------------------------------------
				//  function to convert binary string to gray string

				static const std::string BinarytoGray(const std::string &binary)
				{
					std::string gray = "";

					//  MSB of gray code is same as binary code

					gray += binary[0];

					// Compute remaining bits, next bit is comuted by
					// doing XOR of previous and current in Binary

					for (size_t i = 1; i < binary.length(); i++)
					{
						// Concatenate XOR of previous bit with current bit

						gray += binary[i - 1] == binary[i] ? '0' : '1';

					}

					return gray;
				}

				//--------------------------------------------------------------------
				// function to convert gray code string to binary string

				static const std::string GraytoBinary(const std::string &gray)
				{
					std::string binary = "";

					//  MSB of binary code is same as gray code

					binary += gray[0];

					// Compute remaining bits

					for (size_t i = 1; i < gray.length(); i++)
					{
						// If current bit is 0, concatenate previous bit

						if (gray[i] == '0')
						{
							binary += binary[i - 1];
						}
						else
						{
							// Else, concatenate the inversion of previous bit

							binary += (binary[i - 1] == '0') ? '1' : '0';
						}
					}

					return binary;
				}

				//--------------------------------------------------------------------
				// ctor / dtor

				GrayString()
				{}

				~GrayString()
				{}

			};

	}
}
