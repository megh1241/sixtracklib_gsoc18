#include "sixtracklib/sixtracklib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_OF_BLOCKS 32
#define THREADS_PER_BLOCK 32


void run(ParticleData *p, char proc_flag );
void run(ParticleData *p, char proc_flag )
{
    int npart = p->npart;
    int initial_particles_capacity = npart*8*30;
    int initial_particles_blocks = 1;
    st_Blocks* particles_buffer = st_Blocks_new(
        initial_particles_blocks, initial_particles_capacity);

    st_Particles* particles = st_Blocks_add_particles( particles_buffer, npart );

    int initial_max_num_beam_elements = 40;
    int initial_beam_elements_capacity = initial_max_num_beam_elements * 1024;
    st_Blocks* beam_elements  = st_Blocks_new(
        initial_max_num_beam_elements, initial_beam_elements_capacity );

    if(proc_flag == 'c' || proc_flag == 'C')
    {
    	for(int ipart=0; ipart<npart; ipart++)
    	{
		particles->x[ipart] = p->x[ipart];
		particles->px[ipart] = p->xp[ipart];
		particles->y[ipart] = p->y[ipart];
		particles->py[ipart] = p->yp[ipart];
		particles->beta0[ipart] = p->beta0[ipart];
		particles->q0[ipart] = p->q0[ipart];
		particles->mass0[ipart] = p->mass0[ipart];
		particles->gamma0[ipart] = p->gamma0[ipart];
		particles->sigma[ipart] = p->z[ipart];
		particles->psigma[ipart] = 0;
		particles->delta[ipart] = p->dp[ipart];
		particles->p0c[ipart] = p->p0c[ipart];
		particles->rpp[ipart] = 1;
		particles->rvv[ipart] = 1;
    	}
    }

    for(int ipart=0; ipart<npart; ipart++)
    {
	particles->rpp[ipart] = 10;
    }
    st_Blocks_add_drift( beam_elements, 5.0 );
    st_Blocks_add_multipole( beam_elements, 0,0,0,1, (double[]){0,0,1e-3,0} );
    st_Blocks_add_drift( beam_elements, 1.0 );
    st_Blocks_add_multipole( beam_elements, 0,0,0,1, (double[]){0,0,-1e-3,0} );

    st_Blocks_serialize( particles_buffer );
    st_Blocks_serialize( beam_elements);
    int num_turns=1;

    if (proc_flag == 'c' || proc_flag == 'C')
    {
    	st_Track_particles_on_cuda( NUM_OF_BLOCKS, THREADS_PER_BLOCK, num_turns,
            particles_buffer, beam_elements, 0 );
        for (int ipart=0; ipart<npart; ipart++) 
	{
      	    p->x[ipart] = particles->x[ipart];
      	    p->xp[ipart] = particles->px[ipart];
      	    p->y[ipart] = particles->y[ipart];
      	    p->yp[ipart] = particles->py[ipart];
      	    p->q0[ipart] = particles->q0[ipart];
      	    p->mass0[ipart] = particles->mass0[ipart];
      	    p->beta0[ipart] = particles->beta0[ipart];
      	    p->gamma0[ipart] = particles->gamma0[ipart];
      	    p->z[ipart] = particles->sigma[ipart];
      	    p->dp[ipart] = particles->delta[ipart];
      	    p->p0c[ipart] = particles->p0c[ipart];
    	};
    }
    else{
	 printf("enterred\n");
    	/*st_Track_particles_on_cuda_gpu_part(NUM_OF_BLOCKS, THREADS_PER_BLOCK, num_turns,
		particles_buffer, beam_elements, 0, p->x, p->xp, p->y, p->yp, p->q0, 
		p->mass0, p->beta0, p->gamma0, p->z, p->dp, p->p0c);*/
    	st_Track_particles_on_cuda_gpu_part(NUM_OF_BLOCKS, THREADS_PER_BLOCK, num_turns,
		particles_buffer, beam_elements,0, p);
   }

    return;
}
