//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

/* Copyright (c) 2011 The LevelDB Authors. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file. See the AUTHORS file for names of contributors.

  C bindings for rocksdb.  May be useful as a stable ABI that can be
  used by programs that keep rocksdb in a shared library, or for
  a JNI api.

  Does not support:
  . getters for the option types
  . custom comparators that implement key shortening
  . capturing post-write-snapshot
  . custom iter, db, env, cache implementations using just the C bindings

  Some conventions:

  (1) We expose just opaque struct pointers and functions to clients.
  This allows us to change internal representations without having to
  recompile clients.

  (2) For simplicity, there is no equivalent to the Slice type.  Instead,
  the caller has to pass the pointer and length as separate
  arguments.

  (3) Errors are represented by a null-terminated c string.  NULL
  means no error.  All operations that can raise an error are passed
  a "char** errptr" as the last argument.  One of the following must
  be true on entry:
     *errptr == NULL
     *errptr points to a malloc()ed null-terminated error message
  On success, a leveldb routine leaves *errptr unchanged.
  On failure, leveldb frees the old value of *errptr and
  set *errptr to a malloc()ed error message.

  (4) Bools have the type unsigned char (0 == false; rest == true)

  (5) All of the pointer arguments must be non-NULL.
*/

#ifndef STORAGE_ROCKSDB_INCLUDE_C_H_
#define STORAGE_ROCKSDB_INCLUDE_C_H_

#pragma once

#ifdef _WIN32
#ifdef ROCKSDB_DLL
#ifdef ROCKSDB_LIBRARY_EXPORTS
#define ROCKSDB_LIBRARY_API __declspec(dllexport)
#else
#define ROCKSDB_LIBRARY_API __declspec(dllimport)
#endif
#else
#define ROCKSDB_LIBRARY_API
#endif
#else
#define ROCKSDB_LIBRARY_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

/* Exported types */

typedef struct rocksdb_silk_t                 rocksdb_silk_t;
typedef struct rocksdb_silk_backup_engine_t   rocksdb_silk_backup_engine_t;
typedef struct rocksdb_silk_backup_engine_info_t   rocksdb_silk_backup_engine_info_t;
typedef struct rocksdb_silk_restore_options_t rocksdb_silk_restore_options_t;
typedef struct rocksdb_silk_cache_t           rocksdb_silk_cache_t;
typedef struct rocksdb_silk_compactionfilter_t rocksdb_silk_compactionfilter_t;
typedef struct rocksdb_silk_compactionfiltercontext_t
    rocksdb_silk_compactionfiltercontext_t;
typedef struct rocksdb_silk_compactionfilterfactory_t
    rocksdb_silk_compactionfilterfactory_t;
typedef struct rocksdb_silk_comparator_t      rocksdb_silk_comparator_t;
typedef struct rocksdb_silk_dbpath_t          rocksdb_silk_dbpath_t;
typedef struct rocksdb_silk_env_t             rocksdb_silk_env_t;
typedef struct rocksdb_silk_fifo_compaction_options_t rocksdb_silk_fifo_compaction_options_t;
typedef struct rocksdb_silk_filelock_t        rocksdb_silk_filelock_t;
typedef struct rocksdb_silk_filterpolicy_t    rocksdb_silk_filterpolicy_t;
typedef struct rocksdb_silk_flushoptions_t    rocksdb_silk_flushoptions_t;
typedef struct rocksdb_silk_iterator_t        rocksdb_silk_iterator_t;
typedef struct rocksdb_silk_logger_t          rocksdb_silk_logger_t;
typedef struct rocksdb_silk_mergeoperator_t   rocksdb_silk_mergeoperator_t;
typedef struct rocksdb_silk_options_t         rocksdb_silk_options_t;
typedef struct rocksdb_silk_compactoptions_t rocksdb_silk_compactoptions_t;
typedef struct rocksdb_silk_block_based_table_options_t
    rocksdb_silk_block_based_table_options_t;
typedef struct rocksdb_silk_cuckoo_table_options_t
    rocksdb_silk_cuckoo_table_options_t;
typedef struct rocksdb_silk_randomfile_t      rocksdb_silk_randomfile_t;
typedef struct rocksdb_silk_readoptions_t     rocksdb_silk_readoptions_t;
typedef struct rocksdb_silk_seqfile_t         rocksdb_silk_seqfile_t;
typedef struct rocksdb_silk_slicetransform_t  rocksdb_silk_slicetransform_t;
typedef struct rocksdb_silk_snapshot_t        rocksdb_silk_snapshot_t;
typedef struct rocksdb_silk_writablefile_t    rocksdb_silk_writablefile_t;
typedef struct rocksdb_silk_writebatch_t      rocksdb_silk_writebatch_t;
typedef struct rocksdb_silk_writebatch_wi_t   rocksdb_silk_writebatch_wi_t;
typedef struct rocksdb_silk_writeoptions_t    rocksdb_silk_writeoptions_t;
typedef struct rocksdb_silk_universal_compaction_options_t rocksdb_silk_universal_compaction_options_t;
typedef struct rocksdb_silk_livefiles_t     rocksdb_silk_livefiles_t;
typedef struct rocksdb_silk_column_family_handle_t rocksdb_silk_column_family_handle_t;
typedef struct rocksdb_silk_envoptions_t      rocksdb_silk_envoptions_t;
typedef struct rocksdb_silk_ingestexternalfileoptions_t rocksdb_silk_ingestexternalfileoptions_t;
typedef struct rocksdb_silk_sstfilewriter_t   rocksdb_silk_sstfilewriter_t;
typedef struct rocksdb_silk_ratelimiter_t     rocksdb_silk_ratelimiter_t;
typedef struct rocksdb_silk_pinnableslice_t rocksdb_silk_pinnableslice_t;
typedef struct rocksdb_silk_transactiondb_options_t rocksdb_silk_transactiondb_options_t;
typedef struct rocksdb_silk_transactiondb_t rocksdb_silk_transactiondb_t;
typedef struct rocksdb_silk_transaction_options_t rocksdb_silk_transaction_options_t;
typedef struct rocksdb_silk_transaction_t rocksdb_silk_transaction_t;
typedef struct rocksdb_silk_checkpoint_t rocksdb_silk_checkpoint_t;

/* DB operations */

extern ROCKSDB_LIBRARY_API rocksdb_silk_t* rocksdb_silk_open(
    const rocksdb_silk_options_t* options, const char* name, char** errptr);

extern ROCKSDB_LIBRARY_API rocksdb_silk_t* rocksdb_silk_open_for_read_only(
    const rocksdb_silk_options_t* options, const char* name,
    unsigned char error_if_log_file_exist, char** errptr);

