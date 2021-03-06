#include <rsf.h>

#include "wexutl.h"
/*^*/

#ifndef _wexutl_h

/*------------------------------------------------------------*/
typedef struct wexcub *wexcub3d;
/*^*/

struct wexcub{
    bool    verb;
    sf_axa  amx,amy;
    sf_axa  alx,aly;
    sf_axa  az;
    sf_axa  aw;
    sf_axa  ae;
    float   eps;
    int  ompnth;
};
/*^*/
/*------------------------------------------------------------*/
typedef struct wexf2d *wexfft2d;
/*^*/

struct wexf2d{
    int            n1,n2;
    kiss_fft_cfg   forw1; /*   FFT on axis 1 */
    kiss_fft_cfg   invs1;
    kiss_fft_cfg   forw2; /*   FFT on axis 2 */
    kiss_fft_cfg   invs2;
    kiss_fft_cpx  *ctmp1; /* temp array */
    kiss_fft_cpx  *ctmp2; /* temp array */
    float       fftscale; /* FFT scale  */
};
/*^*/

/*------------------------------------------------------------*/
typedef struct wexssr *wexssr3d;
/*^*/

struct wexssr{
    sf_axa     bxx,byy;
    float         **kk;
    int        *lx,*ly;
    sf_complex   ***pk;
    sf_complex   ***wk;
    float        ****wt;
    float       dsmax2;
    wexfft2d      *f2d;
};
/*^*/

/*------------------------------------------------------------*/
typedef struct wexlsr *wexlsr3d;
/*^*/

struct wexlsr{
    sf_axa     bxx,byy;
    float         **kk;
    float         **kw;
    int        *lx,*ly;
    sf_complex   ***wk;
    sf_complex   ***wt;
    float       dsmax2;
    wexfft2d      *f2d;
    int            nsc;
    float       csc[5];
};
/*^*/

/*------------------------------------------------------------*/
typedef struct wextap *wextap3d;
/*^*/

struct wextap{
    int     nt1,   nt2,   nt3;
    int      n1,    n2,    n3;
    bool     b1,    b2,    b3;
    float *tap1, *tap2, *tap3;
};
/*^*/

/*------------------------------------------------------------*/
typedef struct wexslo *wexslo3d;
/*^*/

struct wexslo{
    sf_file    file; /* slowness slice */
    int         *nr; /* number of references */
    float      **sm; /* ref slo squared */
    int       nrmax;
    float     dsmax;
    float      ***s;
};
/*^*/

/*------------------------------------------------------------*/
typedef struct wexop *wexop3d;
/*^*/

struct wexop{
    sf_complex ***wws  ; /*          wavefield */   
    sf_complex ***wwr  ; /*          wavefield */   
    sf_complex ****ws;   /*          wavefield */     
    sf_complex ****wr;   /*          wavefield */     
    float ***rr;          /*       reflectivity */
};
/*^*/

/*------------------------------------------------------------*/
typedef struct wexcip *wexcip3d;
/*^*/

struct wexcip{
    sf_complex ***ws;   /*          wavefield */
    sf_complex ***wr;   /*          wavefield */
    sf_complex *****ei;  /* cip gathers      */
    sf_complex *****di;  /* cip gathers      */
    sf_complex ***ci;    /* image            */
    sf_complex   **tt;   /* time-shift       */
    int **mcxall;
    int **pcxall;
    int **mcyall;
    int **pcyall;
    int **mczall;
    int **pczall;
    int *ccin;
    pt3d *cc;
    float czmin,cymin,cxmin;
    float czmax,cymax,cxmax;
    int   nhz,  nhy,  nhx,  nht, nc;
    int   nhz2, nhy2, nhx2, nht2;
    float oht, dht;
};
/*^*/

/*------------------------------------------------------------*/
typedef struct wexncip *wexncip3d;
/*^*/

struct wexncip{
    sf_complex ***ws;   /*          wavefield */
    sf_complex ***wr;   /*          wavefield */
    sf_complex *****ei;  /* cip gathers      */
    sf_complex ***ci;    /* image            */
    sf_complex   **tt;   /* time-shift       */
    float ***dipx;
    float ***dipy;
    int ****mcx;
    int ****pcx;
    int ****mcy;
    int ****pcy;
    int ****mcz;
    int ****pcz;
    int *ccin;
    pt3d *cc;
    float czmin,cymin,cxmin;
    float czmax,cymax,cxmax;
    int   nhz,  nhy,  nhx,  nht, nc;
    int   nhz2, nhy2, nhx2, nht2;
  float oht, dht, dhx, dhy, dhz;
};
/*^*/

/*------------------------------------------------------------*/
typedef struct wexmvaop *wexmvaop3d;
/*^*/

struct wexmvaop{
    sf_complex ****bws;    /* background wavefield   */
    sf_complex ****bwr;    /* background wavefield   */
    sf_complex ***dws;     /* temp WF perturbation   */
    sf_complex ***dwr;     /* temp WF perturbation   */
    sf_complex ***stmp;    /* temp perturbation      */
    sf_complex ***rtmp;    /* temp perturbation      */
    sf_complex ****pws;    /* wavefield perturbation */
    sf_complex ****pwr;    /* wavefield perturbation */
    sf_complex ***pss;     /*  slowness perturbation */
    sf_complex ***psr;     /*  slowness perturbation */
    sf_complex ***pssum;   /* slowness perturbation  */
};
/*^*/

#endif
