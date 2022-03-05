#include "msalloc.hpp"
#include "monitor.hpp"
#include "mscalloc.h"
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////
/////////////                                                    /////////////
/////////////                                                    /////////////
/////////////        Comparison Of Memory Allocation Time        /////////////
/////////////                                                    /////////////
/////////////                                                    /////////////
//////////////////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
    using namespace msalloc;
    using namespace msalloc::mointor;
    
    const int num = 1000;
    const int size = 100;

    struct Frame
    {
        uint8_t start_byte;
        uint32_t data;
        uint8_t stop_byte;
    };
    using Type = Frame;

    //////////////////////////////////////////////////////////////////////////////
    /////////////                                                    /////////////
    /////////////                         NEW                        /////////////
    /////////////                                                    /////////////
    //////////////////////////////////////////////////////////////////////////////
        {
            MemoryMonitor monitor_new( "new", true );
            Type* arr[size];

            for ( int n = 0; n < num; ++n )
            {

                for ( int i = size - 1; 0 <= i; --i )
                {
                    arr[i] = new Type;
                }

                for ( int i = size - 1; 0 <= i; --i )
                {
                    delete arr[i];
                }

            }     
        }
    //////////////////////////////////////////////////////////////////////////////
    /////////////                                                    /////////////
    /////////////                       MSALLOC                      /////////////
    /////////////                                                    /////////////
    //////////////////////////////////////////////////////////////////////////////
        {
            MemoryMonitor monitor_msalloc( "msalloc", true );
            Type* ms_arr[size];
            MemorySpace<Type> ms;
            ms.create_memory_space( size );

            for ( int n = 0; n < num; ++n )
            {

                for ( int i = size - 1; 0 <= i; --i )
                {
                    ms_arr[i] = ms.ms_allocate();
                }

                for ( int i = size - 1; 0 <= i; --i )
                {
                    ms.ms_free( ms_arr[i] );
                }

            }
            ms.delete_memory_space();
        }
    //////////////////////////////////////////////////////////////////////////////
    /////////////                                                    /////////////
    /////////////                       MSCALLOC                     /////////////
    /////////////                                                    /////////////
    //////////////////////////////////////////////////////////////////////////////   
        {
            MemoryMonitor monitor_mscalloc( "mscalloc", true );
            Type* msc_arr[size];
            memory_space p;
            create_memory_space(&p, sizeof(p), size);

            for ( int n = 0; n < num; ++n )
            {

                for ( int i = size - 1; 0 <= i; --i )
                {
                    msc_arr[i] = (Type * )msc_alloc(&p);
                }

                for ( int i = size - 1; 0 <= i; --i )
                {
                    msc_free(&p,  msc_arr[i] );
                }

            }
            delete_memory_space(&p);
        }   
    //////////////////////////////////////////////////////////////////////////////
    /////////////                                                    /////////////
    /////////////                       MALLOC                       /////////////
    /////////////                                                    /////////////
    //////////////////////////////////////////////////////////////////////////////   
        {
            MemoryMonitor monitor_mscalloc( "malloc", true );
            Type* m_arr[size];

            for ( int n = 0; n < num; ++n )
            {

                for ( int i = size - 1; 0 <= i; --i )
                {
                    m_arr[i] = (Type * )malloc(sizeof(Type));
                }

                for ( int i = size - 1; 0 <= i; --i )
                {
                    free(m_arr[i]);
                }

            }
        }   
                   
    return 0;
}