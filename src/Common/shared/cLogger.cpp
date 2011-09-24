#include "cLogger.h"
#include "shared/iSingleton.h"
#include "shared/utils.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>

namespace jRylServer {
namespace common {
namespace shared {

Logger::Logger() {
    m_PrintAnsiSeq = false;
    SetDefaultLogToSTDOUT();
}

Logger::~Logger() {

}

void Logger::SetDefaultLogToSTDOUT() {
    m_DefaultLogFile.reset(new LogFile);
    m_DefaultLogFile->SetFileDescriptor(_STDOUT_);
}

bool Logger::SetDefaultLogFile(LogFile_ptr file) {
    if (file->GetFileDescriptor() != INVALID_FILEDESCRIPTOR) {
        m_DefaultLogFile = file;
        return true;
    }
    return false;
}

Logger::LogFile_ptr Logger::CreateLogFile(const char* path, bool truncateExisting) {
    LogFile_ptr file(new LogFile);

#if defined(_WIN32) || defined(_WIN64)
    LOGFILEDESCRIPTOR hfile = CreateFileA(path,
      FILE_APPEND_DATA,
      FILE_SHARE_READ,
      NULL,
      (truncateExisting ? CREATE_ALWAYS : OPEN_ALWAYS),
      FILE_ATTRIBUTE_NORMAL,
      NULL);

    if (hfile == INVALID_FILEDESCRIPTOR) {
        ShowError("Could not open/create file %s error code: %d.\n", path, GetLastError());
    }
#else
    LOGFILEDESCRIPTOR hfile = fopen(path, (truncateExisting ? "w" : "a"));
    if (hfile == INVALID_FILEDESCRIPTOR) {
        ShowError("Could not open/create file %s error code: %d.\n", path, errno);
    }
#endif

    file->SetFileDescriptor(hfile);
    return file;
}

void Logger::ReleaseLogFile(LogFile_ptr file) {
    RemoveObserver(file);
#if defined(_WIN32) || defined(_WIN64)
    CloseHandle(file->GetFileDescriptor());
#else
    fclose(file->GetFileDescriptor());
#endif
    file->SetFileDescriptor(INVALID_FILEDESCRIPTOR);
}

void Logger::AddObserver(LogFile_ptr file) {
    RemoveObserver(file);
    m_Observers.push_back(file);
}

void Logger::RemoveObserver(LogFile_ptr file) {
    vector<LogFile_ptr>::iterator begin = m_Observers.begin();
    vector<LogFile_ptr>::iterator end = m_Observers.end();
    while (begin != end) {
        if (*begin == file) {
            m_Observers.erase(begin);
            break;
        }
        begin++;
    }
}

void Logger::ClearLogFile(LogFile_ptr file) {
    if (is_console(file->GetFileDescriptor())) {
        ShowMessage(file, CL_CLS);
    }

}

void Logger::ClearDefaultLogFile() {
    ClearLogFile(m_DefaultLogFile);
}

void Logger::ShowError(const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    tm timer;
    j_GetLocalTime(&timer);

    m_DefaultLogFile->lock();
    cprintf(m_DefaultLogFile,
      CL_RED"[Error]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
      timer.tm_mday,
      timer.tm_mon,
      timer.tm_year + 1900,
      timer.tm_hour,
      timer.tm_min,
      timer.tm_sec);

    cprintf(m_DefaultLogFile, fmt, argptr);
    m_DefaultLogFile->unlock();

    vector<LogFile_ptr>::iterator begin = m_Observers.begin();
    vector<LogFile_ptr>::iterator end = m_Observers.end();
    while (begin != end) {
        (*begin)->lock();
        cprintf(*begin,
          CL_RED"[Error]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
          timer.tm_mday,
          timer.tm_mon,
          timer.tm_year + 1900,
          timer.tm_hour,
          timer.tm_min,
          timer.tm_sec);

        cprintf(*begin, fmt, argptr);
        (*begin)->unlock();
        begin++;
    }

    va_end(argptr);
}

void Logger::ShowInfo(const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    tm timer;
    j_GetLocalTime(&timer);

    m_DefaultLogFile->lock();
    cprintf(m_DefaultLogFile, CL_WHITE"[Info]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
      timer.tm_mday,
      timer.tm_mon,
      timer.tm_year + 1900,
      timer.tm_hour,
      timer.tm_min,
      timer.tm_sec);

    cprintf(m_DefaultLogFile, fmt, argptr);
    m_DefaultLogFile->unlock();

    vector<LogFile_ptr>::iterator begin = m_Observers.begin();
    vector<LogFile_ptr>::iterator end = m_Observers.end();
    while (begin != end) {
        (*begin)->lock();
        cprintf(*begin, CL_WHITE"[Info]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
          timer.tm_mday,
          timer.tm_mon,
          timer.tm_year + 1900,
          timer.tm_hour,
          timer.tm_min,
          timer.tm_sec);
        cprintf(*begin, fmt, argptr);
        (*begin)->unlock();
        begin++;
    }

    va_end(argptr);
}

void Logger::ShowNotice(const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    tm timer;
    j_GetLocalTime(&timer);

    m_DefaultLogFile->lock();
    cprintf(m_DefaultLogFile, CL_WHITE"[Notice]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
      timer.tm_mday,
      timer.tm_mon,
      timer.tm_year + 1900,
      timer.tm_hour,
      timer.tm_min,
      timer.tm_sec);
    cprintf(m_DefaultLogFile, fmt, argptr);
    m_DefaultLogFile->unlock();

    vector<LogFile_ptr>::iterator begin = m_Observers.begin();
    vector<LogFile_ptr>::iterator end = m_Observers.end();
    while (begin != end) {
        (*begin)->lock();
        cprintf(*begin, CL_WHITE"[Notice]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
          timer.tm_mday,
          timer.tm_mon,
          timer.tm_year + 1900,
          timer.tm_hour,
          timer.tm_min,
          timer.tm_sec);
        cprintf(*begin, fmt, argptr);
        (*begin)->unlock();
        begin++;
    }

    va_end(argptr);
}

void Logger::ShowWarning(const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    tm timer;
    j_GetLocalTime(&timer);

    m_DefaultLogFile->lock();
    cprintf(m_DefaultLogFile, CL_YELLOW"[Warning]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
      timer.tm_mday,
      timer.tm_mon,
      timer.tm_year + 1900,
      timer.tm_hour,
      timer.tm_min,
      timer.tm_sec);
    cprintf(m_DefaultLogFile, fmt, argptr);
    m_DefaultLogFile->unlock();

    vector<LogFile_ptr>::iterator begin = m_Observers.begin();
    vector<LogFile_ptr>::iterator end = m_Observers.end();
    while (begin != end) {
        (*begin)->lock();
        cprintf(*begin, CL_YELLOW"[Warning]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
          timer.tm_mday,
          timer.tm_mon,
          timer.tm_year + 1900,
          timer.tm_hour,
          timer.tm_min,
          timer.tm_sec);
        cprintf(*begin, fmt, argptr);
        (*begin)->unlock();
        begin++;
    }

    va_end(argptr);
}

void Logger::ShowMessage(const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    tm timer;
    j_GetLocalTime(&timer);

    m_DefaultLogFile->lock();
    cprintf(m_DefaultLogFile, fmt, argptr);
    m_DefaultLogFile->unlock();

    vector<LogFile_ptr>::iterator begin = m_Observers.begin();
    vector<LogFile_ptr>::iterator end = m_Observers.end();
    while (begin != end) {
        (*begin)->lock();
        cprintf(*begin, fmt, argptr);
        (*begin)->unlock();
        begin++;
    }

    va_end(argptr);
}

void Logger::ShowError(LogFile_ptr file, const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    tm timer;
    j_GetLocalTime(&timer);

    file->lock();
    cprintf(file, CL_RED"[Error]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
      timer.tm_mday,
      timer.tm_mon,
      timer.tm_year + 1900,
      timer.tm_hour,
      timer.tm_min,
      timer.tm_sec);
    cprintf(file, fmt, argptr);
    file->unlock();
    va_end(argptr);
}

void Logger::ShowInfo(LogFile_ptr file, const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    tm timer;
    j_GetLocalTime(&timer);

    file->lock();
    cprintf(file, CL_WHITE"[Info]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
      timer.tm_mday,
      timer.tm_mon,
      timer.tm_year + 1900,
      timer.tm_hour,
      timer.tm_min,
      timer.tm_sec);
    cprintf(file, fmt, argptr);
    file->unlock();
    va_end(argptr);
}

void Logger::ShowNotice(LogFile_ptr file, const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    tm timer;
    j_GetLocalTime(&timer);

    file->lock();
    cprintf(file, CL_WHITE"[Notice]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
      timer.tm_mday,
      timer.tm_mon,
      timer.tm_year + 1900,
      timer.tm_hour,
      timer.tm_min,
      timer.tm_sec);
    cprintf(file, fmt, argptr);
    file->unlock();
    va_end(argptr);
}

void Logger::ShowWarning(LogFile_ptr file, const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    tm timer;
    j_GetLocalTime(&timer);

    file->lock();
    cprintf(file, CL_YELLOW"[Warning]"CL_RESET"[%02d/%02d/%d %02d:%02d:%02d]: ",
      timer.tm_mday,
      timer.tm_mon,
      timer.tm_year + 1900,
      timer.tm_hour,
      timer.tm_min,
      timer.tm_sec);
    cprintf(file, fmt, argptr);
    file->unlock();
    va_end(argptr);
}

void Logger::ShowMessage(LogFile_ptr file, const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    file->lock();
    cprintf(file, fmt, argptr);
    file->unlock();
    va_end(argptr);
}

int Logger::cprintf(LogFile_ptr logfile, const char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    int ret = cprintf(logfile, fmt, argptr);
    va_end(argptr);
    return ret;
}

int Logger::cprintf(LogFile_ptr logfile, const char *fmt, va_list argptr) {
    DWORD written;
    char *p, *q;
    char tempbuf[4096]; // temporary buffer
    LOGFILEDESCRIPTOR file = logfile->GetFileDescriptor();

    if (!fmt || !*fmt || file == INVALID_FILEDESCRIPTOR) {
        return 0;
    }

    // Print everything to the buffer
    vsprintf(tempbuf, fmt, argptr);
#ifdef WIN32
    if (!is_console(file) && m_PrintAnsiSeq) {
        WriteFile(file, tempbuf, (DWORD) strlen(tempbuf), &written, 0);
        return 0;
    }
#else
    if (is_console(file) || m_PrintAnsiSeq) {
        fprintf(file, "%s", tempbuf);
        return 0;
    }
#endif

    // start with processing
    p = tempbuf;
#ifdef WIN32
    while ((q = strchr(p, 0x1b)) != NULL) { // find the escape character
        if (0 == WriteConsoleA(file, p, (DWORD) (q - p), &written, 0)) // write up to the escape
            WriteFile(file, p, (DWORD) (q - p), &written, 0);

        if (q[1] != '[') { // write the escape char (whatever purpose it has)
            if (0 == WriteConsoleA(file, q, 1, &written, 0))
                WriteFile(file, q, 1, &written, 0);
            p = q + 1; //and start searching again
        } else { // from here, we will skip the '\033['
            // we break at the first unprocessible position
            // assuming regular text is starting there
            uint8 numbers[16], numpoint = 0;
            CONSOLE_SCREEN_BUFFER_INFO info;

            // initialize
            GetConsoleScreenBufferInfo(file, &info);
            memset(numbers, 0, sizeof (numbers));

            // skip escape and bracket
            q = q + 2;
            for (;;) {
                if (ISDIGIT(*q)) { // add number to number array, only accept 2digits, shift out the rest
                    // so // \033[123456789m will become \033[89m
                    numbers[numpoint] = (numbers[numpoint] << 4) | (*q - '0');
                    ++q;
                    // and next character
                    continue;
                } else if (*q == ';') { // delimiter
                    if (numpoint<sizeof (numbers) / sizeof (*numbers)) { // go to next array position
                        numpoint++;
                    } else { // array is full, so we 'forget' the first value
                        memmove(numbers, numbers + 1, sizeof (numbers) / sizeof (*numbers) - 1);
                        numbers[sizeof (numbers) / sizeof (*numbers) - 1] = 0;
                    }
                    ++q;
                    // and next number
                    continue;
                } else if (*q == 'm') { // \033[#;...;#m - Set Graphics Rendition (SGR)
                    uint8 i;
                    for (i = 0; i <= numpoint; ++i) {
                        if (0x00 == (0xF0 & numbers[i])) { // upper nibble 0
                            if (0 == numbers[i]) { // reset
                                info.wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                            } else if (1 == numbers[i]) { // set foreground intensity
                                info.wAttributes |= FOREGROUND_INTENSITY;
                            } else if (5 == numbers[i]) { // set background intensity
                                info.wAttributes |= BACKGROUND_INTENSITY;
                            } else if (7 == numbers[i]) { // reverse colors (just xor them)
                                info.wAttributes ^= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
                                  BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
                            }
                            //case '2': // not existing
                            //case '3': // blinking (not implemented)
                            //case '4': // unterline (not implemented)
                            //case '6': // not existing
                            //case '8': // concealed (not implemented)
                            //case '9': // not existing
                        } else if (0x20 == (0xF0 & numbers[i])) { // off

                            if (1 == numbers[i]) { // set foreground intensity off
                                info.wAttributes &= ~FOREGROUND_INTENSITY;
                            } else if (5 == numbers[i]) { // set background intensity off
                                info.wAttributes &= ~BACKGROUND_INTENSITY;
                            } else if (7 == numbers[i]) { // reverse colors (just xor them)
                                info.wAttributes ^= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
                                  BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
                            }
                        } else if (0x30 == (0xF0 & numbers[i])) { // foreground
                            uint8 num = numbers[i]&0x0F;
                            if (num == 9) info.wAttributes |= FOREGROUND_INTENSITY;
                            if (num > 7) num = 7; // set white for 37, 38 and 39
                            info.wAttributes &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                            if ((num & 0x01) > 0) // lowest bit set = red
                                info.wAttributes |= FOREGROUND_RED;
                            if ((num & 0x02) > 0) // second bit set = green
                                info.wAttributes |= FOREGROUND_GREEN;
                            if ((num & 0x04) > 0) // third bit set = blue
                                info.wAttributes |= FOREGROUND_BLUE;
                        } else if (0x40 == (0xF0 & numbers[i])) { // background
                            uint8 num = numbers[i]&0x0F;
                            if (num == 9) info.wAttributes |= BACKGROUND_INTENSITY;
                            if (num > 7) num = 7; // set white for 47, 48 and 49
                            info.wAttributes &= ~(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
                            if ((num & 0x01) > 0) // lowest bit set = red
                                info.wAttributes |= BACKGROUND_RED;
                            if ((num & 0x02) > 0) // second bit set = green
                                info.wAttributes |= BACKGROUND_GREEN;
                            if ((num & 0x04) > 0) // third bit set = blue
                                info.wAttributes |= BACKGROUND_BLUE;
                        }
                    }
                    // set the attributes
                    SetConsoleTextAttribute(file, info.wAttributes);
                } else if (*q == 'J') { // \033[#J - Erase Display (ED)
                    //    \033[0J - Clears the screen from cursor to end of display. The cursor position is unchanged.
                    //    \033[1J - Clears the screen from start to cursor. The cursor position is unchanged.
                    //    \033[2J - Clears the screen and moves the cursor to the home position (line 1, column 1).
                    uint8 num = (numbers[numpoint] >> 4)*10 + (numbers[numpoint]&0x0F);
                    int cnt;
                    DWORD tmp;
                    COORD origin = {0, 0};
                    if (num == 1) { // chars from start up to and including cursor
                        cnt = info.dwSize.X * info.dwCursorPosition.Y + info.dwCursorPosition.X + 1;
                    } else if (num == 2) { // Number of chars on screen.
                        cnt = info.dwSize.X * info.dwSize.Y;
                        SetConsoleCursorPosition(file, origin);
                    } else// 0 and default
                    { // number of chars from cursor to end
                        origin = info.dwCursorPosition;
                        cnt = info.dwSize.X * (info.dwSize.Y - info.dwCursorPosition.Y) - info.dwCursorPosition.X;
                    }
                    FillConsoleOutputAttribute(file, info.wAttributes, cnt, origin, &tmp);
                    FillConsoleOutputCharacter(file, ' ', cnt, origin, &tmp);
                } else if (*q == 'K') { // \033[K  : clear line from actual position to end of the line
                    //    \033[0K - Clears all characters from the cursor position to the end of the line.
                    //    \033[1K - Clears all characters from start of line to the cursor position.
                    //    \033[2K - Clears all characters of the whole line.

                    uint8 num = (numbers[numpoint] >> 4)*10 + (numbers[numpoint]&0x0F);
                    COORD origin = {0, info.dwCursorPosition.Y}; //warning C4204
                    SHORT cnt;
                    DWORD tmp;
                    if (num == 1) {
                        cnt = info.dwCursorPosition.X + 1;
                    } else if (num == 2) {
                        cnt = info.dwSize.X;
                    } else// 0 and default
                    {
                        origin = info.dwCursorPosition;
                        cnt = info.dwSize.X - info.dwCursorPosition.X; // how many spaces until line is full
                    }
                    FillConsoleOutputAttribute(file, info.wAttributes, cnt, origin, &tmp);
                    FillConsoleOutputCharacter(file, ' ', cnt, origin, &tmp);
                } else if (*q == 'H' || *q == 'f') { // \033[#;#H - Cursor Position (CUP)
                    // \033[#;#f - Horizontal & Vertical Position
                    // The first # specifies the line number, the second # specifies the column.
                    // The default for both is 1
                    info.dwCursorPosition.X = (numbers[numpoint]) ? (numbers[numpoint] >> 4)*10 + ((numbers[numpoint]&0x0F) - 1) : 0;
                    info.dwCursorPosition.Y = (numpoint && numbers[numpoint - 1]) ? (numbers[numpoint - 1] >> 4)*10 + ((numbers[numpoint - 1]&0x0F) - 1) : 0;

                    if (info.dwCursorPosition.X >= info.dwSize.X) info.dwCursorPosition.Y = info.dwSize.X - 1;
                    if (info.dwCursorPosition.Y >= info.dwSize.Y) info.dwCursorPosition.Y = info.dwSize.Y - 1;
                    SetConsoleCursorPosition(file, info.dwCursorPosition);
                } else if (*q == 's') { // \033[s - Save Cursor Position (SCP)
                    /* XXX Two streams are being used. Disabled to avoid inconsistency [flaviojs]
                    CONSOLE_SCREEN_BUFFER_INFO info;
                    GetConsoleScreenBufferInfo(handle, &info);
                    saveposition = info.dwCursorPosition;
                     */
                } else if (*q == 'u') { // \033[u - Restore cursor position (RCP)
                    /* XXX Two streams are being used. Disabled to avoid inconsistency [flaviojs]
                    SetConsoleCursorPosition(handle, saveposition);
                     */
                } else if (*q == 'A') { // \033[#A - Cursor Up (CUU)
                    // Moves the cursor UP # number of lines
                    info.dwCursorPosition.Y -= (numbers[numpoint]) ? (numbers[numpoint] >> 4)*10 + (numbers[numpoint]&0x0F) : 1;

                    if (info.dwCursorPosition.Y < 0)
                        info.dwCursorPosition.Y = 0;
                    SetConsoleCursorPosition(file, info.dwCursorPosition);
                } else if (*q == 'B') { // \033[#B - Cursor Down (CUD)
                    // Moves the cursor DOWN # number of lines
                    info.dwCursorPosition.Y += (numbers[numpoint]) ? (numbers[numpoint] >> 4)*10 + (numbers[numpoint]&0x0F) : 1;

                    if (info.dwCursorPosition.Y >= info.dwSize.Y)
                        info.dwCursorPosition.Y = info.dwSize.Y - 1;
                    SetConsoleCursorPosition(file, info.dwCursorPosition);
                } else if (*q == 'C') { // \033[#C - Cursor Forward (CUF)
                    // Moves the cursor RIGHT # number of columns
                    info.dwCursorPosition.X += (numbers[numpoint]) ? (numbers[numpoint] >> 4)*10 + (numbers[numpoint]&0x0F) : 1;

                    if (info.dwCursorPosition.X >= info.dwSize.X)
                        info.dwCursorPosition.X = info.dwSize.X - 1;
                    SetConsoleCursorPosition(file, info.dwCursorPosition);
                } else if (*q == 'D') { // \033[#D - Cursor Backward (CUB)
                    // Moves the cursor LEFT # number of columns
                    info.dwCursorPosition.X -= (numbers[numpoint]) ? (numbers[numpoint] >> 4)*10 + (numbers[numpoint]&0x0F) : 1;

                    if (info.dwCursorPosition.X < 0)
                        info.dwCursorPosition.X = 0;
                    SetConsoleCursorPosition(file, info.dwCursorPosition);
                } else if (*q == 'E') { // \033[#E - Cursor Next Line (CNL)
                    // Moves the cursor down the indicated # of rows, to column 1
                    info.dwCursorPosition.Y += (numbers[numpoint]) ? (numbers[numpoint] >> 4)*10 + (numbers[numpoint]&0x0F) : 1;
                    info.dwCursorPosition.X = 0;

                    if (info.dwCursorPosition.Y >= info.dwSize.Y)
                        info.dwCursorPosition.Y = info.dwSize.Y - 1;
                    SetConsoleCursorPosition(file, info.dwCursorPosition);
                } else if (*q == 'F') { // \033[#F - Cursor Preceding Line (CPL)
                    // Moves the cursor up the indicated # of rows, to column 1.
                    info.dwCursorPosition.Y -= (numbers[numpoint]) ? (numbers[numpoint] >> 4)*10 + (numbers[numpoint]&0x0F) : 1;
                    info.dwCursorPosition.X = 0;

                    if (info.dwCursorPosition.Y < 0)
                        info.dwCursorPosition.Y = 0;
                    SetConsoleCursorPosition(file, info.dwCursorPosition);
                } else if (*q == 'G') { // \033[#G - Cursor Horizontal Absolute (CHA)
                    // Moves the cursor to indicated column in current row.
                    info.dwCursorPosition.X = (numbers[numpoint]) ? (numbers[numpoint] >> 4)*10 + ((numbers[numpoint]&0x0F) - 1) : 0;

                    if (info.dwCursorPosition.X >= info.dwSize.X)
                        info.dwCursorPosition.X = info.dwSize.X - 1;
                    SetConsoleCursorPosition(file, info.dwCursorPosition);
                } else if (*q == 'L' || *q == 'M' || *q == '@' || *q == 'P') { // not implemented, just skip
                } else { // no number nor valid sequencer
                    // something is fishy, we break and give the current char free
                    --q;
                }
                // skip the sequencer and search again
                p = q + 1;
                break;
            }// end while
        }
    }
    if (*p) // write the rest of the buffer
        if (0 == WriteConsoleA(file, p, (DWORD) strlen(p), &written, 0))
            WriteFile(file, p, (DWORD) strlen(p), &written, 0);
#else
    while ((q = strchr(p, 0x1b)) != NULL) { // find the escape character
        fprintf(file, "%.*s", (int) (q - p), p); // write up to the escape
        if (q[1] != '[') { // write the escape char (whatever purpose it has)
            fprintf(file, "%.*s", 1, q);
            p = q + 1; //and start searching again
        } else { // from here, we will skip the '\033['
            // we break at the first unprocessible position
            // assuming regular text is starting there

            // skip escape and bracket
            q = q + 2;
            while (1) {
                if (ISDIGIT(*q)) {
                    ++q;
                    // and next character
                    continue;
                } else if (*q == ';') { // delimiter
                    ++q;
                    // and next number
                    continue;
                } else if (*q == 'm') { // \033[#;...;#m - Set Graphics Rendition (SGR)
                    // set the attributes
                } else if (*q == 'J') { // \033[#J - Erase Display (ED)
                } else if (*q == 'K') { // \033[K  : clear line from actual position to end of the line
                } else if (*q == 'H' || *q == 'f') { // \033[#;#H - Cursor Position (CUP)
                    // \033[#;#f - Horizontal & Vertical Position
                } else if (*q == 's') { // \033[s - Save Cursor Position (SCP)
                } else if (*q == 'u') { // \033[u - Restore cursor position (RCP)
                } else if (*q == 'A') { // \033[#A - Cursor Up (CUU)
                    // Moves the cursor UP # number of lines
                } else if (*q == 'B') { // \033[#B - Cursor Down (CUD)
                    // Moves the cursor DOWN # number of lines
                } else if (*q == 'C') { // \033[#C - Cursor Forward (CUF)
                    // Moves the cursor RIGHT # number of columns
                } else if (*q == 'D') { // \033[#D - Cursor Backward (CUB)
                    // Moves the cursor LEFT # number of columns
                } else if (*q == 'E') { // \033[#E - Cursor Next Line (CNL)
                    // Moves the cursor down the indicated # of rows, to column 1
                } else if (*q == 'F') { // \033[#F - Cursor Preceding Line (CPL)
                    // Moves the cursor up the indicated # of rows, to column 1.
                } else if (*q == 'G') { // \033[#G - Cursor Horizontal Absolute (CHA)
                    // Moves the cursor to indicated column in current row.
                } else if (*q == 'L' || *q == 'M' || *q == '@' || *q == 'P') { // not implemented, just skip
                } else { // no number nor valid sequencer
                    // something is fishy, we break and give the current char free
                    --q;
                }
                // skip the sequencer and search again
                p = q + 1;
                break;
            }// end while
        }
    }
    if (*p) // write the rest of the buffer
        fprintf(file, "%s", p);
#endif
    return 0;
}

} //namespace shared
} //namespace common
} //namespace jRylServer