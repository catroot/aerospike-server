/*
 * ai_btree.h
 *
 * Copyright (C) 2013-2014 Aerospike, Inc.
 *
 * Portions may be licensed to Aerospike, Inc. under one or more contributor
 * license agreements.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 */

#pragma once

#include "base/secondary_index.h"

#include "ai_obj.h"
#include <citrusleaf/cf_ll.h>

// NB: Note that sindex_kvs_arr and dig_arr has to have similar
// overlapping order of things ... do not change the array size
// or structure if you do not know what you are doing 
// Both are used by aggregation code
#define NUM_DIGS_PER_ARR 51
#define NUM_SINDEX_KV_PER_ARR NUM_DIGS_PER_ARR

typedef struct dig_arr_t { 
	uint32_t  num;
	cf_digest digs[NUM_DIGS_PER_ARR];
} __attribute__ ((packed)) dig_arr_t;

typedef struct sindex_kv_arr_s { //NOTE: this data structure MUST be 1KB exactly
	uint32_t      num;
	cf_digest     digs[NUM_SINDEX_KV_PER_ARR];	
	as_sindex_key skeys[NUM_SINDEX_KV_PER_ARR];
} __attribute__ ((packed)) sindex_kv_arr;


typedef struct ll_recl_element_s {
	cf_ll_element   ele;
	dig_arr_t     * dig_arr;
} ll_recl_element;


typedef struct ll_sindex_kv_element_s {
	cf_ll_element   ele;
	sindex_kv_arr * skv_arr;
} ll_sindex_kv_element;

void releaseDigArrToQueue(void *v);

void release_skv_arr_to_queue(sindex_kv_arr * v);

int ai_findandset_imatch(as_sindex_metadata *imd, as_sindex_pmetadata *pimd, int idx);

void ai_btree_init(void);

int ai_btree_create(as_sindex_metadata *imd, int simatch, int *bimatch, int nprts);

int ai_btree_destroy(as_sindex_metadata *imd);

int ai_btree_put(as_sindex_metadata *imd, as_sindex_pmetadata *pimd, void *key, cf_digest *value);

int ai_btree_delete(as_sindex_metadata *imd, as_sindex_pmetadata *pimd, void *key, cf_digest *val);

int ai_btree_query(as_sindex_metadata *imd, as_sindex_range *range, as_sindex_qctx *qctx);

int ai_btree_describe(as_sindex_metadata *imd);

uint64_t ai_btree_get_isize(as_sindex_metadata *imd);

uint64_t ai_btree_get_nsize(as_sindex_metadata *imd);

int ai_btree_list(char *ns, char *set, as_sindex_metadata **imds, int *num_indexes);

int ai_btree_list_ns(char *ns, as_sindex_metadata **imds, int *num_indexes);

uint ai_btree_remove_partition(as_sindex_metadata *imd, as_partition_id partition_id, uint batch_size);

uint64_t ai_btree_get_numkeys(as_sindex_metadata *imd);

int ai_btree_dump(char *ns_name, char *setname, char *fname);

int ai_btree_get_simatch_byname(char *nsname, char *iname);

int ai_btree_get_simatch_by_binid(as_namespace *ns, char *set, int binid, bool isw);

void ai_set_simatch_by_name(char *ns, char *iname, int *imatch, int *simatch);

int ai_btree_numindex(void);

void ai_post_append_only_file_init(int nprts);

int ai_post_index_creation_setup_metadata(as_sindex_metadata *imd, as_sindex_pmetadata *pimd, int simatch, int bimatch, int idx);

int ai_btree_build_defrag_list(as_sindex_metadata *imd, as_sindex_pmetadata *pimd, struct ai_obj *icol, long *nofst, long lim, uint64_t * tot_processed, uint64_t * tot_found, cf_ll *apk2d);

bool ai_btree_defrag_list(as_sindex_metadata *imd, as_sindex_pmetadata *pimd, cf_ll *apk2d, ulong n2del, ulong *deleted);

int ai_btree_key_hash_from_sbin(as_sindex_metadata *imd, as_sindex_bin_data *sbin);

int ai_btree_key_hash(as_sindex_metadata *imd, void *skey);

int ai_post_index_creation_setup_pmetadata(as_sindex_metadata *imd, as_sindex_pmetadata *pimd, int simatch, int idx);

