///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-f0e67a6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/radiobox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class Lod1Dlg
///////////////////////////////////////////////////////////////////////////////
class Lod1Dlg : public wxDialog
{
	private:

	protected:
		wxRadioBox* m_radioBoxCitygml;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText1;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;

		// Virtual event handlers, override them in your derived class
		virtual void m_radioBoxCitygmlOnRadioBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_OnCancelButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_sdbSizer1OnOKButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		Lod1Dlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("OSM Building Creator - LOD 1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 372,297 ), long style = wxDEFAULT_DIALOG_STYLE );

		~Lod1Dlg();

};

