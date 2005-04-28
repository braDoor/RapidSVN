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

// wxWidgets
#include "wx/wx.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/status.hpp"

// stl
#include <vector>

// app
#include "delete_action.hpp"
#include "delete_dlg.hpp"
#include "utils.hpp"

DeleteAction::DeleteAction (wxWindow * parent)
  : Action (parent, _("Delete"), GetBaseFlags ())
{
}

bool
DeleteAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  DeleteDlg dlg (GetParent ());

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_force = dlg.GetForce ();

  return true;
}

bool
DeleteAction::Perform ()
{
  svn::Client client (GetContext ());

  const std::vector<svn::Path> & v = GetTargets ();
  std::vector<svn::Path>::const_iterator it;

  for (it=v.begin (); it!=v.end (); it++)
  {
    svn::Path path (*it);
    const char * cpath = path.c_str ();
    svn::Status status (client.singleStatus (cpath));
    wxString wxpath = Utf8ToLocal(cpath);

    // if the file is versioned then it will be
    // handled by subversion
    if (status.isVersioned ())
      client.remove (path, m_force);
    else if (wxDirExists (wxpath))
    {
      // we dont want to delete unversioned directories...
      wxString msg;
      msg.Printf (_("Skipping unversioned directory: %s"),
                  wxpath.c_str ());
      Trace (msg);
    }
    else if (wxFileExists (wxpath))
    {
      wxString msg;
      msg.Printf (_("Deleting unversioned file: %s"),
                  wxpath.c_str ());
      Trace (msg);

      wxRemoveFile (wxpath);
    }
  }


  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
