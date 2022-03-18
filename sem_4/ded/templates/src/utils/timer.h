#pragma once

#include <cstdio>
#include <chrono>

class Timer {
    std::chrono::time_point<std::chrono::system_clock> start_timestamp;
    std::chrono::time_point<std::chrono::system_clock> stop_timestamp;
    bool is_stopped;

    FILE *fileptr;

public:
    int id;
    long int cur_elapsed;

    Timer(int id = 0, FILE *fileptr = stdout, bool not_to_start = false);

    void start();
    double elapsed();
    void restart();
    void stop();
    void print(bool put_new_line = true, const char *format = nullptr);
};
