#ifndef DEBUGLOGGER
#define DEBUGLOGGER

#include <vector>
#include <iostream>
#include <typeinfo>
#include <thread>
#include <unistd.h>

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include <fstream>
#include <type_traits>

#define INACTIVE_VERBOSITY -1

#include "debug_logger_class.hpp"

    namespace debug_logger
    {  
        class Log_Class
        {
            public:
                Log_Class(int v, std::string fl)
                : verbosity(v)
                , filelocation(fl)
                , print_queue(v, fl)
                , print_thread(print_queue)
                {
                    // if v == 0 throw error
                }

                Log_Class(int v)
                : verbosity(v)
                , filelocation()
                , print_queue()
                , print_thread(print_queue)
                {
                    // if v == 1 or v == 2 throw error
                }

                Log_Class()
                :verbosity(INACTIVE_VERBOSITY)
                {}

                ~Log_Class()
                {
                    if(verbosity != INACTIVE_VERBOSITY)
                    {
                        std::cout << "Log_Class: Killing logger thread\n";
                        Log_Class::killthread();

                        std::cout << "Log_Class: Clearing Queue\n";
                        Log_Class::print_queue.clearqueues();
                    }
                }

                inline void startthread()
                {
                    print_thread.runthread = true;
                }

                inline void stopthread()
                {
                    while(print_thread.dump_printqueue == true);
                    print_thread.runthread = false;
                }

                inline void killthread()
                {
                    Log_Class::stopthread();
                    print_thread.keepalive = false;

                    //std::cout << "Running killthread function\n";
                }

                inline void printloggerconfig()
                {
                    std::cout << "VERBOSITY LEVEL = " << verbosity << "\n";
                    std::cout << "FILE LOCATION = " << filelocation << "\n";
                }

                // empty print function for variadic template "print()" function when there are no more input variables to the variadic template
                // aka input = NULL
                inline void print()
                {
                    
                }

                template <typename T>
                inline void print(T var1)
                {
                    if(verbosity != INACTIVE_VERBOSITY) print_queue.enqueue(toString(var1));
                }

                // variadic template function for printing/logging "debug_logger::print()" function.
                // Required format: debug_logger::print("String format with var1 = %v, and var2 = %v", var1, var2);
                // Variadic functions are recursive so a vector was created to hold: 
                    // - first indext vector[0] holds the print string
                    // - all variables converted to string
                template <typename T, typename... Types>
                inline void print(T var1, Types... var2)
                {
                    if(verbosity != INACTIVE_VERBOSITY)
                    {
                        std::vector<std::string> vec;

                        print_internal(vec, var1, var2...);

                        vec.clear();
                    }
                }

            private:
                int verbosity = 0;
                std::string filelocation;

                SafeQueue<std::string> print_queue;
                PrintThreadClass print_thread;

                //NOTE:
                // using "inline" helped to get rid of "multiple definition of 'function'" error that happens when template functions are in a header file-
                // and that header file is called from multiple places. Inline helps the compiler to know that it should only be created once.

                // function used to replace "%v" variables in print string
                inline std::string replace_identifiers(std::vector<std::string> &print_vec)
                {
                    size_t index;
                    size_t vec_index = 1;

                    std::string print_str = print_vec[0]; // first item stored in vector is print string

                    while(((index = print_str.find("%v")) != std::string::npos)) // find %v in print string
                    {
                        if(vec_index+1 > print_vec.size()) break;

                        else
                        {
                            print_str.replace(index, 2, print_vec[vec_index]); // replace "%v" (str len 2) with variable stored in print vector
                            vec_index++;
                        }
                    }

                    return print_str; // return final string to be printed and/or logged
                }

                // template for toString because "std::to_string()" isn't available for template inputs; need to clarify input
                template<typename T>
                inline std::string toString(T t) {
                    return std::to_string(t);
                }

                // template for toString for "char*" inputs; no changed needed for these inputs
                inline std::string toString(const char* t) {
                    return t;
                }

                // template for toString for "string" inputs; no changed needed for these inputs
                inline std::string toString(const std::string& t) {
                    return t;
                }

                // variadic template that is called whne there is no variables left; all that is left is the vector
                inline void print_internal(std::vector<std::string> &vec)
                {
                    std::string fullstring = replace_identifiers(vec); // replace "%v" with variables included in print() function sequentially
                    // this is where queue push should be
                    print_queue.enqueue(fullstring);
                }

                // variadic template that is called whne there is one variable left (and the vector)
                template<typename T>
                inline void print_internal(std::vector<std::string> &vec, T var1)
                {
                    vec.push_back(toString(var1));
                    print_internal(vec);
                }

                // variadic template function called from "print()" and from itself. Takes variable inputs, converts them to string, then pushes converted variables to vector
                template <typename T, typename... Types>
                inline void print_internal(std::vector<std::string> &vec, T var1, Types... var2)
                {
                    vec.push_back(toString(var1));
                    print_internal(vec, var2...);
                }
        };

        extern Log_Class debuglogger; // defined in debug_logger.cpp ONLY
    } 

#endif