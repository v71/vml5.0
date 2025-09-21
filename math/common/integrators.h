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

///////////////////////////////////////////////////////////////////////////////////////////////
// example
//
// define a function like this :
//
//	 double Linear( double x)
//	{
//			return x;
//	}
//
// then call an integration function like this:
//
// a = vml::math::integrators::Simpson(Linear, 2, 3, vml::math::EPSILON);
//
// this is a simple wrapper, for the complete function see source code below

namespace vml
{
	namespace math
	{

		namespace integrators
		{

			// ------------------------------------------------------------------------------------
			// pointer to single variable type

			typedef double(*func)(double);

			// ------------------------------------------------------------------------------------
			// Full interface. Gives a posteriori information, namely the mumber of function evaluations and a conservative estimate of the error.
			// "f" integrand
			// "a" left limit of integration 
			// "b" right limit of integration 
			// "desiredRelativeError" relative error tolerance 
			// "log2MaxFunctionEvals" If this value is N, the number of function evaluations will be less than 2^n + 1
			// "functionEvalsUsed" Actual number of function evaluations used
			// "estimatedError" Conservative error estimate
			// returns integral value

			static double Simpson(func f, double a, double b, double relativeErrorTolerance, int log2MaxFunctionEvals, int &functionEvalsUsed, double &estimatedError)
			{
				double integral;
				double mostRecentContribution;
				double previousContribution;
				double previousIntegral;
				double sum;
				double x, h;
				int numNewPts;

				mostRecentContribution = 0.0;
				previousContribution = 0.0;
				previousIntegral = 0.0;
				estimatedError = DBL_MAX;
				sum = f(a) + f(b);
				functionEvalsUsed = 2;
				integral = sum * 0.5 * (b - a);

				for (int stage = 0; stage < log2MaxFunctionEvals; stage++)
				{

					// Pattern of Simpson's rule coefficients:
					//
					// 1               1
					// 1       4       1
					// 1   4   2   4   1
					// 1 4 2 4 2 4 2 4 1
					// ...
					//
					// Each row multiplies new function evaluations by 4, and the evalutations from the previous step by 2.

					numNewPts = 1 << (stage - 1);

					mostRecentContribution = 0.0;

					h = (b - a) / numNewPts;

					x = a + 0.5 * h;

					for (int i = 0; i < numNewPts; i++)
						mostRecentContribution += f(x + i * h);

					functionEvalsUsed += numNewPts;

					mostRecentContribution *= 4.0;

					sum += mostRecentContribution - 0.5 * previousContribution;

					integral = sum * (b - a) / (( 1<<stage) * 3.0);

					// Require at least five stages to reduce the risk of incorrectly declaring convergence too soon.
					// Note that you can specify fewer stages, but the early termination rule below will not be used.

					if (stage >= 5)
					{
						estimatedError = fabs(integral - previousIntegral); // conservative
						if (estimatedError <= relativeErrorTolerance * fabs(previousIntegral))
							return integral;
					}

					previousContribution = mostRecentContribution;
					previousIntegral = integral;

				}

				return integral;
			}

			// ------------------------------------------------------------------------------------
			// Full interface. Gives a posteriori information, namely the mumber of function evaluations and a conservative estimate of the error.
			// "f" integrand
			// "a" left limit of integration 
			// "b" right limit of integration 
			// "desiredRelativeError" relative error tolerance 
			// "log2MaxFunctionEvals" If this value is N, the number of function evaluations will be less than 2^n + 1
			// "functionEvalsUsed" Actual number of function evaluations used
			// "estimatedError" Conservative error estimate
			// returns integral value

			static double Trapezoid(func f, double a, double b, double relativeErrorTolerance, int log2MaxFunctionEvals, int &functionEvalsUsed, double &estimatedError)
			{
				double previousIntegral;
				double x, h;
				double integral;

				functionEvalsUsed = 0;
				estimatedError = DBL_MAX;
				previousIntegral = 0.0;
				if (log2MaxFunctionEvals == 0) h = (b - a);
				else h = (b - a) / (double)log2MaxFunctionEvals;
				integral = 0.5*(f(a) + f(b));
				x = a;

				for (int stage = 1; stage<log2MaxFunctionEvals; ++stage)
				{
					x += h;
					integral += f(x);

					// Require at least five stages to reduce the risk of incorrectly declaring convergence too soon.
					// Note that you can specify fewer stages, but the early termination rule below will not be used.

					if (stage >= 5)
					{
						estimatedError = fabs(integral - previousIntegral); // conservative
						if (estimatedError <= relativeErrorTolerance * fabs(previousIntegral))
							return h*integral;
					}

					functionEvalsUsed++;
					previousIntegral = integral;

				}

				return h*integral;
			}

