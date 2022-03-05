#include <cstdint>

namespace msalloc
{
    template<typename T>
    class MemorySpace
    {
        public:
            MemorySpace();
            ~MemorySpace();

            void create_memory_space( uint32_t num_of_blocks );
            void delete_memory_space();

            T* ms_allocate();
            void ms_free( void* address );
            
        private:
            uint8_t* get_address( uint32_t index ) const;
            uint32_t get_index( const uint8_t* address ) const;

            uint8_t* memory_start;
            uint8_t* memory_next;
            const uint32_t block_size;
            uint32_t number_of_blocks;
            uint32_t number_of_free_blocks;
            uint32_t number_of_initialized_blocks;
        
    };


    template<typename T>
    MemorySpace<T>::MemorySpace()
        : number_of_blocks( 0 )
        , block_size( sizeof( T ) )
        , memory_start( nullptr )
        , number_of_free_blocks( 0 )
        , number_of_initialized_blocks( 0 )
        , memory_next( nullptr )
        
    {
        static_assert( sizeof( uint32_t ) <= sizeof( T ), "sizeof( T ) must be equal or greater than sizeof( uint32_t )" );
    }


    template<typename T>
    MemorySpace<T>::~MemorySpace()
    {
        delete_memory_space();
    }


    template<typename T>
    uint8_t* MemorySpace<T>::get_address( uint32_t index ) const
    {
        return memory_start + ( index * block_size );
    }


    template<typename T>
    uint32_t MemorySpace<T>::get_index( const uint8_t* address ) const
    {
        return static_cast<uint32_t>( address - memory_start ) / block_size;
    }


    template<typename T>
    T* MemorySpace<T>::ms_allocate()
    {
        if ( number_of_initialized_blocks < number_of_blocks )
        {
            uint32_t* address = reinterpret_cast<uint32_t*>( get_address( number_of_initialized_blocks ) );
            *address = ++number_of_initialized_blocks;
        }

        T* ret_val = nullptr;

        if ( number_of_free_blocks > 0 )
        {
            ret_val = reinterpret_cast<T*>( memory_next );

            if ( --number_of_free_blocks > 0 )
            {
                memory_next = get_address( *reinterpret_cast<uint32_t*>( memory_next ) );
            }
            else
            {
                memory_next = nullptr;
            }
        }

        return ret_val;
    }


    template<typename T>
    void MemorySpace<T>::ms_free( void* address )
    {
        *static_cast<uint32_t*>( address ) = memory_next == nullptr ? number_of_blocks : get_index( memory_next );
        memory_next = static_cast<uint8_t*>( address );
        ++number_of_free_blocks;
    }


    template<typename T>
    void MemorySpace<T>::create_memory_space( uint32_t num_of_blocks )
    {
        number_of_blocks = num_of_blocks;
        number_of_free_blocks = number_of_blocks;
        memory_start = new uint8_t[number_of_blocks * block_size];
        memory_next = memory_start;
    }


    template<typename T>
    void MemorySpace<T>::delete_memory_space()
    {
        delete[] memory_start;
        memory_start = nullptr;
    }

} // namespace msalloc