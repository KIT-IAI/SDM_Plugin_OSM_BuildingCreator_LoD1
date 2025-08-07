///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-f0e67a6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Lod1Dlg.h"

///////////////////////////////////////////////////////////////////////////

Lod1Dlg::Lod1Dlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxString m_radioBoxCitygmlChoices[] = { wxT("Create Boundary Surfaces and Building Solid"), wxT("Create Building Solid only") };
	int m_radioBoxCitygmlNChoices = sizeof( m_radioBoxCitygmlChoices ) / sizeof( wxString );
	m_radioBoxCitygml = new wxRadioBox( this, wxID_ANY, wxT("CityGML"), wxDefaultPosition, wxDefaultSize, m_radioBoxCitygmlNChoices, m_radioBoxCitygmlChoices, 2, wxRA_SPECIFY_ROWS );
	m_radioBoxCitygml->SetSelection( 0 );
	bSizer1->Add( m_radioBoxCitygml, 0, wxALL, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Create Boundary Surfaces and Building Solid -\nCreate extrusion from building footprints with given heights and\nattribute boundary surfaces (walls, roof, floor) for resulting solid.\n\nCreate Building Solid only -\nCreate extrusion from building footprints with given heights and\ngenerate solid model without boundary surfaces."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL, 5 );


	bSizer1->Add( fgSizer1, 1, wxEXPAND, 5 );

	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();

	bSizer1->Add( m_sdbSizer1, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_radioBoxCitygml->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( Lod1Dlg::m_radioBoxCitygmlOnRadioBox ), NULL, this );
	m_sdbSizer1Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Lod1Dlg::m_OnCancelButtonClick ), NULL, this );
	m_sdbSizer1OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Lod1Dlg::m_sdbSizer1OnOKButtonClick ), NULL, this );
}

Lod1Dlg::~Lod1Dlg()
{
	// Disconnect Events
	m_radioBoxCitygml->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( Lod1Dlg::m_radioBoxCitygmlOnRadioBox ), NULL, this );
	m_sdbSizer1Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Lod1Dlg::m_OnCancelButtonClick ), NULL, this );
	m_sdbSizer1OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Lod1Dlg::m_sdbSizer1OnOKButtonClick ), NULL, this );

}
