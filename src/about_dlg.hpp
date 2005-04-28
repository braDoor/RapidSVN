/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _ABOUT_DLG_H_INCLUDED_
#define _ABOUT_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"

/**
 * this is a "about" dialog for rapidsvn. It shows the logo and
 * some informative text about copyright and versions
 */
class AboutDlg : public wxDialog
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   * @param locale locale used by the application
   */
  AboutDlg (wxWindow * parent, const wxLocale & locale);

  /**
   * destructor
   */
  virtual ~AboutDlg ();
};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
