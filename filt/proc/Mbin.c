/* Data binning in 2-D slices.

Takes: < input.rsf head=header.rsf > binned.rsf
*/

#include <float.h>
#include <math.h>

#include <rsf.h>

#include "int2.h"
#include "interp.h"

int main (int argc, char* argv[])
{
    int id, nk, nd, im, nm, nt, it, nx, ny, n2, xkey, ykey, interp;
    float *mm, *count, *dd, **xy, *hdr;
    float x0, y0, dx, dy, xmin, xmax, ymin, ymax, f, dt, t0, clip;
    char *xk, *yk;
    sf_file in, out, head, fold;

    sf_init (argc,argv);
    in = sf_input("in");
    out = sf_output("out");

    if (!sf_histint(in,"n1",&nd)) sf_error("Need n1= in in");
    if (!sf_histint(in,"n2",&nt)) sf_error("Need n2= in in");
    if (SF_FLOAT != sf_gettype(in)) sf_error("Need float input");

    /* create coordinates */
    xy = sf_floatalloc2(2,nd);
    head = sf_input("head");

    if (SF_FLOAT != sf_gettype(head)) sf_error("Need float header");
    if (!sf_histint(head,"n1",&nk)) sf_error("No n1= in head");
    if (!sf_histint(head,"n2",&n2) || n2 != nd) 
	sf_error("Wrong n2= in head");

    if (NULL != (xk = sf_getstring("xk"))) {
	/* x key name */
	xkey = sf_segykey(xk);
    }  else if (!sf_getint("xkey",&xkey)) {
	/* x key number (if no xk), default is fldr */
	xkey = sf_segykey("fldr");
    }
    if (NULL != (yk = sf_getstring("yk"))) {
	/* y key name */
	ykey = sf_segykey(yk);
    }  else if (!sf_getint("ykey",&ykey)) {
	/* y key number (if no yk), default is tracf */
	ykey = sf_segykey("tracf");
    }

    if (xkey < 0 || xkey >= nk) 
	sf_error("xkey=%d is out of the range [0,%d]",xkey,nk-1);
    if (ykey < 0 || ykey >= nk) 
	sf_error("ykey=%d is out of the range [0,%d]",ykey,nk-1);

    hdr = sf_floatalloc(nk);

    ymin = xmin = +FLT_MAX;
    ymax = xmax = -FLT_MAX;
    for (id=0; id<nd; id++) {	
	sf_read (hdr,sizeof(float),nk,head);
	f = hdr[xkey]; 
	if (f < xmin) xmin=f;
	if (f > xmax) xmax=f;
	xy[id][0] = f;
	f = hdr[ykey]; 
	if (f < ymin) ymin=f;
	if (f > ymax) ymax=f;
	xy[id][1] = f;
    }

    sf_fileclose (head);

    if (sf_histfloat(in,"o2",&t0)) sf_putfloat(out,"o3",t0);
    if (sf_histfloat(in,"d2",&dt)) sf_putfloat(out,"d3",dt);

    /* let user overwrite */
    sf_getfloat ("xmin",&xmin);
    sf_getfloat ("xmax",&xmax);
    sf_getfloat ("ymin",&ymin);
    /* Grid dimensions */
    sf_getfloat ("ymax",&ymax);

    if (xmax <= xmin) sf_error ("xmax=%f <= xmin=%f",xmax,xmin);
    if (ymax <= ymin) sf_error ("ymax=%f <= ymin=%f",xmax,xmin);

    if (!sf_getfloat("x0",&x0)) x0=xmin; 
    if (!sf_getfloat("y0",&y0)) y0=ymin; 
    /* grid origin */

    sf_putfloat (out,"o1",x0);
    sf_putfloat (out,"o2",y0);

    /* create model */
    if (!sf_getint ("nx",&nx)) nx = (int) (xmax - xmin + 1.);
    /* Number of bins in x */
    if (!sf_getint ("ny",&ny)) ny = (int) (ymax - ymin + 1.);
    /* Number of bins in y */

    sf_putint(out,"n1",nx);
    sf_putint(out,"n2",ny);
    sf_putint(out,"n3",nt);

    if (!sf_getfloat("dx",&dx)) {
	/* bin size in x */
	if (1 >= nx) sf_error("Need dx=");
	dx = (xmax-xmin)/(nx-1);
    }

    if (!sf_getfloat("dy",&dy)) {
	/* bin size in y */
	if (1 >= nx) {
	    dy = dx;
	} else {
	    dy = (ymax-ymin)/(ny-1);
	}
    }

    sf_putfloat (out,"d1",dx);
    sf_putfloat (out,"d2",dy);
    
    /* initialize interpolation */
    if (!sf_getint("interp",&interp)) interp=1;
    /* [1,2] interpolation method, 1: nearest neighbor, 2: bi-linear */

    switch (interp) {
	case 1:
	    int2_init (xy, x0,y0,dx,dy,nx,ny, bin_int, 1, nd);
	    sf_warning("Using nearest-neighbor interpolation");
	    break;
	case 2:
	    int2_init (xy, x0,y0,dx,dy,nx,ny, lin_int, 2, nd);
	    sf_warning("Using linear interpolation");
	    break;
	case 3:
	    sf_error("Unsupported interp=%d",interp);
	    break;
    }

    nm = nx*ny;
    mm = sf_floatalloc(nm);
    dd = sf_floatalloc(nd);
    count  = sf_floatalloc(nm);

    /* compute fold */
    for (id=0; id<nd; id++) {
	dd[id]=1.;
    }

    int2_lop (true, false,nm,nd,count,dd);
 
    if (NULL != sf_getstring("fold")) {
	/* output file for fold (optional) */ 
	fold = sf_output("fold");
	sf_putint(fold,"n1",nx);
	sf_putint(fold,"n2",ny);
	sf_putint(fold,"n3",1);
	sf_putfloat(fold,"o1",x0);
	sf_putfloat(fold,"o2",y0);
	sf_putfloat(fold,"d1",dx);
	sf_putfloat(fold,"d2",dy);
	sf_write (count,sizeof(float),nm,fold);
	sf_fileclose (fold);
    }

    if (!sf_getfloat("clip",&clip)) clip = FLT_EPSILON;
    /* clip for fold normalization */

    for (im=0; im<nm; im++) {
	if (clip < count[im]) count[im]=1./fabsf(count[im]);
	else                  count[im]=0.;
    }

    for (it=0; it < nt; it++) { /* loop over time slices */
	sf_read (dd,sizeof(float),nd,in);
	int2_lop (true,false,nm,nd,mm,dd);
	for (im=0; im<nm; im++) {
	    mm[im] *= count[im];
	}
	sf_write (mm,sizeof(float),nm,out);
    }

    sf_close();
    exit(0);
}

/* 	$Id: Mbin.c,v 1.6 2004/03/22 05:43:24 fomels Exp $	 */
