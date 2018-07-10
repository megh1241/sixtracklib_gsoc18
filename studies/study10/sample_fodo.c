#include <stdio.h>
#include <stdlib.h>

#include "sixtracklib/sixtracklib.h"

#define NUM_OF_BLOCKS 32
#define THREADS_PER_BLOCK 32

void run(double **indata, double **outdata, int npart){
    // make particles
    st_Blocks     particles_buffer;
    st_Blocks_preset( &particles_buffer );
    int initial_particles_capacity = npart*8*30;
    int initial_particles_blocks   = 1;
    st_Blocks_init( &particles_buffer, initial_particles_blocks, initial_particles_capacity);
    //to be replaced by:
    //st_Blocks* particles_buffer = st_Blocks_new(initial_blocks, initial_capacity);
    st_Particles* particles = st_Blocks_add_particles( &particles_buffer, npart );
    // fill particles
    for (int ipart=0; ipart<npart; npart++){
      particles->x[ipart]  =indata[0][ipart];
      particles->px[ipart] =indata[1][ipart];
      //particles->rvv[ipart] = 
    };
    // end fill particles

    // make lattice
    // allocate lattice data
    st_Blocks     beam_elements;
    st_Blocks_preset( &beam_elements );
    int initial_elem_elements=40;
    int initial_elem_capacity = nitial_beam_elements
    st_Blocks_init( &particles_buffer, initial_elem_elements, initial_elem_capacity );
    // end allocate lattice data

    //Drift
    st_Blocks_add_drift( &beam_elements, 1.0 );
    //Thin Quad k1= 1e-3
    st_Blocks_add_multipole( &beam_elements, 0,0,0,1, (double[]){0,0,1e-3,0} );
    st_Blocks_add_drift( &beam_elements, 1.0 );
    st_Blocks_add_multipole( &beam_elements, 0,0,0,1, (double[]){0,0,-1e-3,0} );

    //Freeze data for tracking (will not needed in the future)
    st_Blocks_serialize(&particles_buffer);
    st_Blocks_serialize(&beam_elements);

    int num_turns=1;
    ret = st_Track_particles_on_cuda(
        NUM_OF_BLOCKS, THREADS_PER_BLOCK, num_turns,
            &particles_buffer, &beam_elements, 0 );

    // export data
    // fill particles
    for (int ipart=0; ipart<npart; npart++){
      out[0][ipart] =  particles->x[ipart];
      out[1][ipart] =  particles->px[ipart];
      //particles->rvv[ipart] =
    };
    // end fill particles

};








