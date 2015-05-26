#ifndef OUT_H
#define OUT_H

#include <iostream>
#include <string>

#define FATAL 1

class Out
{
    public:
        static void success(std::string msg)
        {
            std::cout << "\x1b[32m[SUCCESS] " << msg << "\x1b[0m" << std::endl;
        }

        static void failure(std::string msg, bool fatal = false)
        {
            if(!fatal) {
                std::cout << "\x1b[31m[FAILURE] " << msg << "\x1b[0m" << std::endl;
            } else { // Fatal
                std::cout << "\x1b[31m[FATAL ERROR] " << msg << std::endl << "Server shutting down...\x1b[0m" << std::endl;
                std::_Exit(EXIT_FAILURE); // Shuts the server down. (std::_Exit)
            }
        }

        static void warning(std::string msg)
        {
            std::cout << "\x1b[33m[WARNING] " << msg << std::endl;
        }

        static void information(std::string msg)
        {
            std::cout << "\x1b[34m[INFO] " << msg << "\x1b[0m" << std::endl;
        }
};

#endif // OUT_H