			// ------------------------------------------------------------------------------------
			// Full interface. Gives a posteriori information, namely the mumber of function evaluations and a conservative estimate of the error.
			// "f" integrand
			// "a" left limit of integration 
			// "b" right limit of integration 
			// "desiredRelativeError" relative error tolerance 
			// "log2MaxFunctionEvals" If this value is N, the number of function evaluations will be less than 2^n + 1
			// "functionEvalsUsed" Actual number of function evaluations used
			// "estimatedError" Conservative error estimate
			// returns integral value

			static double MidPoint(func f, double a, double b, double relativeErrorTolerance, int log2MaxFunctionEvals, int &functionEvalsUsed, double &estimatedError)
			{
				double previousIntegral;
				double x, h;
				double integral;

				functionEvalsUsed = 0;
				estimatedError = DBL_MAX;
				previousIntegral = 0.0;

				if (log2MaxFunctionEvals == 0) h = (b - a);
				else h = (b - a) / (double)log2MaxFunctionEvals;

				x = a + 0.5*h;
				integral = f(x);

				for (int stage = 1; stage<log2MaxFunctionEvals; ++stage)
				{
					x += h;
					integral += f(x);

					// Require at least five stages to reduce the risk of incorrectly declaring convergence too soon.
					// Note that you can specify fewer stages, but the early termination rule below will not be used.

					if (stage >= 5)
					{
						estimatedError = fabs(integral - previousIntegral); // conservative
						if (estimatedError <= relativeErrorTolerance * fabs(previousIntegral))
							return h*integral;
					}

					functionEvalsUsed++;
					previousIntegral = integral;

				}

				return h*integral;

			}

			// ------------------------------------------------------------------------------------
			// Simplest interface, returning no diagnostic data
			// "f" integrand
			// "a" left limit of integration
			// "b" right limit of integration
			// "desiredRelativeError" relative error tolerance
			// returns Integral value

			static double Simpson(func f, double a, double b, double desiredRelativeError)
			{
				int log2MaxFunctionEvals = 10;						// default number of iterations
				int functionEvalsUsed = 0;							// redundant assignement , done for completeness
				double estimatedError = vml::math::EPSILON;
				return Simpson(f, a, b, desiredRelativeError, log2MaxFunctionEvals, functionEvalsUsed, estimatedError);
			}

			// ------------------------------------------------------------------------------------
			// Simplest interface, returning no diagnostic data
			// "f" integrand
			// "a" left limit of integration
			// "b" right limit of integration
			// "desiredRelativeError" relative error tolerance
			// returns Integral value

			static double Trapezoid(func f, double a, double b, double desiredRelativeError)
			{
				int log2MaxFunctionEvals = 10;						// default number of iterations
				int functionEvalsUsed = 0;							// redundant assignement , done for completeness
				double estimatedError = vml::math::EPSILON;
				return Trapezoid(f, a, b, desiredRelativeError, log2MaxFunctionEvals, functionEvalsUsed, estimatedError);
			}

			// ------------------------------------------------------------------------------------
			// Simplest interface, returning no diagnostic data
			// "f" integrand
			// "a" left limit of integration
			// "b" right limit of integration
			// "desiredRelativeError" relative error tolerance
			// returns Integral value

			static double MidPoint(func f, double a, double b, double desiredRelativeError)
			{
				int log2MaxFunctionEvals = 10;						// default number of iterations
				int functionEvalsUsed = 0;							// redundant assignement , done for completeness
				double estimatedError = vml::math::EPSILON;
				return MidPoint(f, a, b, desiredRelativeError, log2MaxFunctionEvals, functionEvalsUsed, estimatedError);
			}

		}
	}
}


