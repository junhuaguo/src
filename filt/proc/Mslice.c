#include <rsf.h>

int main(int argc, char* argv[])
{
    int nt, ns, nx, ix, is, it;
    float *trace, *picks, **semb, s0, ds, s;
    sf_file in, out, pick;

    sf_init(argc,argv);
    in = sf_input("in");
    pick = sf_input("pick");
    out = sf_output("out");

    if (!sf_histint(in,"n1",&nt)) sf_error("No n1= in input");
    if (!sf_histint(in,"n2",&ns)) sf_error("No n2= in input");
    nx = sf_leftsize(in,2);

    if (!sf_histfloat(in,"o2",&s0)) sf_error("No o2= in input");
    if (!sf_histfloat(in,"d2",&ds)) sf_error("No d2= in input");

    sf_putint(out,"n2",1);

    semb = sf_floatalloc2(nt,ns);
    picks = sf_floatalloc (nt);
    trace = sf_floatalloc (nt);

    for (ix=0; ix < nx; ix++) {
	sf_read (semb[0],sizeof(float),nt*ns,in);
	sf_read (picks,sizeof(float),nt,pick);

	for (it=0; it < nt; it++) {
	    s = (picks[it] - s0)/ds;
	    is = s; s -= is;
	    if (is >= 0 && is < ns-1) {
		trace[it] = s * semb[is+1][it] + (1.-s) * semb[is][it];
	    } else {
		trace[it] = 0.;
	    }
	}

	sf_write (trace,sizeof(float),nt,out);
    }

    sf_close();
    exit (0);
}

/* 	$Id: Mslice.c,v 1.2 2004/03/22 05:43:25 fomels Exp $	 */



