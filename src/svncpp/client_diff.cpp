/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// Subversion api
#include "svn_client.h"

// svncpp
#include "client.hpp"
#include "exception.hpp"
#include "pool.hpp"
#include "status.hpp"

namespace svn
{
  /**
   * a quick way to create error messages
   */
  static void
  fail (apr_pool_t *pool, apr_status_t status, const char *fmt, ...)
  {
    va_list ap;
    char *msg;
    svn_error_t * error;

    va_start (ap, fmt);
    msg = apr_pvsprintf (pool, fmt, ap);
    va_end (ap);

    error = svn_error_create (status, NULL, msg);
    throw ClientException (error);
  }

  /**
   * closes and deletes temporary files that diff has been using
   */
  static void
  diffCleanup (apr_file_t * outfile, const char * outfileName,
               apr_file_t * errfile, const char * errfileName,
               apr_pool_t *pool)
  {
    if (outfile != NULL)
      apr_file_close (outfile);

    if (errfile != NULL)
      apr_file_close (errfile);

    if (outfileName != NULL)
      svn_error_clear (svn_io_remove_file (outfileName, pool));

    if (errfileName != NULL)
      svn_error_clear (svn_io_remove_file (errfileName, pool));
  }

  std::string
  Client::diff (const Path & tmpPath, const Path & path,
                const Revision & revision1, const Revision & revision2,
                const bool recurse, const bool ignoreAncestry,
                const bool noDiffDeleted)
  {
    Pool pool;
    svn_error_t * error;
    apr_status_t status;
    apr_file_t * outfile = NULL;
    const char * outfileName = NULL;
    apr_file_t * errfile = NULL;
    const char * errfileName = NULL;
    apr_array_header_t * options;
    svn_stringbuf_t * stringbuf;

    // svn_client_diff needs an options array, even if it is empty
    options = apr_array_make (pool, 0, 0);

    // svn_client_diff needs a temporary file to write diff output to
    error = svn_io_open_unique_file (&outfile, &outfileName,
                                     tmpPath.c_str(), ".tmp",
                                     FALSE, pool);

    if (error != NULL)
    {
      diffCleanup (outfile, outfileName, errfile, errfileName, pool);
      throw ClientException (error);
    }

    // and another one to write errors to
    error = svn_io_open_unique_file (&errfile, &errfileName,
                                     tmpPath.c_str(), ".tmp",
                                     FALSE, pool);

    if (error != NULL)
    {
      diffCleanup (outfile, outfileName, errfile, errfileName, pool);
      throw ClientException (error);
    }

    // run diff
    error = svn_client_diff (options,
                             path.c_str (), revision1.revision (),
                             path.c_str (), revision2.revision (),
                             recurse, ignoreAncestry, noDiffDeleted,
                             outfile, errfile,
                             *m_context,
                             pool);

    if (error != NULL)
    {
      diffCleanup (outfile, outfileName, errfile, errfileName, pool);
      throw ClientException (error);
    }

    // then we reopen outfile for reading
    status = apr_file_close (outfile);
    if (status)
    {
      diffCleanup (outfile, outfileName, errfile, errfileName, pool);
      fail (pool, status, "failed to close '%s'", outfileName);
    }

    status = apr_file_open (&outfile, outfileName, APR_READ, APR_OS_DEFAULT, pool);
    if (status)
    {
      diffCleanup (outfile, outfileName, errfile, errfileName, pool);
      fail (pool, status, "failed to open '%s'", outfileName);
    }

    // now we can read the diff output from outfile and return that
    error = svn_stringbuf_from_aprfile (&stringbuf, outfile, pool);

    if (error != NULL)
    {
      diffCleanup (outfile, outfileName, errfile, errfileName, pool);
      throw ClientException (error);
    }

    diffCleanup (outfile, outfileName, errfile, errfileName, pool);
    return stringbuf->data;
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
