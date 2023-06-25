/* 
(c) 2022-2023 Joscha Egloff
*/

#include <ant/string.hpp>

/// @brief Stream Type! Firs tbit is always 0 and the last one always 1!
enum StreamType{
    SerialStream =    0b00000111,
    DisplayStream =   0b00011001,
    FileStream =      0b01100001
};

#define OP_MOD(name, args, code) inline void name args code;

/// @brief A Logger that support levels and multi stream(serial, display, file)!
class Logger
{
    public:
    static std::string _Span;
    static int _Level;
    static StreamType _Streams;

    static OP_MOD(
        Span,
        (std::string newSpan),
        {
            _Span = newSpan;
        }
    )

    /// @brief Configure the Logger!
    /// @param level The Logger of the logger! 
    /// @param streams The Stream that the Logger sould use(cna be mutible like <Serial | File>)!
     inline void operator ()(int level, StreamType streams){
        _Level = level;
        _Streams = streams;
    }
}; // namespace Logger

