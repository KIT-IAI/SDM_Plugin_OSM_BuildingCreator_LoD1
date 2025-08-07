#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Lod1Dlg.h"
#include "OsmBuildingCreator.h"
#include <LiveLogFeature.hpp>

///////////////////////////////////////////////////////////////////////////

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
};

///////////////////////////////////////////////////////////////////////////

class Lod1DlgImpl : public Lod1Dlg{
public:
    Lod1DlgImpl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("OSM Building Creator - LOD 1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 372,297 ), long style = wxDEFAULT_DIALOG_STYLE );
    ~Lod1DlgImpl();

    int m_bttnChoice;
    bool m_cancel = false;

private:
    virtual void m_sdbSizer1OnOKButtonClick( wxCommandEvent& event );
    virtual void m_OnCancelButtonClick( wxCommandEvent& event );
};