//
//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// range_algebra allows element-by-element operations on ranges
// So far only implemented for plus

#ifndef BLINK_ITERATOR_RANGE_ALGEBRA_H_AHZ
#define BLINK_ITERATOR_RANGE_ALGEBRA_H_AHZ

#include <blink/iterator/range_algebra_wrapper.h>
#include <blink/iterator/range_algebra_plus.h>
#include <blink/iterator/range_algebra_minus.h>
#include <blink/iterator/range_algebra_divides.h>
#include <blink/iterator/range_algebra_multiplies.h>
#include <blink/iterator/range_algebra_logical_and.h>
#include <blink/iterator/range_algebra_logical_or.h>
#include <blink/iterator/range_algebra_logical_not.h>
#include <blink/iterator/range_algebra_greater.h>
#include <blink/iterator/range_algebra_greater_equal.h>
#include <blink/iterator/range_algebra_less.h>
#include <blink/iterator/range_algebra_less_equal.h>
#include <blink/iterator/range_algebra_equal_to.h>
#include <blink/iterator/range_algebra_not_equal_to.h>
#include <blink/iterator/range_algebra_negate.h>
#include <blink/iterator/range_algebra_transform.h>
// bitwise operators still missing
#endif