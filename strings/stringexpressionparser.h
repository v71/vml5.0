#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2020 v71 
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

/////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace strings
	{

			class ExpressionParser
			{
				private:
					
					// ---------------------------------------------
					// 

					size_t       Pos;
					double       Answer;
					unsigned int LastError;
					int          ParenCount;
					const char  *ErrPos;

					// ---------------------------------------------
					// 

					static const unsigned int EXPR_NO_ERR				  = 0;
					static const unsigned int EXPR_UNBALANCED_PARENTHESES = 1;
					static const unsigned int EXPR_DIVISION_BY_ZERO		  = 2;
					static const unsigned int EXPR_UNKNOWN_SYMBOL		  = 3;

					// ---------------------------------------------
					// Parse a number or an expression in parenthesis

					double ParseAtom(const char* &expr)
					{
						// Handle the sign before parenthesis (or before number)

						bool negative = false;

						if (*expr == '-')
						{
							negative = true;
							expr++;
						}

						if (*expr == '+')
						{
							expr++;
						}

						// Check if there is parenthesis

						if (*expr == '(')
						{
							expr++;

							ParenCount++;

							double res = ParseSummands(expr);

							if (*expr != ')')
							{
								// Unmatched opening parenthesis

								LastError = EXPR_UNBALANCED_PARENTHESES;
								ErrPos = expr;
								return std::numeric_limits<double>::quiet_NaN();
							}

							expr++;

							ParenCount--;

							return negative ? -res : res;
						}

						// It should be a number; convert it to double

						char* end_ptr;

						double res = strtod(expr, &end_ptr);

						if (end_ptr == expr)
						{
							// Report error

							LastError = EXPR_UNKNOWN_SYMBOL;
							ErrPos = expr;
							return std::numeric_limits<double>::quiet_NaN();
						}

						// Advance the pointer and return the result

						expr = end_ptr;

						return negative ? -res : res;
					}
					
					// ---------------------------------------------
					// Parse multiplication and division

					double ParseFactors(const char* &expr)
					{
						double num1 = ParseAtom(expr);

						for (;;)
						{
							// Save the operation and position

							char op = *expr;

							const char* pos = expr;

							if (op != '/' && op != '*' && op!='^')
								return num1;

							expr++;

							double num2 = ParseAtom(expr);

							// Perform the saved operation

							if ( op == '/' )
							{

								// Handle division by zero

								if (num2 == 0)
								{
									LastError = EXPR_DIVISION_BY_ZERO;
									ErrPos = pos;
									return std::numeric_limits<double>::quiet_NaN();
								}

								num1 /= num2;

							}
						
							if ( op == '*' )
							{
								num1 *= num2;
							}

							if ( op == '^' )
							{
								num1 = pow(num1,num2);
							}
						}
					}

					// ---------------------------------------------
					// Parse addition and subtraction

					double ParseSummands(const char* &expr)
					{
						double num1 = ParseFactors(expr);

						for (;;)
						{
							char op = *expr;

							if (op != '-' && op != '+')
								return num1;

							expr++;

							double num2 = ParseFactors(expr);

							if (op == '-')
								num1 -= num2;
							else
								num1 += num2;
						}
					}

					// ---------------------------------------------
					//

					double Eval(const char* expr)
					{
				
						double res = ParseSummands(expr);

						// Now, expr should point to '\0', and _paren_count should be zero

						if (ParenCount != 0 || *expr == ')')
						{
							LastError = EXPR_UNBALANCED_PARENTHESES;
							ErrPos = expr;
							return std::numeric_limits<double>::quiet_NaN();
						}

						if (*expr != '\0')
						{
							LastError = EXPR_UNKNOWN_SYMBOL;
							ErrPos = expr;
							return std::numeric_limits<double>::quiet_NaN();
						}

						return res;
					};

				public:

					// ---------------------------------------------------------
					// parse an expression contained in a ';' terminated string

					bool Parse(const std::string &expression)
					{
						ParenCount = 0;
						LastError = EXPR_NO_ERR;
						ErrPos = nullptr;
						Answer=Eval(&expression[0]);
						if (LastError == EXPR_NO_ERR) return true;
						return false;
					}

					// ---------------------------------------------
					// 

					unsigned int GetLastError() const 
					{
						return LastError; 
					}
					
					double GetAnswer() const 
					{
						return Answer; 
					}
					
					const char* GetErrPos()	const
					{
						return ErrPos; 
					}
					
					const std::string GetLastErrorString() const 
					{
						switch (LastError)
						{
							case EXPR_NO_ERR				 : return "No Error";				break;
							case EXPR_UNBALANCED_PARENTHESES : return "Unbalanced Parentheses"; break;
							case EXPR_DIVISION_BY_ZERO		 : return "Division By Zero ";		break;
							case EXPR_UNKNOWN_SYMBOL		 : return "Unknown Symbol";			break;
						}
						
						return "How did you get here ?";
					
					}

					// ---------------------------------------------
					// ctor / dtor

					ExpressionParser()
					{
						Pos        = 0;
						ParenCount = 0;
						LastError  = EXPR_NO_ERR;
						ErrPos     = nullptr;
						Answer     = std::numeric_limits<double>::quiet_NaN();
					}

					~ExpressionParser()
					{
					}
			};
	}
}

