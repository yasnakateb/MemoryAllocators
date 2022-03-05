#pragma once

#include <chrono>
#include <map>
#include <iostream>
#include <string>


namespace msalloc::mointor
{
    class MemoryMonitor
    {
    public:
        MemoryMonitor( const std::string& allocator, bool print = false, std::ostream& = std::cout );
        ~MemoryMonitor();

    private:
        std::size_t currentMilli() const;

        const std::string mm_allocator;
        bool mm_print;
        std::ostream& mm_out;
        std::chrono::time_point<std::chrono::steady_clock> mm_tick;
    };

}
