/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _VSVN_APP_H_INCLUDED_
#define _VSVN_APP_H_INCLUDED_

#include "filelist_ctrl.h"
#include "folder_browser.h"
#include "utils.h"
#include "rapidsvn_frame.h"

class RapidSvnFrame;

class RapidSvnApp:public wxApp
{
  virtual bool OnInit ();
  virtual int OnExit ();

public:
   RapidSvnFrame * appFrame;

  RapidSvnFrame *GetAppFrame ()
  {
    return appFrame;
  }
};

DECLARE_APP (RapidSvnApp)
#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */