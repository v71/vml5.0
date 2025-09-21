#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////
// global Preferences flags

namespace vml
{
    namespace flags
    {

        enum Preferences : std::uint32_t
        {
            VERBOSE       = 0,
            LOG_TO_STDOUT = 1,
            LOG_TO_FILE   = 2,
            LOG_TO_MEM    = 3,

            WINDOWED      = 64,
            FULLSCREEN    = 65,
            HIDECURSOR    = 66,
            MAXIMIZED     = 67,
            LOCKSWAP      = 68,
            ANTIALIASING  = 69,
            EXITONESC     = 128,

        };

        // -----------------------------------------------------------------------------

        static const std::string PreferencesToString(const uint32_t bitvalue)
        {
            switch (bitvalue)
            {
                case VERBOSE       : return "Verbose";       break;
                case LOG_TO_STDOUT : return "Log to stdout"; break;
                case LOG_TO_FILE   : return "Log to File";   break;
                case LOG_TO_MEM    : return "Log to Memory"; break;

                case WINDOWED      : return "Windowed";      break;
                case FULLSCREEN    : return "Full Screen";   break;
                case HIDECURSOR    : return "Hide Cursor";   break;
                case MAXIMIZED     : return "Maximized";     break;
                case LOCKSWAP      : return "LockSwap";      break;
                case EXITONESC     : return "Exit On Esc";   break;
            }
            return "PreferencesFlags : Undefined Flag";
        }

    }
}
