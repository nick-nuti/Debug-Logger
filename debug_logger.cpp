
#include "debug_logger.hpp"

namespace debug_logger
{
    #ifdef DEBUG_VAR
        /*
        SafeQueue<std::string> print_queue;
        PrintThreadClass print_thread(print_queue);
        */
        
        //Log_Class debuglogger(1, "backup_project.txt");
        
        // print only - v = 0
        Log_Class debuglogger;
    #endif
}

// NOTE:
// this is recommended by Joe because I will get a multiple-definition problem if I dont do this

// create class object
// class object constructor should create these two ^
// 