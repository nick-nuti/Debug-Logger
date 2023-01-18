
#include "debug_logger.hpp"

namespace debug_logger
{
    #ifdef DEBUG_VAR
        /*
        SafeQueue<std::string> print_queue;
        PrintThreadClass print_thread(print_queue);
        */
        Log_Class debuglogger(2, "/var/lib/backup_project.txt");
    #endif
}

// NOTE:
// this is recommended by Joe because I will get a multiple-definition problem if I dont do this

// create class object
// class object constructor should create these two ^
// 