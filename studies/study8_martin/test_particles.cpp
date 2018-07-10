#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>

#include "sixtracklib/testlib.h"
#include "sixtracklib/sixtracklib.h"

int main()
{
    st_block_size_t const NUM_PARTICLE_BLOCKS     = 1u;
    st_block_size_t const PARTICLES_DATA_CAPACITY = 1048576u;
    st_block_size_t const NUM_PARTICLES           = 100u;

    st_Blocks particles_buffer;
    st_Blocks_preset( &particles_buffer );

    int ret = st_Blocks_init(
        &particles_buffer, NUM_PARTICLE_BLOCKS, PARTICLES_DATA_CAPACITY );

    assert( ret == 0 );

    st_Particles* particles = st_Blocks_add_particles(
        &particles_buffer, NUM_PARTICLES );

    if( particles != nullptr )
    {
        /* Just some random values assigned to the individual attributes
         * of the acutal particles -> these values do not make any
         * sense physically, but should be safe for calculating maps ->
         * please check with the map for drift whether they do not produce
         * some NaN's at the sqrt or divisions by 0 though!*/

        st_Random_init_genrand64( 20180622u );

        assert( particles->s     != nullptr );
        assert( particles->x     != nullptr );
        assert( particles->y     != nullptr );
        assert( particles->px    != nullptr );
        assert( particles->py    != nullptr );
        assert( particles->sigma != nullptr );
        assert( particles->rpp   != nullptr );
        assert( particles->rvv   != nullptr );

        assert( particles->num_of_particles == NUM_PARTICLES );

        for( st_block_size_t ii = 0 ; ii < NUM_PARTICLES ; ++ii )
        {
            static double const MIN_X  = double{ 0.05 };
            static double const MAX_X  = double{ 1.00 };
            static double const DX     = MAX_X - MIN_X;

            static double const MIN_Y  = double{ 0.05 };
            static double const MAX_Y  = double{ 1.00 };
            static double const DY     = MAX_Y - MIN_Y;

            static double const MIN_PX = double{ 0.05 };
            static double const MAX_PX = double{ 0.20 };
            static double const DPX    = MAX_PX - MIN_PX;

            static double const MIN_PY = double{ 0.05 };
            static double const MAX_PY = double{ 0.20 };
            static double const DPY    = MAX_PY - MIN_PY;

            static double const MIN_SIGMA = double{ 0.01 };
            static double const MAX_SIGMA = double{ 0.5 };
            static double const DSIGMA    = MAX_SIGMA - MIN_SIGMA;

            particles->s[ ii ]  = 0.0;
            particles->x[ ii ]  = MIN_X  + DX  * st_Random_genrand64_int64();
            particles->y[ ii ]  = MIN_Y  + DY  * st_Random_genrand64_int64();
            particles->px[ ii ] = MIN_PX + DPX * st_Random_genrand64_int64();
            particles->py[ ii ] = MIN_PY + DPY * st_Random_genrand64_int64();

            particles->sigma[ ii ] =
                MIN_SIGMA + DSIGMA * st_Random_genrand64_int64();

            particles->rpp[ ii ] = 1.0;
            particles->rvv[ ii ] = 1.0;
        }
    }

    ret = st_Blocks_serialize( &particles_buffer );
    assert( ret == 0 );

    /* ===================================================================== */
    /* Copy to other buffer to simulate working on the GPU */

    std::vector< uint8_t > copy_buffer(
        st_Blocks_get_const_data_begin( &particles_buffer ),
        st_Blocks_get_const_data_end( &particles_buffer ) );

    st_Blocks copy_particles_buffer;
    st_Blocks_preset( &copy_particles_buffer );

    ret = st_Blocks_unserialize( &copy_particles_buffer, copy_buffer.data() );
    assert( ret == 0 );

    /* on the GPU, these pointers will have __global as a decorator */

    SIXTRL_GLOBAL_DEC st_BlockInfo const* it  =
        st_Blocks_get_const_block_infos_begin( &copy_particles_buffer );

    SIXTRL_GLOBAL_DEC st_BlockInfo const* end =
        st_Blocks_get_const_block_infos_end( &copy_particles_buffer );

    for( ; it != end ; ++it )
    {
        SIXTRL_GLOBAL_DEC st_Particles const* particles =
            ( SIXTRL_GLOBAL_DEC st_Particles const* )it->begin;

        std::cout.precision( 4 );

        for( st_block_size_t ii = 0 ; ii < NUM_PARTICLES ; ++ii )
        {
            std::cout << " ii    = " << std::setw( 6 ) << ii
                      << std::fixed
                      << " | s     = " << std::setw( 6 ) << particles->s[ ii ]
                      << " | x     = " << std::setw( 6 ) << particles->x[ ii ]
                      << " | y     = " << std::setw( 6 ) << particles->y[ ii ]
                      << " | px    = " << std::setw( 6 ) << particles->px[ ii ]
                      << " | py    = " << std::setw( 6 ) << particles->py[ ii ]
                      << " | sigma = " << std::setw( 6 ) << particles->sigma[ ii ]
                      << " | rpp   = " << std::setw( 6 ) << particles->rpp[ ii ]
                      << " | rvv   = " << std::setw( 6 ) << particles->rvv[ ii ]
                      << "\r\n";
        }
    }

    std::cout.flush();

    st_Blocks_free( &particles_buffer );
    st_Blocks_free( &copy_particles_buffer );

    return 0;
}

/* end: studies/study8_martin/test_particles.cpp */
