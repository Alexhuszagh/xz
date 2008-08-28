///////////////////////////////////////////////////////////////////////////////
//
/// \file       filter_common.c
/// \brief      Filter-specific stuff common for both encoder and decoder
//
//  Copyright (C) 2008 Lasse Collin
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
///////////////////////////////////////////////////////////////////////////////

#include "filter_common.h"


static const struct {
	/// Filter ID
	lzma_vli id;

	/// True if it is OK to use this filter as non-last filter in
	/// the chain.
	bool non_last_ok;

	/// True if it is OK to use this filter as the last filter in
	/// the chain.
	bool last_ok;

	/// True if the filter may change the size of the data (that is, the
	/// amount of encoded output can be different than the amount of
	/// uncompressed input).
	bool changes_size;

} features[] = {
#if defined (HAVE_ENCODER_LZMA) || defined(HAVE_DECODER_LZMA)
	{
		.id = LZMA_FILTER_LZMA,
		.non_last_ok = false,
		.last_ok = true,
		.changes_size = true,
	},
#endif
#ifdef HAVE_DECODER_LZMA2
	{
		.id = LZMA_FILTER_LZMA2,
		.non_last_ok = false,
		.last_ok = true,
		.changes_size = true,
	},
#endif
#if defined(HAVE_ENCODER_SUBBLOCK) || defined(HAVE_DECODER_SUBBLOCK)
	{
		.id = LZMA_FILTER_SUBBLOCK,
		.non_last_ok = true,
		.last_ok = true,
		.changes_size = true,
	},
#endif
#ifdef HAVE_DECODER_X86
	{
		.id = LZMA_FILTER_X86,
		.non_last_ok = true,
		.last_ok = false,
		.changes_size = false,
	},
#endif
#if defined(HAVE_ENCODER_POWERPC) || defined(HAVE_DECODER_POWERPC)
	{
		.id = LZMA_FILTER_POWERPC,
		.non_last_ok = true,
		.last_ok = false,
		.changes_size = false,
	},
#endif
#ifdef HAVE_DECODER_IA64
	{
		.id = LZMA_FILTER_IA64,
		.non_last_ok = true,
		.last_ok = false,
		.changes_size = false,
	},
#endif
#if defined(HAVE_ENCODER_ARM) || defined(HAVE_DECODER_ARM)
	{
		.id = LZMA_FILTER_ARM,
		.non_last_ok = true,
		.last_ok = false,
		.changes_size = false,
	},
#endif
#if defined(HAVE_ENCODER_ARMTHUMB) || defined(HAVE_DECODER_ARMTHUMB)
	{
		.id = LZMA_FILTER_ARMTHUMB,
		.non_last_ok = true,
		.last_ok = false,
		.changes_size = false,
	},
#endif
#if defined(HAVE_ENCODER_SPARC) || defined(HAVE_DECODER_SPARC)
	{
		.id = LZMA_FILTER_SPARC,
		.non_last_ok = true,
		.last_ok = false,
		.changes_size = false,
	},
#endif
#if defined(HAVE_ENCODER_DELTA) || defined(HAVE_DECODER_DELTA)
	{
		.id = LZMA_FILTER_DELTA,
		.non_last_ok = true,
		.last_ok = false,
		.changes_size = false,
	},
#endif
	{
		.id = LZMA_VLI_VALUE_UNKNOWN
	}
};


