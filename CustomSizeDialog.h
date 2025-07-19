//
// Copyright 2022 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#ifndef _YUVPLAYER_CUSTOM_SIZE_DIALOG_H_
#define _YUVPLAYER_CUSTOM_SIZE_DIALOG_H_

#include <wx/numdlg.h>
#include <wx/spinctrl.h>
#include <wx/wx.h>

#include "CustomSizeDialog.h"

enum {
    ID_WIDTH = 0x20,
    ID_HEIGHT
};

class CustomSizeDialog : public wxDialog {
public:
    CustomSizeDialog(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = _(""))
        : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    {
        wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
        SetSizer(topSizer);

        wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
        topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

        // Width
        wxBoxSizer *widthBox = new wxBoxSizer(wxHORIZONTAL);
        boxSizer->Add(widthBox, 0, wxGROW | wxALL, 5);

        wxStaticText *wLabel = new wxStaticText(this, wxID_STATIC, _("&Width: "), wxDefaultPosition, wxDefaultSize, 0);
        widthBox->Add(wLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

        wxSpinCtrl *widthSpin = new wxSpinCtrl(this, ID_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                               wxSP_ARROW_KEYS, 0, 7680, 800);
        widthBox->Add(widthSpin, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

        // Height
        wxBoxSizer *heightBox = new wxBoxSizer(wxHORIZONTAL);
        boxSizer->Add(heightBox, 0, wxGROW | wxRIGHT, 5);

        wxStaticText *hLabel = new wxStaticText(this, wxID_STATIC, _("&Height: "), wxDefaultPosition, wxDefaultSize, 0);
        heightBox->Add(hLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

        wxSpinCtrl *heightSpin = new wxSpinCtrl(this, ID_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                                wxSP_ARROW_KEYS, 0, 4320, 600);
        heightBox->Add(heightSpin, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

        // OK
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(new wxButton(this, wxID_OK, "&OK"), 0, wxALL, 5);

        boxSizer->Add(buttonSizer, 0, wxALIGN_CENTER);

        topSizer->Fit(this);
        topSizer->SetSizeHints(this);
    }

    bool TransferDataFromWindow() override
    {
        const auto widthSpin = (wxSpinCtrl *)FindWindow(ID_WIDTH);
        const auto heightSpin = (wxSpinCtrl *)FindWindow(ID_HEIGHT);

        width_ = widthSpin->GetValue();
        height_ = heightSpin->GetValue();
        return true;
    }

    std::pair<int, int> GetSize() { return {width_, height_}; }

private:
    int width_ = 0;
    int height_ = 0;
};

#endif // _YUVPLAYER_CUSTOM_SIZE_DIALOG_H_
