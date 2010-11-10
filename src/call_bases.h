/** 
 * \file call_bases.h
 * Public parts of Call Bases.
 *//* 
 *  Created : 20 Apr 2010
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

#ifndef CALL_BASES_H_
#define CALL_BASES_H_

#include "matrix.h"
#include "nuc.h"
#include "utility.h"

/** Pair type allows return of base and quality. */
struct basequal { NUC base; real_t qual;};

/* function prototypes */
NUC call_base_simple( const real_t * restrict p);
NUC call_base_nodata(void);
struct basequal call_base_null(void);
struct basequal call_base( const real_t * restrict p, const real_t lambda, const real_t * restrict penalty, const MAT omega);

real_t adjust_quality(const real_t qual, const NUC prior, const NUC base, const NUC next);
/**  Adjust quality score for first base by setting prior to NUC_AMBIG. */
static inline real_t adjust_first_quality(const real_t qual, const NUC base, const NUC next){
    return adjust_quality(qual, NUC_AMBIG, base, next);
}
/**  Adjust quality score for last base by setting next to NUC_AMBIG. */
static inline real_t adjust_last_quality(const real_t qual, const NUC prior, const NUC base){
    return adjust_quality(qual, prior, base, NUC_AMBIG);
}

real_t get_mu(void);
bool set_mu(const char *mu_str);

#endif /* CALL_BASES_H_ */
