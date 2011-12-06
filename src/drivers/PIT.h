/*
 * timer.h
 *
 *  Created on: 05/12/2011
 *      Author: piranna
 */

// timer.h -- Defines the interface for all PIT-related functions.
// Written for JamesM's kernel development tutorials.

#ifndef PIT_H_
#define PIT_H_

#include "common.h"

void PIT_init(u32int frequency);

#endif /* PIT_H_ */
