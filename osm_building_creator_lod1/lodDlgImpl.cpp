
#include "lodDlgImpl.h"

IMPLEMENT_APP_NO_MAIN(MyApp)

bool MyApp::OnInit()
{
    return true;
}

int MyApp::OnExit()
{
    return 0;
}

Lod1DlgImpl::Lod1DlgImpl(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
        : Lod1Dlg(parent, id, title, pos, size, style)
{
    Centre();

    SetDoubleBuffered(true);


    wxSize minSize = this->GetBestSize();
    this->SetSize(minSize);
}

Lod1DlgImpl::~Lod1DlgImpl()
{
}

void Lod1DlgImpl::m_sdbSizer1OnOKButtonClick(wxCommandEvent &event)
{
    m_bttnChoice = m_radioBoxCitygml->GetSelection();
    EndDialog(0);
}

void Lod1DlgImpl::m_OnCancelButtonClick(wxCommandEvent &event)
{
    m_cancel = true;
    Lod1Dlg::m_OnCancelButtonClick(event);
}
