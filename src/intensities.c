/** 
 * \file intensities.c
 * Intensities Processing.
 *//* 
 *  Created : 19 Apr 2010
 *  Author  : Tim Massingham/Hazel Marsden
 *
 *  Copyright (C) 2010 European Bioinformatics Institute
 *
 *  This file is part of the AYB base calling software.
 *
 *  AYB is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AYB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AYB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <err.h>
#include <string.h>
#include "intensities.h"
#include "lapack.h"
#include "nuc.h"

/* constants */
/* None      */


/* members */
/* None    */


/* private functions */
/* None              */


/* public functions */

/**
 * Process intensities.
 * ip = Minv %*% (Intensities-N) %*% Pinv
 *   - Uses identity: Vec(ip) = ( Pinv^t kronecker Minv) Vec(Intensities-N)
 *   - Storing Intensities-N as an intermediate saved < 3%
 *   - Calculating ip^t rather than ip (pcol loop is over minor index) made no difference
 *   - Using Pinv rather than Pinv^t makes little appreciable difference
 */
MAT process_intensities(const MAT intensities,
                        const MAT Minv_t, const MAT Pinv_t, const MAT N, MAT ip) {

    validate(NULL != intensities, NULL);
    validate(NULL != Minv_t, NULL);
    validate(NULL != Pinv_t, NULL);
    validate(NULL != N, NULL);

    const uint_fast32_t ncycle = Pinv_t->nrow;
    if (NULL==ip){
        ip = new_MAT(NBASE, ncycle);
        validate(NULL != ip, NULL);
    }
    memset(ip->x, 0, ip->nrow * ip->ncol * sizeof(real_t));

    // pre-calculate I - N, especially as I now integer
    real_t *tmp = calloc(NBASE * ncycle, sizeof(real_t));
    if (NULL==tmp) { goto cleanup; }

    for (uint_fast32_t i = 0; i < (NBASE * ncycle); i++) {
       tmp[i] = intensities->xint[i] - N->x[i];
    }

    for (uint_fast32_t icol = 0; icol < ncycle; icol++) {    // Columns of Intensity
        for (uint_fast32_t base = 0; base < NBASE; base++) { // Bases (rows of Minv, cols of Minv_t)
            real_t dp = 0;
            for (uint_fast32_t chan = 0; chan < NBASE; chan++) {  // Channels
                dp += Minv_t->x[base * NBASE + chan] * tmp[icol * NBASE + chan];
            }
            for (uint_fast32_t pcol = 0; pcol < ncycle; pcol++) { // Columns of ip
                ip->x[pcol * NBASE + base] += Pinv_t->x[icol * ncycle + pcol] * dp;
            }
        }
    }

    free(tmp);
    return ip;

cleanup:
    free_MAT(ip);
    return NULL;

}

MAT expected_intensities(const real_t lambda, const NUC * bases,
                         const MAT M, const MAT P, const MAT N, MAT e){
    validate(lambda>=0,NULL);
    validate(NULL!=bases,NULL);
    validate(NULL!=M,NULL);
    validate(NULL!=P,NULL);
    validate(NULL!=N,NULL);
    const uint_fast32_t ncycle = P->nrow;
    if(NULL==e){
        e = new_MAT(NBASE,ncycle);
        validate(NULL!=e,NULL);
    }
    memset(e->x, 0, NBASE*ncycle*sizeof(real_t));

    if(has_ambiguous_base(bases,ncycle)){
        for(uint_fast32_t cy2=0 ; cy2<ncycle ; cy2++){
            for(uint_fast32_t cy=0 ; cy<ncycle ; cy++){
                const uint_fast32_t base = bases[cy];
                if(!isambig(base)){
                    for ( uint_fast32_t ch=0 ; ch<NBASE ; ch++){
                        e->x[cy2*NBASE+ch] += M->x[base*NBASE+ch] * P->x[cy2*ncycle+cy];
		    }
                }
            }
        }
    } else {
       for(uint_fast32_t cy2=0 ; cy2<ncycle ; cy2++){
            for(uint_fast32_t cy=0 ; cy<ncycle ; cy++){
                const uint_fast32_t base = bases[cy];
                for ( uint_fast32_t ch=0 ; ch<NBASE ; ch++){
                    e->x[cy2*NBASE+ch] += M->x[base*NBASE+ch] * P->x[cy2*ncycle+cy];
                }
            }
        }
    }

    // Multiply by brightness;
    scale_MAT(e,lambda);
    // Add noise
    for ( uint_fast32_t i=0 ; i<(NBASE*ncycle) ; i++){
        e->x[i] += N->x[i];
    }
    return e;
}

/**
 * Process observed intensities into sequence space.
 * Solves the linear system vec(I_i-N) = A vec(S_i).
 * The input is the LU decomposition of the transpose of A.
 */
MAT processNew(const struct structLU AtLU, const MAT N, const MAT intensities, MAT p){
    if (NULL==AtLU.mat || NULL==N || NULL==intensities) { return NULL;}

    const int ncycle = N->ncol;
    const int nelt = NBASE*ncycle;

    // Create a new matrix for result if doesn't exist
    if(NULL==p){
        p = new_MAT(NBASE,ncycle);
        if(NULL==p){ return NULL;}
    }

	// Left-hand of equation. Writen over by solution
    for ( int i=0 ; i<nelt ; i++){
        p->x[i] = intensities->xint[i] - N->x[i];
    }

    // Solve using LAPACK routine
    const int inc = 1;
    int info = 0;
    getrs(LAPACK_TRANS,&nelt,&inc,AtLU.mat->x,&nelt,AtLU.piv,p->x,&nelt,&info);
    if(info!=0){ warnx("getrs in %s returned %d\n",__func__,info);}
    return p;
}
