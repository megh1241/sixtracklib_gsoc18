#include <stdio.h>

#include "sixtracklib/sixtracklib.h"

extern void run(double **indata, double **outdata, int npart );

int main()
{
    double **in, **out;
    int npart=10000;
    in = (double**)malloc(2*sizeof(double*));
    for (int i=0; i<2; i++)
    {
	    in[i] = (double*)malloc(npart*sizeof(double));
    }
    for (int i=0; i<2; i++)
    {
	    for (int j=0; j<npart; j++)
	    {
		    in[i][j] = ((double)(j)+20)/1000;
	    }
    }
    runtest(&in[0], out, npart);
    return 0;
}

/* end: studies/study10/run_sample_fodo.c */
