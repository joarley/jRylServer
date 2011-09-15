#ifndef LOGGER_H
#define LOGGER_H

// for help with the console colors look here:
// http://www.edoceo.com/liberum/?doc=printf-with-color
// some code explanation (used here):
// \033[2J : clear screen and go up/left (0, 0 position)
// \033[K  : clear line from actual position to end of the line
// \033[0m : reset color parameter
// \033[1m : use bold for font

#include "shared/typedef.h"
#include "shared/iSingleton.h"

#include <string>
#include <cstdarg>


#define CL_RESET	"\033[0m"
#define CL_CLS		"\033[2J"
#define CL_CLL		"\033[K"

// font settings
#define CL_BOLD		"\033[1m"
#define CL_NORM		CL_RESET
#define CL_NORMAL	CL_RESET
#define CL_NONE		CL_RESET
// foreground color and bold font (bright color on windows)
#define CL_WHITE	"\033[1;37m"
#define CL_GRAY		"\033[1;30m"
#define CL_RED		"\033[1;31m"
#define CL_GREEN	"\033[1;32m"
#define CL_YELLOW	"\033[1;33m"
#define CL_BLUE		"\033[1;34m"
#define CL_MAGENTA	"\033[1;35m"
#define CL_CYAN		"\033[1;36m"

// background color
#define CL_BG_BLACK		"\033[40m"
#define CL_BG_RED		"\033[41m"
#define CL_BG_GREEN		"\033[42m"
#define CL_BG_YELLOW	"\033[43m"
#define CL_BG_BLUE		"\033[44m"
#define CL_BG_MAGENTA	"\033[45m"
#define CL_BG_CYAN		"\033[46m"
#define CL_BG_WHITE		"\033[47m"
// foreground color and normal font (normal color on windows)
#define CL_LT_BLACK		"\033[0;30m"
#define CL_LT_RED		"\033[0;31m"
#define CL_LT_GREEN		"\033[0;32m"
#define CL_LT_YELLOW	"\033[0;33m"
#define CL_LT_BLUE		"\033[0;34m"
#define CL_LT_MAGENTA	"\033[0;35m"
#define CL_LT_CYAN		"\033[0;36m"
#define CL_LT_WHITE		"\033[0;37m"
// foreground color and bold font (bright color on windows)
#define CL_BT_BLACK		"\033[1;30m"
#define CL_BT_RED		"\033[1;31m"
#define CL_BT_GREEN		"\033[1;32m"
#define CL_BT_YELLOW	"\033[1;33m"
#define CL_BT_BLUE		"\033[1;34m"
#define CL_BT_MAGENTA	"\033[1;35m"
#define CL_BT_CYAN		"\033[1;36m"
#define CL_BT_WHITE		"\033[1;37m"

#define CL_WTBL			"\033[37;44m"	// white on blue
#define CL_XXBL			"\033[0;44m"	// default on blue
#define CL_PASS			"\033[0;32;42m"	// green on green

#define CL_SPACE		"           "	// space aquivalent of the print messages

#define ISDIGIT(c) (isdigit((unsigned char)(c)))
#ifdef _WIN32
#define is_console(handle) (FILE_TYPE_CHAR==GetFileType(handle))
#else
#define is_console(file) (0!=isatty(fileno(file)))
#endif

using namespace std;

namespace jRylServer {
namespace common {
namespace shared {

class Logger: public Singleton<Logger> {
public:
    MAKE_SINGLETON(Logger);
    void ClearScreen();
    void ShowError(string st, ...);
    void ShowInfo(string st, ...);
    void ShowNotice(string st, ...);
    void ShowWarning(string st, ...);
    void ShowMessage(string st, ...);
private:
    Logger();
#ifdef WIN32
    HANDLE m_stdout;
    int Logger::cprintf(HANDLE handle, const char *fmt, va_list argptr);
#else    
    FILE* m_stdout;
    int cprintf(FILE *file, const char *fmt, va_list argptr);
#endif
    bool stdout_with_ansisequence;
};

} //namespace shared
} //namespace common
} //namespace jRylServer
#endif