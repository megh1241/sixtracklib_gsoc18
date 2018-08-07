#include "sixtracklib/sixtracklib.h"

#define NUM_OF_BLOCKS 32
#define THREADS_PER_BLOCK 32

extern void runtest(double **indata, double **outdata, int npart );

void runtest(double **indata, double **outdata, int npart){

    printf("Before\n");
    for(int ipart=0; ipart<10; ipart++){
	  printf("particle x; %lf\n",  indata[0][ipart]);
	  printf("particle xp; %lf\n",  indata[1][ipart]);
    }
    // end fill particles
    // make particles
    int initial_particles_capacity = npart*8*30;
    int initial_particles_blocks   = 1;
    st_Blocks* particles_buffer = st_Blocks_new(
        initial_particles_blocks, initial_particles_capacity);

    st_Particles* particles = st_Blocks_add_particles( particles_buffer, npart );

    // fill particles
    for (int ipart=0; ipart<npart; ipart++){
      particles->x[ipart]  = indata[0][ipart];
      particles->px[ipart] = indata[1][ipart];
    };

    // make lattice
    // allocate lattice data
    int initial_max_num_beam_elements = 40;
    int initial_beam_elements_capacity = initial_max_num_beam_elements * 1024;
    st_Blocks* beam_elements  = st_Blocks_new(
        initial_max_num_beam_elements, initial_beam_elements_capacity );
    // end allocate lattice data

    //Drift
    st_Drift* drift = st_Blocks_add_drift( beam_elements, 4.0 );
    printf( "drift= %.16f\r\n", drift->length );

    st_Blocks_serialize( particles_buffer );
    st_Blocks_serialize( beam_elements );

    int num_turns=1;
    st_Track_particles_on_cuda( NUM_OF_BLOCKS, THREADS_PER_BLOCK, num_turns,
            particles_buffer, beam_elements, 0 );

    printf("After\n");
    double* out_x = NS(Particles_get_x)(particles);
    double* out_px = NS(Particles_get_px)(particles);
    for(int i=0; i<10; i++)
    {
	    printf("%lf\n", out_x[i]);
	    printf("%lf\n", out_px[i]);
    }


    // export data
    // fill particles
    for (int i=0; i<10; i++){
	    num_turns++;
    };
    // end fill particles

    //free / release resources:
    particles = 0;
    st_Blocks_delete( particles_buffer );
    particles_buffer = 0;

    st_Blocks_delete( beam_elements );
    beam_elements = 0;

    return;
}

/* end: studies/study10/sample_fodo.c */