extern ROCKSDB_LIBRARY_API rocksdb_silk_backup_engine_t* rocksdb_silk_backup_engine_open(
    const rocksdb_silk_options_t* options, const char* path, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_backup_engine_create_new_backup(
    rocksdb_silk_backup_engine_t* be, rocksdb_silk_t* db, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_backup_engine_purge_old_backups(
    rocksdb_silk_backup_engine_t* be, uint32_t num_backups_to_keep, char** errptr);

extern ROCKSDB_LIBRARY_API rocksdb_silk_restore_options_t*
rocksdb_silk_restore_options_create();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_restore_options_destroy(
    rocksdb_silk_restore_options_t* opt);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_restore_options_set_keep_log_files(
    rocksdb_silk_restore_options_t* opt, int v);

extern ROCKSDB_LIBRARY_API void
rocksdb_silk_backup_engine_restore_db_from_latest_backup(
    rocksdb_silk_backup_engine_t* be, const char* db_dir, const char* wal_dir,
    const rocksdb_silk_restore_options_t* restore_options, char** errptr);

extern ROCKSDB_LIBRARY_API const rocksdb_silk_backup_engine_info_t*
rocksdb_silk_backup_engine_get_backup_info(rocksdb_silk_backup_engine_t* be);

extern ROCKSDB_LIBRARY_API int rocksdb_silk_backup_engine_info_count(
    const rocksdb_silk_backup_engine_info_t* info);

extern ROCKSDB_LIBRARY_API int64_t
rocksdb_silk_backup_engine_info_timestamp(const rocksdb_silk_backup_engine_info_t* info,
                                     int index);

extern ROCKSDB_LIBRARY_API uint32_t
rocksdb_silk_backup_engine_info_backup_id(const rocksdb_silk_backup_engine_info_t* info,
                                     int index);

extern ROCKSDB_LIBRARY_API uint64_t
rocksdb_silk_backup_engine_info_size(const rocksdb_silk_backup_engine_info_t* info,
                                int index);

extern ROCKSDB_LIBRARY_API uint32_t rocksdb_silk_backup_engine_info_number_files(
    const rocksdb_silk_backup_engine_info_t* info, int index);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_backup_engine_info_destroy(
    const rocksdb_silk_backup_engine_info_t* info);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_backup_engine_close(
    rocksdb_silk_backup_engine_t* be);

extern ROCKSDB_LIBRARY_API rocksdb_silk_checkpoint_t*
rocksdb_silk_checkpoint_object_create(rocksdb_silk_t* db, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_checkpoint_create(
    rocksdb_silk_checkpoint_t* checkpoint, const char* checkpoint_dir,
    uint64_t log_size_for_flush, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_checkpoint_object_destroy(
    rocksdb_silk_checkpoint_t* checkpoint);

extern ROCKSDB_LIBRARY_API rocksdb_silk_t* rocksdb_silk_open_column_families(
    const rocksdb_silk_options_t* options, const char* name, int num_column_families,
    const char** column_family_names,
    const rocksdb_silk_options_t** column_family_options,
    rocksdb_silk_column_family_handle_t** column_family_handles, char** errptr);

extern ROCKSDB_LIBRARY_API rocksdb_silk_t*
rocksdb_silk_open_for_read_only_column_families(
    const rocksdb_silk_options_t* options, const char* name, int num_column_families,
    const char** column_family_names,
    const rocksdb_silk_options_t** column_family_options,
    rocksdb_silk_column_family_handle_t** column_family_handles,
    unsigned char error_if_log_file_exist, char** errptr);

extern ROCKSDB_LIBRARY_API char** rocksdb_silk_list_column_families(
    const rocksdb_silk_options_t* options, const char* name, size_t* lencf,
    char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_list_column_families_destroy(
    char** list, size_t len);

extern ROCKSDB_LIBRARY_API rocksdb_silk_column_family_handle_t*
rocksdb_silk_create_column_family(rocksdb_silk_t* db,
                             const rocksdb_silk_options_t* column_family_options,
                             const char* column_family_name, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_drop_column_family(
    rocksdb_silk_t* db, rocksdb_silk_column_family_handle_t* handle, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_column_family_handle_destroy(
    rocksdb_silk_column_family_handle_t*);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_close(rocksdb_silk_t* db);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_put(
    rocksdb_silk_t* db, const rocksdb_silk_writeoptions_t* options, const char* key,
    size_t keylen, const char* val, size_t vallen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_put_cf(
    rocksdb_silk_t* db, const rocksdb_silk_writeoptions_t* options,
    rocksdb_silk_column_family_handle_t* column_family, const char* key,
    size_t keylen, const char* val, size_t vallen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_delete(
    rocksdb_silk_t* db, const rocksdb_silk_writeoptions_t* options, const char* key,
    size_t keylen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_delete_cf(
    rocksdb_silk_t* db, const rocksdb_silk_writeoptions_t* options,
    rocksdb_silk_column_family_handle_t* column_family, const char* key,
    size_t keylen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_merge(
    rocksdb_silk_t* db, const rocksdb_silk_writeoptions_t* options, const char* key,
    size_t keylen, const char* val, size_t vallen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_merge_cf(
    rocksdb_silk_t* db, const rocksdb_silk_writeoptions_t* options,
    rocksdb_silk_column_family_handle_t* column_family, const char* key,
    size_t keylen, const char* val, size_t vallen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_write(
    rocksdb_silk_t* db, const rocksdb_silk_writeoptions_t* options,
    rocksdb_silk_writebatch_t* batch, char** errptr);

/* Returns NULL if not found.  A malloc()ed array otherwise.
   Stores the length of the array in *vallen. */
extern ROCKSDB_LIBRARY_API char* rocksdb_silk_get(
    rocksdb_silk_t* db, const rocksdb_silk_readoptions_t* options, const char* key,
    size_t keylen, size_t* vallen, char** errptr);

extern ROCKSDB_LIBRARY_API char* rocksdb_silk_get_cf(
    rocksdb_silk_t* db, const rocksdb_silk_readoptions_t* options,
    rocksdb_silk_column_family_handle_t* column_family, const char* key,
    size_t keylen, size_t* vallen, char** errptr);

// if values_list[i] == NULL and errs[i] == NULL,
// then we got status.IsNotFound(), which we will not return.
// all errors except status status.ok() and status.IsNotFound() are returned.
//
// errs, values_list and values_list_sizes must be num_keys in length,
// allocated by the caller.
// errs is a list of strings as opposed to the conventional one error,
// where errs[i] is the status for retrieval of keys_list[i].
// each non-NULL errs entry is a malloc()ed, null terminated string.
// each non-NULL values_list entry is a malloc()ed array, with
// the length for each stored in values_list_sizes[i].
extern ROCKSDB_LIBRARY_API void rocksdb_silk_multi_get(
    rocksdb_silk_t* db, const rocksdb_silk_readoptions_t* options, size_t num_keys,
    const char* const* keys_list, const size_t* keys_list_sizes,
    char** values_list, size_t* values_list_sizes, char** errs);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_multi_get_cf(
    rocksdb_silk_t* db, const rocksdb_silk_readoptions_t* options,
    const rocksdb_silk_column_family_handle_t* const* column_families,
    size_t num_keys, const char* const* keys_list,
    const size_t* keys_list_sizes, char** values_list,
    size_t* values_list_sizes, char** errs);

extern ROCKSDB_LIBRARY_API rocksdb_silk_iterator_t* rocksdb_silk_create_iterator(
    rocksdb_silk_t* db, const rocksdb_silk_readoptions_t* options);

extern ROCKSDB_LIBRARY_API rocksdb_silk_iterator_t* rocksdb_silk_create_iterator_cf(
    rocksdb_silk_t* db, const rocksdb_silk_readoptions_t* options,
    rocksdb_silk_column_family_handle_t* column_family);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_create_iterators(
    rocksdb_silk_t *db, rocksdb_silk_readoptions_t* opts,
    rocksdb_silk_column_family_handle_t** column_families,
    rocksdb_silk_iterator_t** iterators, size_t size, char** errptr);

extern ROCKSDB_LIBRARY_API const rocksdb_silk_snapshot_t* rocksdb_silk_create_snapshot(
    rocksdb_silk_t* db);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_release_snapshot(
    rocksdb_silk_t* db, const rocksdb_silk_snapshot_t* snapshot);

/* Returns NULL if property name is unknown.
   Else returns a pointer to a malloc()-ed null-terminated value. */
extern ROCKSDB_LIBRARY_API char* rocksdb_silk_property_value(rocksdb_silk_t* db,
                                                        const char* propname);
/* returns 0 on success, -1 otherwise */
int rocksdb_silk_property_int(
    rocksdb_silk_t* db,
    const char* propname, uint64_t *out_val);

extern ROCKSDB_LIBRARY_API char* rocksdb_silk_property_value_cf(
    rocksdb_silk_t* db, rocksdb_silk_column_family_handle_t* column_family,
    const char* propname);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_approximate_sizes(
    rocksdb_silk_t* db, int num_ranges, const char* const* range_start_key,
    const size_t* range_start_key_len, const char* const* range_limit_key,
    const size_t* range_limit_key_len, uint64_t* sizes);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_approximate_sizes_cf(
    rocksdb_silk_t* db, rocksdb_silk_column_family_handle_t* column_family,
    int num_ranges, const char* const* range_start_key,
    const size_t* range_start_key_len, const char* const* range_limit_key,
    const size_t* range_limit_key_len, uint64_t* sizes);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_compact_range(rocksdb_silk_t* db,
                                                      const char* start_key,
                                                      size_t start_key_len,
                                                      const char* limit_key,
                                                      size_t limit_key_len);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_compact_range_cf(
    rocksdb_silk_t* db, rocksdb_silk_column_family_handle_t* column_family,
    const char* start_key, size_t start_key_len, const char* limit_key,
    size_t limit_key_len);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_compact_range_opt(
    rocksdb_silk_t* db, rocksdb_silk_compactoptions_t* opt, const char* start_key,
    size_t start_key_len, const char* limit_key, size_t limit_key_len);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_compact_range_cf_opt(
    rocksdb_silk_t* db, rocksdb_silk_column_family_handle_t* column_family,
    rocksdb_silk_compactoptions_t* opt, const char* start_key, size_t start_key_len,
    const char* limit_key, size_t limit_key_len);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_delete_file(rocksdb_silk_t* db,
                                                    const char* name);

extern ROCKSDB_LIBRARY_API const rocksdb_silk_livefiles_t* rocksdb_silk_livefiles(
    rocksdb_silk_t* db);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_flush(
    rocksdb_silk_t* db, const rocksdb_silk_flushoptions_t* options, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_disable_file_deletions(rocksdb_silk_t* db,
                                                               char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_enable_file_deletions(
    rocksdb_silk_t* db, unsigned char force, char** errptr);

/* Management operations */

extern ROCKSDB_LIBRARY_API void rocksdb_silk_destroy_db(
    const rocksdb_silk_options_t* options, const char* name, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_repair_db(
    const rocksdb_silk_options_t* options, const char* name, char** errptr);

/* Iterator */

extern ROCKSDB_LIBRARY_API void rocksdb_silk_iter_destroy(rocksdb_silk_iterator_t*);
extern ROCKSDB_LIBRARY_API unsigned char rocksdb_silk_iter_valid(
    const rocksdb_silk_iterator_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_iter_seek_to_first(rocksdb_silk_iterator_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_iter_seek_to_last(rocksdb_silk_iterator_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_iter_seek(rocksdb_silk_iterator_t*,
                                                  const char* k, size_t klen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_iter_seek_for_prev(rocksdb_silk_iterator_t*,
                                                           const char* k,
                                                           size_t klen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_iter_next(rocksdb_silk_iterator_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_iter_prev(rocksdb_silk_iterator_t*);
extern ROCKSDB_LIBRARY_API const char* rocksdb_silk_iter_key(
    const rocksdb_silk_iterator_t*, size_t* klen);
extern ROCKSDB_LIBRARY_API const char* rocksdb_silk_iter_value(
    const rocksdb_silk_iterator_t*, size_t* vlen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_iter_get_error(
    const rocksdb_silk_iterator_t*, char** errptr);

/* Write batch */

extern ROCKSDB_LIBRARY_API rocksdb_silk_writebatch_t* rocksdb_silk_writebatch_create();
extern ROCKSDB_LIBRARY_API rocksdb_silk_writebatch_t* rocksdb_silk_writebatch_create_from(
    const char* rep, size_t size);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_destroy(
    rocksdb_silk_writebatch_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_clear(rocksdb_silk_writebatch_t*);
extern ROCKSDB_LIBRARY_API int rocksdb_silk_writebatch_count(rocksdb_silk_writebatch_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_put(rocksdb_silk_writebatch_t*,
                                                       const char* key,
                                                       size_t klen,
                                                       const char* val,
                                                       size_t vlen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_put_cf(
    rocksdb_silk_writebatch_t*, rocksdb_silk_column_family_handle_t* column_family,
    const char* key, size_t klen, const char* val, size_t vlen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_putv(
    rocksdb_silk_writebatch_t* b, int num_keys, const char* const* keys_list,
    const size_t* keys_list_sizes, int num_values,
    const char* const* values_list, const size_t* values_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_putv_cf(
    rocksdb_silk_writebatch_t* b, rocksdb_silk_column_family_handle_t* column_family,
    int num_keys, const char* const* keys_list, const size_t* keys_list_sizes,
    int num_values, const char* const* values_list,
    const size_t* values_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_merge(rocksdb_silk_writebatch_t*,
                                                         const char* key,
                                                         size_t klen,
                                                         const char* val,
                                                         size_t vlen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_merge_cf(
    rocksdb_silk_writebatch_t*, rocksdb_silk_column_family_handle_t* column_family,
    const char* key, size_t klen, const char* val, size_t vlen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_mergev(
    rocksdb_silk_writebatch_t* b, int num_keys, const char* const* keys_list,
    const size_t* keys_list_sizes, int num_values,
    const char* const* values_list, const size_t* values_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_mergev_cf(
    rocksdb_silk_writebatch_t* b, rocksdb_silk_column_family_handle_t* column_family,
    int num_keys, const char* const* keys_list, const size_t* keys_list_sizes,
    int num_values, const char* const* values_list,
    const size_t* values_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_delete(rocksdb_silk_writebatch_t*,
                                                          const char* key,
                                                          size_t klen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_delete_cf(
    rocksdb_silk_writebatch_t*, rocksdb_silk_column_family_handle_t* column_family,
    const char* key, size_t klen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_deletev(
    rocksdb_silk_writebatch_t* b, int num_keys, const char* const* keys_list,
    const size_t* keys_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_deletev_cf(
    rocksdb_silk_writebatch_t* b, rocksdb_silk_column_family_handle_t* column_family,
    int num_keys, const char* const* keys_list, const size_t* keys_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_delete_range(
    rocksdb_silk_writebatch_t* b, const char* start_key, size_t start_key_len,
    const char* end_key, size_t end_key_len);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_delete_range_cf(
    rocksdb_silk_writebatch_t* b, rocksdb_silk_column_family_handle_t* column_family,
    const char* start_key, size_t start_key_len, const char* end_key,
    size_t end_key_len);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_delete_rangev(
    rocksdb_silk_writebatch_t* b, int num_keys, const char* const* start_keys_list,
    const size_t* start_keys_list_sizes, const char* const* end_keys_list,
    const size_t* end_keys_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_delete_rangev_cf(
    rocksdb_silk_writebatch_t* b, rocksdb_silk_column_family_handle_t* column_family,
    int num_keys, const char* const* start_keys_list,
    const size_t* start_keys_list_sizes, const char* const* end_keys_list,
    const size_t* end_keys_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_put_log_data(
    rocksdb_silk_writebatch_t*, const char* blob, size_t len);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_iterate(
    rocksdb_silk_writebatch_t*, void* state,
    void (*put)(void*, const char* k, size_t klen, const char* v, size_t vlen),
    void (*deleted)(void*, const char* k, size_t klen));
extern ROCKSDB_LIBRARY_API const char* rocksdb_silk_writebatch_data(
    rocksdb_silk_writebatch_t*, size_t* size);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_set_save_point(
    rocksdb_silk_writebatch_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_rollback_to_save_point(
    rocksdb_silk_writebatch_t*, char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_pop_save_point(
    rocksdb_silk_writebatch_t*, char** errptr);

/* Write batch with index */

extern ROCKSDB_LIBRARY_API rocksdb_silk_writebatch_wi_t* rocksdb_silk_writebatch_wi_create(
                                                       size_t reserved_bytes,
                                                       unsigned char overwrite_keys);
extern ROCKSDB_LIBRARY_API rocksdb_silk_writebatch_wi_t* rocksdb_silk_writebatch_wi_create_from(
    const char* rep, size_t size);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_destroy(
    rocksdb_silk_writebatch_wi_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_clear(rocksdb_silk_writebatch_wi_t*);
extern ROCKSDB_LIBRARY_API int rocksdb_silk_writebatch_wi_count(rocksdb_silk_writebatch_wi_t* b);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_put(rocksdb_silk_writebatch_wi_t*,
                                                       const char* key,
                                                       size_t klen,
                                                       const char* val,
                                                       size_t vlen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_put_cf(
    rocksdb_silk_writebatch_wi_t*, rocksdb_silk_column_family_handle_t* column_family,
    const char* key, size_t klen, const char* val, size_t vlen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_putv(
    rocksdb_silk_writebatch_wi_t* b, int num_keys, const char* const* keys_list,
    const size_t* keys_list_sizes, int num_values,
    const char* const* values_list, const size_t* values_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_putv_cf(
    rocksdb_silk_writebatch_wi_t* b, rocksdb_silk_column_family_handle_t* column_family,
    int num_keys, const char* const* keys_list, const size_t* keys_list_sizes,
    int num_values, const char* const* values_list,
    const size_t* values_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_merge(rocksdb_silk_writebatch_wi_t*,
                                                         const char* key,
                                                         size_t klen,
                                                         const char* val,
                                                         size_t vlen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_merge_cf(
    rocksdb_silk_writebatch_wi_t*, rocksdb_silk_column_family_handle_t* column_family,
    const char* key, size_t klen, const char* val, size_t vlen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_mergev(
    rocksdb_silk_writebatch_wi_t* b, int num_keys, const char* const* keys_list,
    const size_t* keys_list_sizes, int num_values,
    const char* const* values_list, const size_t* values_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_mergev_cf(
    rocksdb_silk_writebatch_wi_t* b, rocksdb_silk_column_family_handle_t* column_family,
    int num_keys, const char* const* keys_list, const size_t* keys_list_sizes,
    int num_values, const char* const* values_list,
    const size_t* values_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_delete(rocksdb_silk_writebatch_wi_t*,
                                                          const char* key,
                                                          size_t klen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_delete_cf(
    rocksdb_silk_writebatch_wi_t*, rocksdb_silk_column_family_handle_t* column_family,
    const char* key, size_t klen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_deletev(
    rocksdb_silk_writebatch_wi_t* b, int num_keys, const char* const* keys_list,
    const size_t* keys_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_deletev_cf(
    rocksdb_silk_writebatch_wi_t* b, rocksdb_silk_column_family_handle_t* column_family,
    int num_keys, const char* const* keys_list, const size_t* keys_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_delete_range(
    rocksdb_silk_writebatch_wi_t* b, const char* start_key, size_t start_key_len,
    const char* end_key, size_t end_key_len);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_delete_range_cf(
    rocksdb_silk_writebatch_wi_t* b, rocksdb_silk_column_family_handle_t* column_family,
    const char* start_key, size_t start_key_len, const char* end_key,
    size_t end_key_len);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_delete_rangev(
    rocksdb_silk_writebatch_wi_t* b, int num_keys, const char* const* start_keys_list,
    const size_t* start_keys_list_sizes, const char* const* end_keys_list,
    const size_t* end_keys_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_delete_rangev_cf(
    rocksdb_silk_writebatch_wi_t* b, rocksdb_silk_column_family_handle_t* column_family,
    int num_keys, const char* const* start_keys_list,
    const size_t* start_keys_list_sizes, const char* const* end_keys_list,
    const size_t* end_keys_list_sizes);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_put_log_data(
    rocksdb_silk_writebatch_wi_t*, const char* blob, size_t len);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_iterate(
    rocksdb_silk_writebatch_wi_t* b,
    void* state,
    void (*put)(void*, const char* k, size_t klen, const char* v, size_t vlen),
    void (*deleted)(void*, const char* k, size_t klen));
extern ROCKSDB_LIBRARY_API const char* rocksdb_silk_writebatch_wi_data(
    rocksdb_silk_writebatch_wi_t* b,
    size_t* size);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_set_save_point(
    rocksdb_silk_writebatch_wi_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writebatch_wi_rollback_to_save_point(
    rocksdb_silk_writebatch_wi_t*, char** errptr);
extern ROCKSDB_LIBRARY_API char* rocksdb_silk_writebatch_wi_get_from_batch(
    rocksdb_silk_writebatch_wi_t* wbwi,
    const rocksdb_silk_options_t* options,
    const char* key, size_t keylen,
    size_t* vallen,
    char** errptr);
extern ROCKSDB_LIBRARY_API char* rocksdb_silk_writebatch_wi_get_from_batch_cf(
    rocksdb_silk_writebatch_wi_t* wbwi,
    const rocksdb_silk_options_t* options,
    rocksdb_silk_column_family_handle_t* column_family,
    const char* key, size_t keylen,
    size_t* vallen,
    char** errptr);
extern ROCKSDB_LIBRARY_API char* rocksdb_silk_writebatch_wi_get_from_batch_and_db(
    rocksdb_silk_writebatch_wi_t* wbwi,
    rocksdb_silk_t* db,
    const rocksdb_silk_readoptions_t* options,
    const char* key, size_t keylen,
    size_t* vallen,
    char** errptr);
extern ROCKSDB_LIBRARY_API char* rocksdb_silk_writebatch_wi_get_from_batch_and_db_cf(
    rocksdb_silk_writebatch_wi_t* wbwi,
    rocksdb_silk_t* db,
    const rocksdb_silk_readoptions_t* options,
    rocksdb_silk_column_family_handle_t* column_family,
    const char* key, size_t keylen,
    size_t* vallen,
    char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_write_writebatch_wi(
    rocksdb_silk_t* db,
    const rocksdb_silk_writeoptions_t* options,
    rocksdb_silk_writebatch_wi_t* wbwi,
    char** errptr);
extern ROCKSDB_LIBRARY_API rocksdb_silk_iterator_t* rocksdb_silk_writebatch_wi_create_iterator_with_base(
    rocksdb_silk_writebatch_wi_t* wbwi,
    rocksdb_silk_iterator_t* base_iterator);
extern ROCKSDB_LIBRARY_API rocksdb_silk_iterator_t* rocksdb_silk_writebatch_wi_create_iterator_with_base_cf(
    rocksdb_silk_writebatch_wi_t* wbwi,
    rocksdb_silk_iterator_t* base_iterator,
    rocksdb_silk_column_family_handle_t* cf);


/* Block based table options */

extern ROCKSDB_LIBRARY_API rocksdb_silk_block_based_table_options_t*
rocksdb_silk_block_based_options_create();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_block_based_options_destroy(
    rocksdb_silk_block_based_table_options_t* options);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_block_based_options_set_block_size(
    rocksdb_silk_block_based_table_options_t* options, size_t block_size);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_block_based_options_set_block_size_deviation(
    rocksdb_silk_block_based_table_options_t* options, int block_size_deviation);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_block_based_options_set_block_restart_interval(
    rocksdb_silk_block_based_table_options_t* options, int block_restart_interval);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_block_based_options_set_filter_policy(
    rocksdb_silk_block_based_table_options_t* options,
    rocksdb_silk_filterpolicy_t* filter_policy);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_block_based_options_set_no_block_cache(
    rocksdb_silk_block_based_table_options_t* options, unsigned char no_block_cache);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_block_based_options_set_block_cache(
    rocksdb_silk_block_based_table_options_t* options, rocksdb_silk_cache_t* block_cache);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_block_based_options_set_block_cache_compressed(
    rocksdb_silk_block_based_table_options_t* options,
    rocksdb_silk_cache_t* block_cache_compressed);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_block_based_options_set_whole_key_filtering(
    rocksdb_silk_block_based_table_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_block_based_options_set_format_version(
    rocksdb_silk_block_based_table_options_t*, int);
enum {
  rocksdb_silk_block_based_table_index_type_binary_search = 0,
  rocksdb_silk_block_based_table_index_type_hash_search = 1,
  rocksdb_silk_block_based_table_index_type_two_level_index_search = 2,
};
extern ROCKSDB_LIBRARY_API void rocksdb_silk_block_based_options_set_index_type(
    rocksdb_silk_block_based_table_options_t*, int);  // uses one of the above enums
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_block_based_options_set_hash_index_allow_collision(
    rocksdb_silk_block_based_table_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_block_based_options_set_cache_index_and_filter_blocks(
    rocksdb_silk_block_based_table_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_block_based_options_set_pin_l0_filter_and_index_blocks_in_cache(
    rocksdb_silk_block_based_table_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_block_based_table_factory(
    rocksdb_silk_options_t* opt, rocksdb_silk_block_based_table_options_t* table_options);

/* Cuckoo table options */

extern ROCKSDB_LIBRARY_API rocksdb_silk_cuckoo_table_options_t*
rocksdb_silk_cuckoo_options_create();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_cuckoo_options_destroy(
    rocksdb_silk_cuckoo_table_options_t* options);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_cuckoo_options_set_hash_ratio(
    rocksdb_silk_cuckoo_table_options_t* options, double v);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_cuckoo_options_set_max_search_depth(
    rocksdb_silk_cuckoo_table_options_t* options, uint32_t v);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_cuckoo_options_set_cuckoo_block_size(
    rocksdb_silk_cuckoo_table_options_t* options, uint32_t v);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_cuckoo_options_set_identity_as_first_hash(
    rocksdb_silk_cuckoo_table_options_t* options, unsigned char v);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_cuckoo_options_set_use_module_hash(
    rocksdb_silk_cuckoo_table_options_t* options, unsigned char v);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_cuckoo_table_factory(
    rocksdb_silk_options_t* opt, rocksdb_silk_cuckoo_table_options_t* table_options);

/* Options */
extern ROCKSDB_LIBRARY_API void rocksdb_silk_set_options(
    rocksdb_silk_t* db, int count, const char* const keys[], const char* const values[], char** errptr);

extern ROCKSDB_LIBRARY_API rocksdb_silk_options_t* rocksdb_silk_options_create();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_destroy(rocksdb_silk_options_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_increase_parallelism(
    rocksdb_silk_options_t* opt, int total_threads);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_optimize_for_point_lookup(
    rocksdb_silk_options_t* opt, uint64_t block_cache_size_mb);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_optimize_level_style_compaction(
    rocksdb_silk_options_t* opt, uint64_t memtable_memory_budget);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_optimize_universal_style_compaction(
    rocksdb_silk_options_t* opt, uint64_t memtable_memory_budget);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_compaction_filter(
    rocksdb_silk_options_t*, rocksdb_silk_compactionfilter_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_compaction_filter_factory(
    rocksdb_silk_options_t*, rocksdb_silk_compactionfilterfactory_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_compaction_readahead_size(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_comparator(
    rocksdb_silk_options_t*, rocksdb_silk_comparator_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_merge_operator(
    rocksdb_silk_options_t*, rocksdb_silk_mergeoperator_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_uint64add_merge_operator(
    rocksdb_silk_options_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_compression_per_level(
    rocksdb_silk_options_t* opt, int* level_values, size_t num_levels);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_create_if_missing(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_create_missing_column_families(rocksdb_silk_options_t*,
                                                   unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_error_if_exists(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_paranoid_checks(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_db_paths(rocksdb_silk_options_t*,
                                                             const rocksdb_silk_dbpath_t** path_values, 
                                                             size_t num_paths);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_env(rocksdb_silk_options_t*,
                                                        rocksdb_silk_env_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_info_log(rocksdb_silk_options_t*,
                                                             rocksdb_silk_logger_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_info_log_level(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_write_buffer_size(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_db_write_buffer_size(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_open_files(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_file_opening_threads(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_total_wal_size(
    rocksdb_silk_options_t* opt, uint64_t n);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_compression_options(
    rocksdb_silk_options_t*, int, int, int, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_prefix_extractor(
    rocksdb_silk_options_t*, rocksdb_silk_slicetransform_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_num_levels(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_level0_file_num_compaction_trigger(rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_level0_slowdown_writes_trigger(rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_level0_stop_writes_trigger(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_mem_compaction_level(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_target_file_size_base(
    rocksdb_silk_options_t*, uint64_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_target_file_size_multiplier(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_bytes_for_level_base(
    rocksdb_silk_options_t*, uint64_t);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_level_compaction_dynamic_level_bytes(rocksdb_silk_options_t*,
                                                         unsigned char);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_max_bytes_for_level_multiplier(rocksdb_silk_options_t*, double);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_max_bytes_for_level_multiplier_additional(
    rocksdb_silk_options_t*, int* level_values, size_t num_levels);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_enable_statistics(
    rocksdb_silk_options_t*);

/* returns a pointer to a malloc()-ed, null terminated string */
extern ROCKSDB_LIBRARY_API char* rocksdb_silk_options_statistics_get_string(
    rocksdb_silk_options_t* opt);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_write_buffer_number(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_min_write_buffer_number_to_merge(rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_max_write_buffer_number_to_maintain(rocksdb_silk_options_t*,
                                                        int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_background_compactions(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_base_background_compactions(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_background_flushes(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_log_file_size(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_log_file_time_to_roll(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_keep_log_file_num(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_recycle_log_file_num(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_soft_rate_limit(
    rocksdb_silk_options_t*, double);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_hard_rate_limit(
    rocksdb_silk_options_t*, double);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_soft_pending_compaction_bytes_limit(
    rocksdb_silk_options_t* opt, size_t v);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_hard_pending_compaction_bytes_limit(
    rocksdb_silk_options_t* opt, size_t v);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_rate_limit_delay_max_milliseconds(rocksdb_silk_options_t*,
                                                      unsigned int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_manifest_file_size(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_table_cache_numshardbits(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_table_cache_remove_scan_count_limit(rocksdb_silk_options_t*,
                                                        int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_arena_block_size(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_use_fsync(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_db_log_dir(
    rocksdb_silk_options_t*, const char*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_wal_dir(rocksdb_silk_options_t*,
                                                            const char*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_WAL_ttl_seconds(
    rocksdb_silk_options_t*, uint64_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_WAL_size_limit_MB(
    rocksdb_silk_options_t*, uint64_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_manifest_preallocation_size(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_purge_redundant_kvs_while_flush(rocksdb_silk_options_t*,
                                                    unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_allow_mmap_reads(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_allow_mmap_writes(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_use_direct_reads(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_use_direct_io_for_flush_and_compaction(rocksdb_silk_options_t*,
                                                           unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_is_fd_close_on_exec(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_skip_log_error_on_recovery(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_stats_dump_period_sec(
    rocksdb_silk_options_t*, unsigned int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_advise_random_on_open(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_access_hint_on_compaction_start(rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_use_adaptive_mutex(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_bytes_per_sync(
    rocksdb_silk_options_t*, uint64_t);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_allow_concurrent_memtable_write(rocksdb_silk_options_t*,
                                                    unsigned char);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_enable_write_thread_adaptive_yield(rocksdb_silk_options_t*,
                                                       unsigned char);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_max_sequential_skip_in_iterations(rocksdb_silk_options_t*,
                                                      uint64_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_disable_auto_compactions(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_optimize_filters_for_hits(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_delete_obsolete_files_period_micros(rocksdb_silk_options_t*,
                                                        uint64_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_prepare_for_bulk_load(
    rocksdb_silk_options_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_memtable_vector_rep(
    rocksdb_silk_options_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_memtable_prefix_bloom_size_ratio(
    rocksdb_silk_options_t*, double);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_compaction_bytes(
    rocksdb_silk_options_t*, uint64_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_hash_skip_list_rep(
    rocksdb_silk_options_t*, size_t, int32_t, int32_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_hash_link_list_rep(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_plain_table_factory(
    rocksdb_silk_options_t*, uint32_t, int, double, size_t);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_min_level_to_compress(
    rocksdb_silk_options_t* opt, int level);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_memtable_huge_page_size(
    rocksdb_silk_options_t*, size_t);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_max_successive_merges(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_bloom_locality(
    rocksdb_silk_options_t*, uint32_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_inplace_update_support(
    rocksdb_silk_options_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_inplace_update_num_locks(
    rocksdb_silk_options_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_report_bg_io_stats(
    rocksdb_silk_options_t*, int);

enum {
  rocksdb_silk_tolerate_corrupted_tail_records_recovery = 0,
  rocksdb_silk_absolute_consistency_recovery = 1,
  rocksdb_silk_point_in_time_recovery = 2,
  rocksdb_silk_skip_any_corrupted_records_recovery = 3
};
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_wal_recovery_mode(
    rocksdb_silk_options_t*, int);

enum {
  rocksdb_silk_no_compression = 0,
  rocksdb_silk_snappy_compression = 1,
  rocksdb_silk_zlib_compression = 2,
  rocksdb_silk_bz2_compression = 3,
  rocksdb_silk_lz4_compression = 4,
  rocksdb_silk_lz4hc_compression = 5,
  rocksdb_silk_xpress_compression = 6,
  rocksdb_silk_zstd_compression = 7
};
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_compression(
    rocksdb_silk_options_t*, int);

enum {
  rocksdb_silk_level_compaction = 0,
  rocksdb_silk_universal_compaction = 1,
  rocksdb_silk_fifo_compaction = 2
};
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_compaction_style(
    rocksdb_silk_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_options_set_universal_compaction_options(
    rocksdb_silk_options_t*, rocksdb_silk_universal_compaction_options_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_fifo_compaction_options(
    rocksdb_silk_options_t* opt, rocksdb_silk_fifo_compaction_options_t* fifo);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_options_set_ratelimiter(
    rocksdb_silk_options_t* opt, rocksdb_silk_ratelimiter_t* limiter);

/* RateLimiter */
extern ROCKSDB_LIBRARY_API rocksdb_silk_ratelimiter_t* rocksdb_silk_ratelimiter_create(
    int64_t rate_bytes_per_sec, int64_t refill_period_us, int32_t fairness);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_ratelimiter_destroy(rocksdb_silk_ratelimiter_t*);

/* Compaction Filter */

extern ROCKSDB_LIBRARY_API rocksdb_silk_compactionfilter_t*
rocksdb_silk_compactionfilter_create(
    void* state, void (*destructor)(void*),
    unsigned char (*filter)(void*, int level, const char* key,
                            size_t key_length, const char* existing_value,
                            size_t value_length, char** new_value,
                            size_t* new_value_length,
                            unsigned char* value_changed),
    const char* (*name)(void*));
extern ROCKSDB_LIBRARY_API void rocksdb_silk_compactionfilter_set_ignore_snapshots(
    rocksdb_silk_compactionfilter_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_compactionfilter_destroy(
    rocksdb_silk_compactionfilter_t*);

/* Compaction Filter Context */

extern ROCKSDB_LIBRARY_API unsigned char
rocksdb_silk_compactionfiltercontext_is_full_compaction(
    rocksdb_silk_compactionfiltercontext_t* context);

extern ROCKSDB_LIBRARY_API unsigned char
rocksdb_silk_compactionfiltercontext_is_manual_compaction(
    rocksdb_silk_compactionfiltercontext_t* context);

/* Compaction Filter Factory */

extern ROCKSDB_LIBRARY_API rocksdb_silk_compactionfilterfactory_t*
rocksdb_silk_compactionfilterfactory_create(
    void* state, void (*destructor)(void*),
    rocksdb_silk_compactionfilter_t* (*create_compaction_filter)(
        void*, rocksdb_silk_compactionfiltercontext_t* context),
    const char* (*name)(void*));
extern ROCKSDB_LIBRARY_API void rocksdb_silk_compactionfilterfactory_destroy(
    rocksdb_silk_compactionfilterfactory_t*);

/* Comparator */

extern ROCKSDB_LIBRARY_API rocksdb_silk_comparator_t* rocksdb_silk_comparator_create(
    void* state, void (*destructor)(void*),
    int (*compare)(void*, const char* a, size_t alen, const char* b,
                   size_t blen),
    const char* (*name)(void*));
extern ROCKSDB_LIBRARY_API void rocksdb_silk_comparator_destroy(
    rocksdb_silk_comparator_t*);

/* Filter policy */

extern ROCKSDB_LIBRARY_API rocksdb_silk_filterpolicy_t* rocksdb_silk_filterpolicy_create(
    void* state, void (*destructor)(void*),
    char* (*create_filter)(void*, const char* const* key_array,
                           const size_t* key_length_array, int num_keys,
                           size_t* filter_length),
    unsigned char (*key_may_match)(void*, const char* key, size_t length,
                                   const char* filter, size_t filter_length),
    void (*delete_filter)(void*, const char* filter, size_t filter_length),
    const char* (*name)(void*));
extern ROCKSDB_LIBRARY_API void rocksdb_silk_filterpolicy_destroy(
    rocksdb_silk_filterpolicy_t*);

extern ROCKSDB_LIBRARY_API rocksdb_silk_filterpolicy_t*
rocksdb_silk_filterpolicy_create_bloom(int bits_per_key);
extern ROCKSDB_LIBRARY_API rocksdb_silk_filterpolicy_t*
rocksdb_silk_filterpolicy_create_bloom_full(int bits_per_key);

/* Merge Operator */

extern ROCKSDB_LIBRARY_API rocksdb_silk_mergeoperator_t*
rocksdb_silk_mergeoperator_create(
    void* state, void (*destructor)(void*),
    char* (*full_merge)(void*, const char* key, size_t key_length,
                        const char* existing_value,
                        size_t existing_value_length,
                        const char* const* operands_list,
                        const size_t* operands_list_length, int num_operands,
                        unsigned char* success, size_t* new_value_length),
    char* (*partial_merge)(void*, const char* key, size_t key_length,
                           const char* const* operands_list,
                           const size_t* operands_list_length, int num_operands,
                           unsigned char* success, size_t* new_value_length),
    void (*delete_value)(void*, const char* value, size_t value_length),
    const char* (*name)(void*));
extern ROCKSDB_LIBRARY_API void rocksdb_silk_mergeoperator_destroy(
    rocksdb_silk_mergeoperator_t*);

/* Read options */

extern ROCKSDB_LIBRARY_API rocksdb_silk_readoptions_t* rocksdb_silk_readoptions_create();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_destroy(
    rocksdb_silk_readoptions_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_set_verify_checksums(
    rocksdb_silk_readoptions_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_set_fill_cache(
    rocksdb_silk_readoptions_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_set_snapshot(
    rocksdb_silk_readoptions_t*, const rocksdb_silk_snapshot_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_set_iterate_upper_bound(
    rocksdb_silk_readoptions_t*, const char* key, size_t keylen);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_set_read_tier(
    rocksdb_silk_readoptions_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_set_tailing(
    rocksdb_silk_readoptions_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_set_readahead_size(
    rocksdb_silk_readoptions_t*, size_t);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_set_pin_data(
    rocksdb_silk_readoptions_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_readoptions_set_total_order_seek(
    rocksdb_silk_readoptions_t*, unsigned char);

/* Write options */

extern ROCKSDB_LIBRARY_API rocksdb_silk_writeoptions_t*
rocksdb_silk_writeoptions_create();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writeoptions_destroy(
    rocksdb_silk_writeoptions_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writeoptions_set_sync(
    rocksdb_silk_writeoptions_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_writeoptions_disable_WAL(
    rocksdb_silk_writeoptions_t* opt, int disable);

/* Compact range options */

extern ROCKSDB_LIBRARY_API rocksdb_silk_compactoptions_t*
rocksdb_silk_compactoptions_create();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_compactoptions_destroy(
    rocksdb_silk_compactoptions_t*);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_compactoptions_set_exclusive_manual_compaction(
    rocksdb_silk_compactoptions_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_compactoptions_set_change_level(
    rocksdb_silk_compactoptions_t*, unsigned char);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_compactoptions_set_target_level(
    rocksdb_silk_compactoptions_t*, int);

/* Flush options */

extern ROCKSDB_LIBRARY_API rocksdb_silk_flushoptions_t*
rocksdb_silk_flushoptions_create();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_flushoptions_destroy(
    rocksdb_silk_flushoptions_t*);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_flushoptions_set_wait(
    rocksdb_silk_flushoptions_t*, unsigned char);

/* Cache */

extern ROCKSDB_LIBRARY_API rocksdb_silk_cache_t* rocksdb_silk_cache_create_lru(
    size_t capacity);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_cache_destroy(rocksdb_silk_cache_t* cache);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_cache_set_capacity(
    rocksdb_silk_cache_t* cache, size_t capacity);
extern ROCKSDB_LIBRARY_API size_t
rocksdb_silk_cache_get_usage(rocksdb_silk_cache_t* cache);
extern ROCKSDB_LIBRARY_API size_t
rocksdb_silk_cache_get_pinned_usage(rocksdb_silk_cache_t* cache);

/* DBPath */

extern ROCKSDB_LIBRARY_API rocksdb_silk_dbpath_t* rocksdb_silk_dbpath_create(const char* path, uint64_t target_size);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_dbpath_destroy(rocksdb_silk_dbpath_t*);

/* Env */

extern ROCKSDB_LIBRARY_API rocksdb_silk_env_t* rocksdb_silk_create_default_env();
extern ROCKSDB_LIBRARY_API rocksdb_silk_env_t* rocksdb_silk_create_mem_env();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_env_set_background_threads(
    rocksdb_silk_env_t* env, int n);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_env_set_high_priority_background_threads(rocksdb_silk_env_t* env, int n);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_env_join_all_threads(
    rocksdb_silk_env_t* env);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_env_destroy(rocksdb_silk_env_t*);

extern ROCKSDB_LIBRARY_API rocksdb_silk_envoptions_t* rocksdb_silk_envoptions_create();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_envoptions_destroy(
    rocksdb_silk_envoptions_t* opt);

/* SstFile */

extern ROCKSDB_LIBRARY_API rocksdb_silk_sstfilewriter_t*
rocksdb_silk_sstfilewriter_create(const rocksdb_silk_envoptions_t* env,
                             const rocksdb_silk_options_t* io_options);
extern ROCKSDB_LIBRARY_API rocksdb_silk_sstfilewriter_t*
rocksdb_silk_sstfilewriter_create_with_comparator(
    const rocksdb_silk_envoptions_t* env, const rocksdb_silk_options_t* io_options,
    const rocksdb_silk_comparator_t* comparator);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_sstfilewriter_open(
    rocksdb_silk_sstfilewriter_t* writer, const char* name, char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_sstfilewriter_add(
    rocksdb_silk_sstfilewriter_t* writer, const char* key, size_t keylen,
    const char* val, size_t vallen, char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_sstfilewriter_put(
    rocksdb_silk_sstfilewriter_t* writer, const char* key, size_t keylen,
    const char* val, size_t vallen, char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_sstfilewriter_merge(
    rocksdb_silk_sstfilewriter_t* writer, const char* key, size_t keylen,
    const char* val, size_t vallen, char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_sstfilewriter_delete(
    rocksdb_silk_sstfilewriter_t* writer, const char* key, size_t keylen,
    char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_sstfilewriter_finish(
    rocksdb_silk_sstfilewriter_t* writer, char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_sstfilewriter_destroy(
    rocksdb_silk_sstfilewriter_t* writer);

extern ROCKSDB_LIBRARY_API rocksdb_silk_ingestexternalfileoptions_t*
rocksdb_silk_ingestexternalfileoptions_create();
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_ingestexternalfileoptions_set_move_files(
    rocksdb_silk_ingestexternalfileoptions_t* opt, unsigned char move_files);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_ingestexternalfileoptions_set_snapshot_consistency(
    rocksdb_silk_ingestexternalfileoptions_t* opt,
    unsigned char snapshot_consistency);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_ingestexternalfileoptions_set_allow_global_seqno(
    rocksdb_silk_ingestexternalfileoptions_t* opt, unsigned char allow_global_seqno);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_ingestexternalfileoptions_set_allow_blocking_flush(
    rocksdb_silk_ingestexternalfileoptions_t* opt,
    unsigned char allow_blocking_flush);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_ingestexternalfileoptions_destroy(
    rocksdb_silk_ingestexternalfileoptions_t* opt);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_ingest_external_file(
    rocksdb_silk_t* db, const char* const* file_list, const size_t list_len,
    const rocksdb_silk_ingestexternalfileoptions_t* opt, char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_ingest_external_file_cf(
    rocksdb_silk_t* db, rocksdb_silk_column_family_handle_t* handle,
    const char* const* file_list, const size_t list_len,
    const rocksdb_silk_ingestexternalfileoptions_t* opt, char** errptr);

/* SliceTransform */

extern ROCKSDB_LIBRARY_API rocksdb_silk_slicetransform_t*
rocksdb_silk_slicetransform_create(
    void* state, void (*destructor)(void*),
    char* (*transform)(void*, const char* key, size_t length,
                       size_t* dst_length),
    unsigned char (*in_domain)(void*, const char* key, size_t length),
    unsigned char (*in_range)(void*, const char* key, size_t length),
    const char* (*name)(void*));
extern ROCKSDB_LIBRARY_API rocksdb_silk_slicetransform_t*
    rocksdb_silk_slicetransform_create_fixed_prefix(size_t);
extern ROCKSDB_LIBRARY_API rocksdb_silk_slicetransform_t*
rocksdb_silk_slicetransform_create_noop();
extern ROCKSDB_LIBRARY_API void rocksdb_silk_slicetransform_destroy(
    rocksdb_silk_slicetransform_t*);

/* Universal Compaction options */

enum {
  rocksdb_silk_similar_size_compaction_stop_style = 0,
  rocksdb_silk_total_size_compaction_stop_style = 1
};

extern ROCKSDB_LIBRARY_API rocksdb_silk_universal_compaction_options_t*
rocksdb_silk_universal_compaction_options_create();
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_universal_compaction_options_set_size_ratio(
    rocksdb_silk_universal_compaction_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_universal_compaction_options_set_min_merge_width(
    rocksdb_silk_universal_compaction_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_universal_compaction_options_set_max_merge_width(
    rocksdb_silk_universal_compaction_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_universal_compaction_options_set_max_size_amplification_percent(
    rocksdb_silk_universal_compaction_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_universal_compaction_options_set_compression_size_percent(
    rocksdb_silk_universal_compaction_options_t*, int);
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_universal_compaction_options_set_stop_style(
    rocksdb_silk_universal_compaction_options_t*, int);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_universal_compaction_options_destroy(
    rocksdb_silk_universal_compaction_options_t*);

extern ROCKSDB_LIBRARY_API rocksdb_silk_fifo_compaction_options_t*
rocksdb_silk_fifo_compaction_options_create();
extern ROCKSDB_LIBRARY_API void
rocksdb_silk_fifo_compaction_options_set_max_table_files_size(
    rocksdb_silk_fifo_compaction_options_t* fifo_opts, uint64_t size);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_fifo_compaction_options_destroy(
    rocksdb_silk_fifo_compaction_options_t* fifo_opts);

extern ROCKSDB_LIBRARY_API int rocksdb_silk_livefiles_count(
    const rocksdb_silk_livefiles_t*);
extern ROCKSDB_LIBRARY_API const char* rocksdb_silk_livefiles_name(
    const rocksdb_silk_livefiles_t*, int index);
extern ROCKSDB_LIBRARY_API int rocksdb_silk_livefiles_level(
    const rocksdb_silk_livefiles_t*, int index);
extern ROCKSDB_LIBRARY_API size_t
rocksdb_silk_livefiles_size(const rocksdb_silk_livefiles_t*, int index);
extern ROCKSDB_LIBRARY_API const char* rocksdb_silk_livefiles_smallestkey(
    const rocksdb_silk_livefiles_t*, int index, size_t* size);
extern ROCKSDB_LIBRARY_API const char* rocksdb_silk_livefiles_largestkey(
    const rocksdb_silk_livefiles_t*, int index, size_t* size);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_livefiles_destroy(
    const rocksdb_silk_livefiles_t*);

/* Utility Helpers */

extern ROCKSDB_LIBRARY_API void rocksdb_silk_get_options_from_string(
    const rocksdb_silk_options_t* base_options, const char* opts_str,
    rocksdb_silk_options_t* new_options, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_delete_file_in_range(
    rocksdb_silk_t* db, const char* start_key, size_t start_key_len,
    const char* limit_key, size_t limit_key_len, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_delete_file_in_range_cf(
    rocksdb_silk_t* db, rocksdb_silk_column_family_handle_t* column_family,
    const char* start_key, size_t start_key_len, const char* limit_key,
    size_t limit_key_len, char** errptr);

/* Transactions */

extern ROCKSDB_LIBRARY_API rocksdb_silk_transactiondb_t* rocksdb_silk_transactiondb_open(
    const rocksdb_silk_options_t* options,
    const rocksdb_silk_transactiondb_options_t* txn_db_options, const char* name,
    char** errptr);

extern ROCKSDB_LIBRARY_API const rocksdb_silk_snapshot_t*
rocksdb_silk_transactiondb_create_snapshot(rocksdb_silk_transactiondb_t* txn_db);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transactiondb_release_snapshot(
    rocksdb_silk_transactiondb_t* txn_db, const rocksdb_silk_snapshot_t* snapshot);

extern ROCKSDB_LIBRARY_API rocksdb_silk_transaction_t* rocksdb_silk_transaction_begin(
    rocksdb_silk_transactiondb_t* txn_db,
    const rocksdb_silk_writeoptions_t* write_options,
    const rocksdb_silk_transaction_options_t* txn_options,
    rocksdb_silk_transaction_t* old_txn);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_commit(
    rocksdb_silk_transaction_t* txn, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_rollback(
    rocksdb_silk_transaction_t* txn, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_destroy(
    rocksdb_silk_transaction_t* txn);

extern ROCKSDB_LIBRARY_API char* rocksdb_silk_transaction_get(
    rocksdb_silk_transaction_t* txn, const rocksdb_silk_readoptions_t* options,
    const char* key, size_t klen, size_t* vlen, char** errptr);

extern ROCKSDB_LIBRARY_API char* rocksdb_silk_transactiondb_get(
    rocksdb_silk_transactiondb_t* txn_db, const rocksdb_silk_readoptions_t* options,
    const char* key, size_t klen, size_t* vlen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_put(
    rocksdb_silk_transaction_t* txn, const char* key, size_t klen, const char* val,
    size_t vlen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transactiondb_put(
    rocksdb_silk_transactiondb_t* txn_db, const rocksdb_silk_writeoptions_t* options,
    const char* key, size_t klen, const char* val, size_t vlen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_delete(
    rocksdb_silk_transaction_t* txn, const char* key, size_t klen, char** errptr);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transactiondb_delete(
    rocksdb_silk_transactiondb_t* txn_db, const rocksdb_silk_writeoptions_t* options,
    const char* key, size_t klen, char** errptr);

extern ROCKSDB_LIBRARY_API rocksdb_silk_iterator_t*
rocksdb_silk_transaction_create_iterator(rocksdb_silk_transaction_t* txn,
                                    const rocksdb_silk_readoptions_t* options);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transactiondb_close(
    rocksdb_silk_transactiondb_t* txn_db);

extern ROCKSDB_LIBRARY_API rocksdb_silk_checkpoint_t*
rocksdb_silk_transactiondb_checkpoint_object_create(rocksdb_silk_transactiondb_t* txn_db,
                                               char** errptr);

/* Transaction Options */

extern ROCKSDB_LIBRARY_API rocksdb_silk_transactiondb_options_t*
rocksdb_silk_transactiondb_options_create();

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transactiondb_options_destroy(
    rocksdb_silk_transactiondb_options_t* opt);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transactiondb_options_set_max_num_locks(
    rocksdb_silk_transactiondb_options_t* opt, int64_t max_num_locks);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transactiondb_options_set_num_stripes(
    rocksdb_silk_transactiondb_options_t* opt, size_t num_stripes);

extern ROCKSDB_LIBRARY_API void
rocksdb_silk_transactiondb_options_set_transaction_lock_timeout(
    rocksdb_silk_transactiondb_options_t* opt, int64_t txn_lock_timeout);

extern ROCKSDB_LIBRARY_API void
rocksdb_silk_transactiondb_options_set_default_lock_timeout(
    rocksdb_silk_transactiondb_options_t* opt, int64_t default_lock_timeout);

extern ROCKSDB_LIBRARY_API rocksdb_silk_transaction_options_t*
rocksdb_silk_transaction_options_create();

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_options_destroy(
    rocksdb_silk_transaction_options_t* opt);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_options_set_set_snapshot(
    rocksdb_silk_transaction_options_t* opt, unsigned char v);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_options_set_deadlock_detect(
    rocksdb_silk_transaction_options_t* opt, unsigned char v);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_options_set_lock_timeout(
    rocksdb_silk_transaction_options_t* opt, int64_t lock_timeout);

extern ROCKSDB_LIBRARY_API void rocksdb_silk_transaction_options_set_expiration(
    rocksdb_silk_transaction_options_t* opt, int64_t expiration);

extern ROCKSDB_LIBRARY_API void
rocksdb_silk_transaction_options_set_deadlock_detect_depth(
    rocksdb_silk_transaction_options_t* opt, int64_t depth);

extern ROCKSDB_LIBRARY_API void
rocksdb_silk_transaction_options_set_max_write_batch_size(
    rocksdb_silk_transaction_options_t* opt, size_t size);

// referring to convention (3), this should be used by client
// to free memory that was malloc()ed
extern ROCKSDB_LIBRARY_API void rocksdb_silk_free(void* ptr);

extern ROCKSDB_LIBRARY_API rocksdb_silk_pinnableslice_t* rocksdb_silk_get_pinned(
    rocksdb_silk_t* db, const rocksdb_silk_readoptions_t* options, const char* key,
    size_t keylen, char** errptr);
extern ROCKSDB_LIBRARY_API rocksdb_silk_pinnableslice_t* rocksdb_silk_get_pinned_cf(
    rocksdb_silk_t* db, const rocksdb_silk_readoptions_t* options,
    rocksdb_silk_column_family_handle_t* column_family, const char* key,
    size_t keylen, char** errptr);
extern ROCKSDB_LIBRARY_API void rocksdb_silk_pinnableslice_destroy(
    rocksdb_silk_pinnableslice_t* v);
extern ROCKSDB_LIBRARY_API const char* rocksdb_silk_pinnableslice_value(
    const rocksdb_silk_pinnableslice_t* t, size_t* vlen);

#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif  /* STORAGE_ROCKSDB_INCLUDE_C_H_ */