static lzma_ret
validate_chain(const lzma_filter *filters, size_t *count)
{
	// There must be at least one filter.
	if (filters == NULL || filters[0].id == LZMA_VLI_VALUE_UNKNOWN)
		return LZMA_PROG_ERROR;

	// Number of non-last filters that may change the size of the data
	// significantly (that is, more than 1-2 % or so).
	size_t changes_size_count = 0;

	// True if it is OK to add a new filter after the current filter.
	bool non_last_ok = true;

	// True if the last filter in the given chain is actually usable as
	// the last filter. Only filters that support embedding End of Payload
	// Marker can be used as the last filter in the chain.
	bool last_ok = false;

	size_t i = 0;
	do {
		size_t j;
		for (j = 0; filters[i].id != features[j].id; ++j)
			if (features[j].id == LZMA_VLI_VALUE_UNKNOWN)
				return LZMA_HEADER_ERROR;

		// If the previous filter in the chain cannot be a non-last
		// filter, the chain is invalid.
		if (!non_last_ok)
			return LZMA_HEADER_ERROR;

		non_last_ok = features[j].non_last_ok;
		last_ok = features[j].last_ok;
		changes_size_count += features[j].changes_size;

	} while (filters[++i].id != LZMA_VLI_VALUE_UNKNOWN);

	// There must be 1-4 filters. The last filter must be usable as
	// the last filter in the chain. At maximum of three filters are
	// allowed to change the size of the data.
	if (i > LZMA_BLOCK_FILTERS_MAX || !last_ok || changes_size_count > 3)
		return LZMA_HEADER_ERROR;

	*count = i;
	return LZMA_OK;
}


extern lzma_ret
lzma_raw_coder_init(lzma_next_coder *next, lzma_allocator *allocator,
		const lzma_filter *options,
		lzma_filter_find coder_find, bool is_encoder)
{
	// Do some basic validation and get the number of filters.
	size_t count;
	return_if_error(validate_chain(options, &count));

	// Set the filter functions and copy the options pointer.
	lzma_filter_info filters[LZMA_BLOCK_FILTERS_MAX + 1];
	if (is_encoder) {
		for (size_t i = 0; i < count; ++i) {
			// The order of the filters is reversed in the
			// encoder. It allows more efficient handling
			// of the uncompressed data.
			const size_t j = count - i - 1;

			const lzma_filter_coder *const fc
					= coder_find(options[i].id);
			if (fc == NULL || fc->init == NULL)
				return LZMA_HEADER_ERROR;

			filters[j].init = fc->init;
			filters[j].options = options[i].options;
		}
	} else {
		for (size_t i = 0; i < count; ++i) {
			const lzma_filter_coder *const fc
					= coder_find(options[i].id);
			if (fc == NULL || fc->init == NULL)
				return LZMA_HEADER_ERROR;

			filters[i].init = fc->init;
			filters[i].options = options[i].options;
		}
	}

	// Terminate the array.
	filters[count].init = NULL;

	// Initialize the filters.
	const lzma_ret ret = lzma_next_filter_init(next, allocator, filters);
	if (ret != LZMA_OK)
		lzma_next_end(next, allocator);

	return ret;
}


extern uint64_t
lzma_memusage_coder(lzma_filter_find coder_find,
		const lzma_filter *filters)
{
	// The chain has to have at least one filter.
	if (filters[0].id == LZMA_VLI_VALUE_UNKNOWN)
		return UINT64_MAX;

	uint64_t total = 0;
	size_t i = 0;

	do {
		const lzma_filter_coder *const fc
				 = coder_find(filters[i].id);
		if (fc == NULL)
			return UINT64_MAX; // Unsupported Filter ID

		if (fc->memusage == NULL) {
			// This filter doesn't have a function to calculate
			// the memory usage. Such filters need only little
			// memory, so we use 1 KiB as a good estimate.
			total += 1024;
		} else {
			// Call the filter-specific memory usage calculation
			// function.
			const uint64_t usage
					= fc->memusage(filters[i].options);
			if (usage == UINT64_MAX)
				return UINT64_MAX; // Invalid options

			total += usage;
		}
	} while (filters[++i].id != LZMA_VLI_VALUE_UNKNOWN);

	// Add some fixed amount of extra. It's to compensate memory usage
	// of Stream, Block etc. coders, malloc() overhead, stack etc.
	return total + (1U << 15);
}
