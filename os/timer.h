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

namespace vml
{

	namespace os
	{

			///////////////////////////////////////////////////////////
			// timing library

			class Timer
			{
					bool		  PerformanceTimer;
					float		  TimeAtStart;
					float		  TimeSinceStart;
					float		  OldTimeSinceStart;
					float		  LastUpdate;
					float		  FpsUpdateInterval;
					float		  Fps;
					unsigned int  NumFrames;
					__int64		  Frequency;

				public:

					///////////////////////////////////////////////////////////////////////////////////////////////
					// Called every time you need the current game time

					float GetElapsedTime() const
					{
						__int64 ticks=0;
						// This is the number of clock ticks since start
						if (!QueryPerformanceCounter((LARGE_INTEGER *)&ticks))
							ticks = (UINT64)timeGetTime();
						// Divide by frequency to get the time in seconds
						float time = (float)(__int64)ticks / (float)(__int64)Frequency;
						// Subtract the time at game start to get
						// the time since the game started
						time -= TimeAtStart;
						return time;
					}

					///////////////////////////////////////////////////////////////////////////////////////////////
					// get the time interval in milliseconds

					float GetDt() 
					{
						__int64 ticks=0;
						// This is the number of clock ticks since start
						if (!QueryPerformanceCounter((LARGE_INTEGER *)&ticks))
							ticks = (UINT64)timeGetTime();
						TimeSinceStart = (float)(__int64)ticks / (float)(__int64)Frequency;
						float deltaTime = TimeSinceStart - OldTimeSinceStart;
						OldTimeSinceStart = TimeSinceStart;
						return deltaTime;
					}

					/////////////////////////////////////////////////////////////////////
					// Called once for every frame

					float GetFPS() 
					{
						NumFrames++;

						__int64 ticks=0;

						// This is the number of clock ticks since start

						if (!QueryPerformanceCounter((LARGE_INTEGER *)&ticks))
							ticks = (UINT64)timeGetTime();

						// Divide by frequency to get the time in seconds

						float currentUpdate = (float)(__int64)ticks / (float)(__int64)Frequency;

						// Subtract the time at game start to get
						// the time since the game started

						currentUpdate -= TimeAtStart;

						if (currentUpdate - LastUpdate > FpsUpdateInterval)
						{

							Fps = NumFrames / (currentUpdate - LastUpdate);
							LastUpdate = currentUpdate;
							NumFrames = 0;
						}

						return Fps;
					}

					///////////////////////////////////////////////////////////////////////
					//  initialise timer

					void Init()
					{
						// Check To See If A Performance Counter Is Available
						// If One Is Available The Timer Frequency Will Be Updated

						if (!QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency))
						{
							Frequency = 1000;
							TimeAtStart = 0.0f;
							TimeAtStart = GetElapsedTime();
							PerformanceTimer = FALSE;
						}
						else
						{
							// If timeAtGameStart is 0 then we get the time since
							// the start of the computer when we call GetGameTime()
							TimeAtStart = 0.0f;
							TimeAtStart = GetElapsedTime();
							PerformanceTimer = TRUE;
						}

						// initialise deltas 

						GetDt();
					}

					///////////////////////////////////////////////////////////////////////
					// query functions

					const __int64 GetFrequency() const { return Frequency; }
					const float GetResolution() const { return 1000.0f / (float)Frequency; }
					const bool HasPerformanceCounter() const { return PerformanceTimer; }

					//////////////////////////////////////////////////////////////////////
					// ctor / dtor

					Timer()
					{
						Frequency         = 0;
						LastUpdate        = 0.0f;
						FpsUpdateInterval = 0.5f;
						NumFrames         = 0;
						Fps               = 0.0f;
						PerformanceTimer  = false;
						TimeAtStart       = 0.0f;
						TimeSinceStart    = 0.0f;
						OldTimeSinceStart = 0.0f;
					}

					~Timer()
					{
					}

			};

	} // end of os namespace

} // end of vml namespace
