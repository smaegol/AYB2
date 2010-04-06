/**
 * \file ayb_options.h
 * Public parts of AYB specific Options.
 *//*
 *  Created : 23 Feb 2010
 *  Author  : Hazel Marsden
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

#ifndef AYB_OPTIONS_H_
#define AYB_OPTIONS_H_

#include <stdbool.h>        // temp for flag


#define PROGNAME "AYB"                          ///< Program name in generic text.
#define AUTHOR "Hazel Marsden"                  ///< Author in generic text.
#define CONTACT "hazelm@ebi.ac.uk"              ///< Contact details in generic text.

/** AYB options structure; any here hmhm?. */
typedef struct {
    bool aflag;
    int aval;
    int ncycle;
} AYBOPT;


/* function prototypes */

bool read_options(const int argc, char ** const argv);
AYBOPT *myopt();

#endif /* AYB_OPTIONS_H_ */