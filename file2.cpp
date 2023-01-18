
#include "file2.hpp"

void printyfuncy()
{
    debug_logger::debuglogger.print("Ha haha ha\n");
}

void printyfuncy_ref(debug_logger::Log_Class &logclass_obj)
{
    logclass_obj.print("Wee woo\n");
}