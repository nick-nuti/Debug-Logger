#!/bin/bash

clang++ -std=c++17 -pthread main.cpp debug_logger.cpp file2.cpp -o sidethread -DDEBUG_VAR