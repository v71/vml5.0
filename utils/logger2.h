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
    namespace logger
    {
        // ----------------------------------------------------------------------------------
        // log verbose modes flags

        enum class VerboseModes : std::uint8_t
        {
            QUIET   = 0,
            VERBOSE = 1,
        };

        // ----------------------------------------------------------------------------------
        // log message type

        enum class LogLevels : std::uint8_t
        {
            LEVEL_INFO    = 0,
            LEVEL_WARNING = 1,
            LEVEL_ERROR   = 2,
        };

        // ----------------------------------------------------------------------------------
        // log modes flags

        enum class LogModes : std::uint8_t
        {
            TO_STD  = 0,
            TO_FILE = 1,
            TO_MEM  = 2,
        };

        ////////////////////////////////////////////////////////////////////////
        // Log messages

        class LogMessage
        {
                std::string Text;
                LogLevels LogLevel;
            public:
                const std::string& GetText() const { return Text; }
                const LogLevels& GetLogLevel() const { return LogLevel; }
                LogMessage(const std::string& text, const LogLevels& lv) : Text(text), LogLevel(lv) {}
                ~LogMessage() {}
        };

        ////////////////////////////////////////////////////////////////////////

        class Logger2
        {

            private:
                
                ////////////////////////////////////////////////////////////////////////
                // Column class

                class Column
                {
                    public:
                        std::string Title;
                        uint32_t Width;
                        Column(const std::string& title, const uint32_t width) :Title(title), Width(width) {}
                        ~Column() {}
                };
                
                // ---------------------------------------------
                // singleton design pattern
                // A singlton is a class instantiated only once.
                // Usually it is globally accessiblae,
                // constructor and destructors are kept private to the 
                // final user. User has the responsability to delete
                // memory associated with the class explicitly at
                // main entry point closure

                static Logger2* Singleton;		  // Singleton pointer
                
                // ------------------------------------------------------------------

                std::vector<Column>   Columns;                  // Columns
                char                  ColumnSeparator;          // character fro columns separator
                char                  SpacingSeparator;         // character for spacing
                std::list<LogMessage> LogLines;                 // log message list
                uint32_t              LineNumber;               // Line number count
                uint32_t              MaxNumberDigits;          // maxmimn number of digits
                uint32_t              MaxHistory;               // maximum number of lines
                bool                  Initialized;              // initialisation flag
                VerboseModes          VerboseMode;              // verbose mode
                LogModes              LogMode;                  // logging mode
                std::string			  FileName;		            // string holding log's filename
                std::ofstream		  FileStream;		        // file stream
                std::mutex            Lock;                     // mutex lock

                // ------------------------------------------------------------------
                // get string line number

                std::string GetLineNumberStr()
                {
                    return std::format("[ {:0{}} ]", LineNumber, MaxNumberDigits);
                }

                // ---------------------------------------------------------------------------------
                // formatted string holding timing

                [[nodiscard]] const std::string GetFormatTime() const
                {
                    const std::chrono::time_point<std::chrono::system_clock>& time = std::chrono::system_clock::now();
                    const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(time);
                    const auto fraction = time - seconds;
                    std::tm t{};
                    const std::time_t timer = std::chrono::system_clock::to_time_t(time);
                    #if defined(__unix__)
                        localtime_r(&timer, &t);
                    #elif defined(_MSC_VER)
                        localtime_s(&t, &timer);
                    #else
                        static_assert(false)
                    #endif
                        const long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(fraction).count();
                    if (t.tm_hour == 1 && t.tm_min == 0 && t.tm_sec == 0 && millis == 0)
                        return "Not available";
                    const std::string hour = t.tm_hour > 9 ? std::to_string(t.tm_hour) : "0" + std::to_string(t.tm_hour);
                    const std::string min = t.tm_min > 9 ? std::to_string(t.tm_min) : "0" + std::to_string(t.tm_min);
                    const std::string sec = t.tm_sec > 9 ? std::to_string(t.tm_sec) : "0" + std::to_string(t.tm_sec);
                    std::string millisecond;
                    if (millis >= 0 && millis < 10) {
                        millisecond = "00" + std::to_string(millis);
                    }
                    else if (millis > 9 && millis < 100) {
                        millisecond = "0" + std::to_string(millis);
                    }
                    else {
                        millisecond = std::to_string(millis);
                    }
                    const int mon = t.tm_mon + 1;
                    const std::string year = std::to_string(t.tm_year + 1900);
                    const std::string month = mon > 9 ? std::to_string(mon) : "0" + std::to_string(mon);
                    const std::string day = t.tm_mday > 9 ? std::to_string(t.tm_mday) : "0" + std::to_string(t.tm_mday);
                    return "[ " + year + "-" + month + "-" + day + " " + hour + ":" + min + ":" + sec + ":" + millisecond + " ]";
                }

                // ------------------------------------------------------------------
                // truncate a stirng if exceeds a fiuxed amount of space 

                std::string TruncateIf(const std::string& src, uint32_t width) const
                {
                    size_t n = src.size();
                    if (n == 0)
                        vml::os::Message::Error("text size is null");
                    if (n <= width)
                    {
                        std::string text = src;
                        text.append( width - n , SpacingSeparator);
                        return text;
                    }
                    return src.substr(0, (size_t)width - 3) + "...";
                }
                
                // ------------------------------------------------------------------
                // private ctor / dtor

                Logger2()
                {
                    Singleton        = nullptr;
                    ColumnSeparator  = ' ';
                    SpacingSeparator = ' ';
                    LineNumber       = 0;
                    MaxNumberDigits  = 0;
                    MaxHistory       = 0;
                    Initialized      = false;
                    VerboseMode      = VerboseModes::VERBOSE;
                    LogMode          = LogModes::TO_STD;
                    FileName         = "";
                }
                
            public:
                
                //---------------------------------------------------------------------
                // get instance of singleton , if singleton is null , which is mandatory at
                // application startup, it will be allocated only once
                // Eventually, to reallocate , user must call the delete member function

                static Logger2* GetInstance()
                {
                    if (Singleton == nullptr)
                        Singleton = new Logger2();
                    return Singleton;
                }
                
                // ------------------------------------------------------------------
                // add column

                bool AddColumn(const std::string& title, const uint32_t width)
                {
                    if (!Initialized)
                        vml::os::Message::Error("Logger is not initialized");

                    for (auto it = Columns.begin(); it != Columns.end(); ++it)
                        if ((*it).Title == title)
                            vml::os::Message::Error("Column title must be unique");
                    Columns.emplace_back(Column(title, width));
                    return true;
                }
                
                // ---------------------------------------------------------------------------------

                void Log(const std::vector<std::string>& line, const LogLevels& lvl)
                {
                    if (!Initialized)
                        vml::os::Message::Error("Logger is not initialized");

                    if (Columns.size() == 0)
                        vml::os::Message::Error("Columns count is zero");

                    if (VerboseMode == VerboseModes::QUIET)
                        return;

                    size_t n = line.size();

                    // validate message number contatined in a single line

                    if (n == 0)
                        vml::os::Message::Error("zero line");
                    if (n > Columns.size())
                        vml::os::Message::Error("Exceeded columns number");
                    if (n < Columns.size())
                        vml::os::Message::Error("Not enough messages");

                    // cycle past end of log lines

                    if (LineNumber >= MaxHistory)
                        LogLines.erase(LogLines.begin());

                    // compose string
                    // add line number string 

                    std::string text = GetLineNumberStr() + ColumnSeparator;

                    // add format time string

                    text += GetFormatTime() + ColumnSeparator;

                    // add string log level

                    if (lvl == LogLevels::LEVEL_ERROR)   text += "[ E ] : ";
                    if (lvl == LogLevels::LEVEL_INFO)    text += "[ I ] : ";
                    if (lvl == LogLevels::LEVEL_WARNING) text += "[ W ] : ";

                    size_t i = 0;
                    do
                    {
                        text += TruncateIf(line[i], Columns[i].Width) + ColumnSeparator;
                        ++i;
                    } while (i < Columns.size() - 1);
                    text += line[i];

                    switch (LogMode)
                    {
                        case LogModes::TO_STD:
                            std::cout << text << "\n";
                        break;

                        case LogModes::TO_MEM:
                            Lock.lock();
                            LogLines.emplace_back(LogMessage(text, lvl));
                            Lock.unlock();
                        break;

                        case LogModes::TO_FILE:
                            Lock.lock();
                            FileStream << text << "\n";
                            FileStream.flush();
                            Lock.unlock();
                        break;

                    }

                    LineNumber++;
                }

                // ------------------------------------------------------------

                void Error(const std::vector<std::string>& line)
                {
                    Log(line, LogLevels::LEVEL_ERROR);
                }

                void Info(const std::vector<std::string>& line)
                {
                    Log(line, LogLevels::LEVEL_INFO);
                }

                void Warn(const std::vector<std::string>& line)
                {
                    Log(line, LogLevels::LEVEL_WARNING);
                }

                // ------------------------------------------------------------
                // closes logger

                void Close()
                {
                    if (!IsInitialized())
                        vml::os::Message::Error("Logger : Logger not initted");

                    // clear flags
                    Initialized = false;

                    // close file stream
                    if (LogMode == LogModes::TO_FILE)
                        FileStream.close();

                    // delete singleton
                    vml::os::SafeDelete(Singleton);
                }

                // ------------------------------------------------------------------

                void ClearMessages()
                {
                    if (!Initialized)
                        vml::os::Message::Error("Logger is not initialized");

                    LogLines.clear();
                    LineNumber = 0;

                    // clear file is log mode is set to file
                    if (LogMode == LogModes::TO_FILE) {
                        FileStream.open(FileName, std::ios::out | std::ios::trunc);
                        FileStream.close();
                    }
                }

                // ------------------------------------------------------------------
                // Initialize logger

                void Init(const LogModes& logmode, const VerboseModes& verbosemode, const std::string& filename = "")
                {
                    if (Initialized)
                        vml::os::Message::Error("Logger already initialized");

                    Initialized = true;
                    LineNumber  = 0;
                    MaxHistory  = 9999;
                    VerboseMode = verbosemode;
                    LogMode     = logmode;

                    // compute the maxmim number of digits for
                    // printing the line number

                    MaxNumberDigits = floor(log10(MaxHistory) + 1);

                    switch (LogMode)
                    {
                        case LogModes::TO_STD:
                            if (!filename.empty())
                                vml::os::Message::Error("Logger : Logger is set to std out, but you provided a filename");
                        break;

                        case LogModes::TO_MEM:
                            if (!filename.empty())
                                vml::os::Message::Error("Logger : Logger is set to mem, but you provided a filename");
                        break;

                        case LogModes::TO_FILE:

                        {
                            if (filename.empty())
                                vml::os::Message::Error("Logger : Logger is set to file but FileName is null");
                            if (filename.length() > 255)
                                vml::os::Message::Error("Logger : Logger is set to file but FileName length is greater than 255");
                            // check for valid file format
                            std::regex filenamePattern(R"(^[a-zA-Z0-9_\-\.]+$)");
                            if (!std::regex_match(filename, filenamePattern))
                                vml::os::Message::Error("Logger : Logger is set to file but FileName has invalid format");
                            FileName = filename;
                            // open file stream
                            FileStream.open(filename.c_str(), std::ios::out);
                            // check file stream validity
                            if (!FileStream.is_open())
                                vml::os::Message::Error("Logger : Cannot open file ", filename.c_str());
                        }

                        break;

                        default:

                        break;
                    }
                }

                // ---------------------------------------------------------------------------------
                // sets max history

                void SetMaxHistory(const uint32_t maxhistory)
                {
                    if (maxhistory < 16 || maxhistory > 9999)
                        vml::os::Message::Error("Logger : Invalid MaxHistory value, range is 16-65536");
                    MaxHistory = maxhistory;
                    MaxNumberDigits = floor(log10(MaxHistory) + 1);
                    LogLines.clear();
                    LineNumber = 0;
                    // clear file is log mode is set to file
                    if (LogMode == LogModes::TO_FILE) {
                        FileStream.open(FileName, std::ios::out | std::ios::trunc);
                        FileStream.close();
                    }
                }

                // ---------------------------------------------------------------------------------
                // get log messages if log is set to memory

                const std::list<LogMessage>& GetHistory()
                {
                    return LogLines;
                }

                // ---------------------------------------------------------------------------------
                // gets number of lines

                uint32_t GetLinesCount() const
                {
                    return LineNumber;
                }

                // ---------------------------------------------------------------------------------
                // returns true if logger is initialized  

                bool IsInitialized() const
                {
                    return Initialized;
                }

                // ------------------------------------------------------------------
                // returns if log is set to verbose

                bool IsVerbose() const
                {
                    return VerboseMode == VerboseModes::VERBOSE;
                }

                // ------------------------------------------------------------------
                // returns if log is set to quiet

                bool IsQuiet() const
                {
                    return VerboseMode == VerboseModes::QUIET;
                }

                // ------------------------------------------------------------------
                // returns if log mode i sset to std output

                bool IsLogModeStd() const
                {
                    return LogMode == LogModes::TO_STD;
                }

                // ------------------------------------------------------------------
                // returns if log mode i sset to file

                bool IsLogModeFile() const
                {
                    return LogMode == LogModes::TO_FILE;
                }

                // ------------------------------------------------------------------
                // returns if log mode i sset to memory

                bool IsLogModeMem() const
                {
                    return LogMode == LogModes::TO_MEM;
                }

                // ------------------------------------------------------------------
                // dumps out log lines if logger is set to memory

                void Dump()
                {
                    for (auto it = LogLines.begin(); it != LogLines.end(); ++it)
                        std::cout << (*it).GetText() << std::endl;
                }
                
                //---------------------------------------------------------------------
                // copy constructor is private
                // no copies allowed since classes
                // are referenced

                Logger2(Logger2& other) = delete;

                //---------------------------------------------------------------------
                // overload operator is private, 
                // no copies allowed since classes
                // are referenced

                Logger2 operator=(const Logger2&) = delete;

                //-----------------------------------------------------------------------------------
                // class is non mmovable

                Logger2(Logger2&& other) = delete;
                Logger2& operator=(Logger2&& other) = delete;

                ~Logger2()
                {
                }

        };
    }
}
