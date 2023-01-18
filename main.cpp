// backup: godbolt: https://godbolt.org/z/Y5jc5aTPe
// class-based: https://godbolt.org/z/zrq198c4b

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "debug_logger.hpp"
#include "file2.hpp"

int main(int argc, char **argv)
{
    debug_logger::debuglogger.printloggerconfig();
    
    int i0 = 0;
    int i1 = 1;
    int i2 = 2;
    
    auto start_time = std::chrono::steady_clock::now();

    debug_logger::debuglogger.print("i0 = %v\n", i0);
    debug_logger::debuglogger.print("i1 = %v\n", i1);
    debug_logger::debuglogger.print("i2 = %v\n", i2);
    debug_logger::debuglogger.print("i0 + i1 = %v + %v = %v\n", i0, i1, i0+i1);
    debug_logger::debuglogger.print("i1 + i2 = %v + %v = %v\n", i1, i2, i1+i2);
    debug_logger::debuglogger.print("i0 + i2 = %v + %v = %v\n", i0, i2, i0+i2);
    debug_logger::debuglogger.print(i0);
    debug_logger::debuglogger.print(i1);
    debug_logger::debuglogger.print(i2);

    auto end_time = std::chrono::steady_clock::now();
    std::cout << "Time Elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end_time-start_time).count() << "us\n";
    
    printyfuncy();
    printyfuncy_ref(debug_logger::debuglogger); // testing debug_logger::print() in another file
  
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    int i;

    auto start_time1 = std::chrono::steady_clock::now();
    for(i = 0; i < 10; i++) debug_logger::debuglogger.print("THIS IS A STRING PRINT TEST ON ITERATION %v\n", i);
    auto end_time1 = std::chrono::steady_clock::now();
    
    auto start_time2 = std::chrono::steady_clock::now();
    for(i = 0; i < 10; i++) printf("THIS IS A STRING PRINT TEST ON ITERATION %i\n", i);
    auto end_time2 = std::chrono::steady_clock::now();
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    // timeout thread and kill it
    usleep(999);

    std::cout << "MY PRINT Time Elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end_time1-start_time1).count() << "us\n";
    std::cout << "PRINTF Time Elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end_time2-start_time2).count() << "us\n";

    // timeout thread and kill it
    
    //debug_logger::debuglogger.stopthread();

    return 0;
}