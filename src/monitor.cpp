#include "monitor.hpp"

namespace msalloc::mointor
{
    MemoryMonitor::MemoryMonitor( const std::string& allocator, bool print, std::ostream& out )
        : mm_allocator( allocator )
        , mm_print( print )
        , mm_out( out )
        , mm_tick( std::chrono::steady_clock::now() )
    {
        const auto milli = currentMilli();
        if ( mm_print )
            mm_out << "\n************** " << mm_allocator << " **************" << "\nBegin ==> [" << milli << " ms] " << std::endl;
    }

    MemoryMonitor::~MemoryMonitor()
    {
        const auto milli = currentMilli();
        if ( mm_print )
            mm_out << "End ==> [" << milli << " ms] " << std::endl;
        
    }

    std::size_t MemoryMonitor::currentMilli() const
    {
        const auto tick = std::chrono::steady_clock::now();
        return static_cast<std::size_t>( std::chrono::duration_cast<std::chrono::milliseconds>( tick - mm_tick ).count() );
    }

} // namespace msalloc::mointor