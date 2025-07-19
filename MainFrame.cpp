//
// Copyright 2022-2025 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#include "MainFrame.h"

#include <algorithm>

#include "CenterCanvas.h"
#include "CustomSizeDialog.h"
#include "Utils.h"
#include "resource.h"
#include "wx/aboutdlg.h"
#include "wx/gauge.h"
#include "wx/tglbtn.h"

enum StatusBarIndex {
    STATUSBAR_OPEN = 0,
    STATUSBAR_PLAY,
    STATUSBAR_STOP,
    STATUSBAR_PROCESS,
    STATUSBAR_RATE,
    STATUSBAR_INFO,
};

#define YUVPLAYER_STYLE                                                                                                \
    (wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxRESIZE_BORDER | wxCLIP_CHILDREN)

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Sugar YUVPlayer", wxPoint(320, 180), wxDefaultSize, YUVPLAYER_STYLE),
      pixelFormat_(ID_FORMAT_I420), framerate_(24)
{
    CenterCanvas::InitSDL();

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuSize = CreateMenuSize();
    wxMenu *menuFormat = CreateMenuPixelFormat();
    wxMenu *menuFrameRate = CreateMenuFramerate();
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuSize, "&Size");
    menuBar->Append(menuFormat, "&Format");
    menuBar->Append(menuFrameRate, "&FrameRate");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    SetClientSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    canvas_ = new CenterCanvas(this);
    canvas_->SetImageSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(canvas_, 1, wxEXPAND | wxALL, 0);
    SetSizer(sizer);
    statusBar_ = CreateStatusBar();

    SetMinClientSize(wxSize(174, 144)); // QCIF

#ifdef __WXMSW__
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
    if (hIcon) {
        wxIcon icon;
        icon.CreateFromHICON(hIcon);
        SetIcon(icon);
    }
#else
    wxIcon icon;
    if (icon.LoadFile("resources/app.ico", wxBITMAP_TYPE_ICO)) {
        SetIcon(icon);
    }
#endif

    timer_ = new wxTimer(this);

    Bind(wxEVT_MENU, &MainFrame::OnOpenFile, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnCustomSize, this, ID_CustomSize);
    Bind(wxEVT_MENU, &MainFrame::OnSizeRadioSelected, this, ID_SIZE_2160P);
    Bind(wxEVT_MENU, &MainFrame::OnSizeRadioSelected, this, ID_SIZE_1440P);
    Bind(wxEVT_MENU, &MainFrame::OnSizeRadioSelected, this, ID_SIZE_1080P);
    Bind(wxEVT_MENU, &MainFrame::OnSizeRadioSelected, this, ID_SIZE_720P);
    Bind(wxEVT_MENU, &MainFrame::OnSizeRadioSelected, this, ID_SIZE_480P);
    Bind(wxEVT_MENU, &MainFrame::OnSizeRadioSelected, this, ID_SIZE_360P);
    Bind(wxEVT_MENU, &MainFrame::OnPixelFormatRadioSelected, this, ID_FORMAT_I420);
    Bind(wxEVT_MENU, &MainFrame::OnPixelFormatRadioSelected, this, ID_FORMAT_YV12);
    Bind(wxEVT_MENU, &MainFrame::OnPixelFormatRadioSelected, this, ID_FORMAT_NV12);
    Bind(wxEVT_MENU, &MainFrame::OnPixelFormatRadioSelected, this, ID_FORMAT_NV21);
    Bind(wxEVT_MENU, &MainFrame::OnPixelFormatRadioSelected, this, ID_FORMAT_I422);
    Bind(wxEVT_MENU, &MainFrame::OnPixelFormatRadioSelected, this, ID_FORMAT_UYVY);
    Bind(wxEVT_MENU, &MainFrame::OnPixelFormatRadioSelected, this, ID_FORMAT_YUY2);
    Bind(wxEVT_MENU, &MainFrame::OnPixelFormatRadioSelected, this, ID_FORMAT_YVYU);
    Bind(wxEVT_MENU, &MainFrame::OnPixelFormatRadioSelected, this, ID_FORMAT_I444);
    Bind(wxEVT_MENU, &MainFrame::OnFramerateRadioSelected, this, ID_FRAMERATE_1);
    Bind(wxEVT_MENU, &MainFrame::OnFramerateRadioSelected, this, ID_FRAMERATE_10);
    Bind(wxEVT_MENU, &MainFrame::OnFramerateRadioSelected, this, ID_FRAMERATE_24);
    Bind(wxEVT_MENU, &MainFrame::OnFramerateRadioSelected, this, ID_FRAMERATE_25);
    Bind(wxEVT_MENU, &MainFrame::OnFramerateRadioSelected, this, ID_FRAMERATE_30);
    Bind(wxEVT_MENU, &MainFrame::OnFramerateRadioSelected, this, ID_FRAMERATE_60);
    Bind(wxEVT_CHAR, &MainFrame::OnChar, this);
    Bind(wxEVT_SIZE, &MainFrame::OnSize, this);
    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);

    LOG("ctor");
}

void MainFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
    wxAboutDialogInfo info;
    info.SetCopyright("Copyright (c) 2022-2025 SHAO Liming");
    info.SetName("Sugar YUVPlayer");
    info.SetDescription(_("This is a raw video player."));
    info.SetWebSite("https://github.com/lmshao/YUVPlayer");

// Set About dialog icon from embedded resource
#ifdef __WXMSW__
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
    if (hIcon) {
        wxIcon aboutIcon;
        aboutIcon.CreateFromHICON(hIcon);
        info.SetIcon(aboutIcon);
    }
#else
    // Fallback for other platforms
    wxIcon aboutIcon;
    if (aboutIcon.LoadFile("resources/app.ico", wxBITMAP_TYPE_ICO)) {
        info.SetIcon(aboutIcon);
    }
#endif

    //  info.AddDeveloper(_("SHAO Liming"));
    wxAboutBox(info, this);
}

void MainFrame::OnOpenFile(wxCommandEvent &event)
{
    wxFileDialog fd(this, "Select a yuv file", wxEmptyString, wxEmptyString, _("YUV files (*.yuv)|*.yuv"));
    if (fd.ShowModal() == wxID_OK) {
        mediaFile_ = fd.GetPath();
    }

    if (mediaFile_.empty())
        return;

    if (reader_) {
        delete reader_;
    }

    reader_ = new FileReader();
    if (!reader_->Open(mediaFile_)) {
        wxMessageBox("Failed to open file: %s", mediaFile_.c_str());
        return;
    }

    DoPreviewMode();
    LOG("OnOpenFile ok, then Refresh()");
}

void MainFrame::OnCustomSize(wxCommandEvent &event)
{
    auto frame = new CustomSizeDialog(this, 0, "Custom Size");
    if (frame->ShowModal() == wxID_OK) {
        std::pair<int, int> res = frame->GetSize();
        width_ = res.first;
        height_ = res.second;
        LOG("select OnCustomSize, %d x %d", width_, height_);
        // SetClientSize(width_, height_);

        if (statusBar_) {
            wxString paramStr = wxString::Format("%dx%d @ %d fps", width_, height_, framerate_);
            SetStatusBarInfoCenterText(statusBar_, STATUSBAR_INFO, paramStr);
        }
    }
    DoPreviewMode();
}

void MainFrame::OnSizeRadioSelected(wxCommandEvent &event)
{
    switch (event.GetId()) {
        case ID_SIZE_2160P:
            width_ = 3840;
            height_ = 2160;
            LOG("select 2160p, %d x %d", width_, height_);
            break;
        case ID_SIZE_1440P:
            width_ = 2560;
            height_ = 1440;
            LOG("select 1440p, %d x %d", width_, height_);
            break;
        case ID_SIZE_1080P:
            width_ = 1920;
            height_ = 1080;
            LOG("select 1080p, %d x %d", width_, height_);
            break;
        case ID_SIZE_720P:
            width_ = 1280;
            height_ = 720;
            LOG("select 720p, %d x %d", width_, height_);
            break;
        case ID_SIZE_480P:
            width_ = 640;
            height_ = 480;
            LOG("select 480p, %d x %d", width_, height_);
            break;
        case ID_SIZE_360P:
            width_ = 640;
            height_ = 360;
            LOG("select 360p, %d x %d", width_, height_);
            break;
        default:
            width_ = 640;
            height_ = 360;
            LOG("select 360p, %d x %d", width_, height_);
            break;
    }

    if (statusBar_) {
        wxString paramStr = wxString::Format("%dx%d @ %d fps", width_, height_, framerate_);
        SetStatusBarInfoCenterText(statusBar_, STATUSBAR_INFO, paramStr);
    }

    DoPreviewMode();
}

void MainFrame::OnPixelFormatRadioSelected(wxCommandEvent &event)
{
    pixelFormat_ = (PIXEL_FORMAT)event.GetId();
    if (running_) {
        // TODO: ResetAllStatus
    }
    DoPreviewMode();
}

void MainFrame::OnFramerateRadioSelected(wxCommandEvent &event)
{
    switch (event.GetId()) {
        case ID_FRAMERATE_1:
            framerate_ = 1;
            break;
        case ID_FRAMERATE_10:
            framerate_ = 10;
            break;
        case ID_FRAMERATE_24:
            framerate_ = 24;
            break;
        case ID_FRAMERATE_25:
            framerate_ = 25;
            break;
        case ID_FRAMERATE_30:
            framerate_ = 30;
            break;
        case ID_FRAMERATE_60:
            framerate_ = 60;
            break;
        default:
            framerate_ = 24;
            break;
    }

    if (timer_->IsRunning()) {
        timer_->Stop();
        timer_->Start(1000 / framerate_);
    }

    if (statusBar_) {
        wxString paramStr = wxString::Format("%dx%d @ %d fps", width_, height_, framerate_);
        SetStatusBarInfoCenterText(statusBar_, STATUSBAR_INFO, paramStr);
    }
}

void MainFrame::OnProcessBarDrag(wxCommandEvent &event)
{
    wxSlider *progressBar = (wxSlider *)event.GetEventObject();

    int value = progressBar->GetValue();
    LOG("OnProcessBarDrag value: %d", value);
    statusBar_->SetStatusText(wxString::Format(wxT("%d/100"), value), STATUSBAR_RATE);
}

void MainFrame::OnPlayButton(wxCommandEvent &event)
{
    if (mediaFile_.empty()) {
        wxMessageBox("pls select a yuv file");
        return;
    }

    if (!reader_) {
        reader_ = new FileReader();
        if (!reader_->Open(mediaFile_)) {
            wxMessageBox("Failed to open file: %s", mediaFile_.c_str());
            return;
        }
    }

    wxToggleButton *playBt = (wxToggleButton *)event.GetEventObject();
    if (playBt->GetValue()) {
        running_ = true;
        playBt->SetLabelText("Pause");

        wxSlider *progressBar = (wxSlider *)wxWindow::FindWindowById(ID_BUTTON_PROCESSBAR);
        progressBar->Enable(true);
        // progressBar->SetBackgroundColour(wxColor(0x7c, 0xca, 0xce));

        // TODO: Play the file
        reader_->SetFrameSize(GetFrameSize());

        frameNum_ = reader_->GetFrameNum();
        if (frameNum_ <= 0) {
            wxMessageBox("Invalid file");
            return;
        }

        statusBar_->SetStatusText(wxString::Format("%d/%d", frameSeq_, frameNum_), STATUSBAR_RATE);
        timer_->Start(1000 / framerate_);
    } else {
        playBt->SetLabelText("Play");
        pause_ = true;
        timer_->Stop();
    }
}

void MainFrame::OnStopButton(wxCommandEvent &event)
{
    statusBar_->SetStatusText(_("0/0"), STATUSBAR_RATE);
    if (!running_) {
        return;
    }
    running_ = false;
    pause_ = false;
    timer_->Stop();

    ResetPlayStatus();

    if (reader_) {
        delete reader_;
        reader_ = nullptr;
        frameSeq_ = 0;
        frameNum_ = 0;
    }
}

wxMenu *MainFrame::CreateMenuSize()
{
    wxMenu *menuSize = new wxMenu;
    wxMenuItem *item2160 = menuSize->AppendRadioItem(ID_SIZE_2160P, _("3840x2160\t(4K UHD)"));
    item2160->Enable(true);
    item2160->Check(false);
    wxMenuItem *item1440 = menuSize->AppendRadioItem(ID_SIZE_1440P, _("2560x1440\t(2K QHD)"));
    item1440->Enable(true);
    item1440->Check(false);
    wxMenuItem *item1080 = menuSize->AppendRadioItem(ID_SIZE_1080P, _("1920x1080\t(FHD)"));
    item1080->Enable(true);
    item1080->Check(false);
    wxMenuItem *item720 = menuSize->AppendRadioItem(ID_SIZE_720P, _("1280x720\t(HD)"));
    item720->Enable(true);
    item720->Check(true);
    width_ = 1280;
    height_ = 720;
    wxMenuItem *item480 = menuSize->AppendRadioItem(ID_SIZE_480P, _("640x480\t(VGA)"));
    item480->Enable(true);
    item480->Check(false);
    wxMenuItem *item360 = menuSize->AppendRadioItem(ID_SIZE_360P, _("640x360\t(nHD)"));
    item360->Enable(true);
    item360->Check(false);
    menuSize->AppendSeparator();
    menuSize->Append(ID_CustomSize, "Custom", "Custom Size");
    return menuSize;
}

wxMenu *MainFrame::CreateMenuPixelFormat()
{
    wxMenu *menuFormat = new wxMenu;
    auto itemI420 = menuFormat->AppendRadioItem(ID_FORMAT_I420, _("I420"));
    itemI420->Enable(true);
    itemI420->Check(true);
    pixelFormat_ = ID_FORMAT_I420; // default
    auto itemYV12 = menuFormat->AppendRadioItem(ID_FORMAT_YV12, _("YV12"));
    itemYV12->Enable(true);
    itemYV12->Check(false);
    auto itemNV12 = menuFormat->AppendRadioItem(ID_FORMAT_NV12, _("NV12"));
    itemNV12->Enable(true);
    itemNV12->Check(false);
    auto itemNV21 = menuFormat->AppendRadioItem(ID_FORMAT_NV21, _("NV21"));
    itemNV21->Enable(true);
    itemNV21->Check(false);
    auto itemI422 = menuFormat->AppendRadioItem(ID_FORMAT_I422, _("I422"));
    itemI422->Enable(true);
    itemI422->Check(false);
    auto itemUYVY = menuFormat->AppendRadioItem(ID_FORMAT_UYVY, _("UYVY"));
    itemUYVY->Enable(true);
    itemUYVY->Check(false);
    auto itemYUY2 = menuFormat->AppendRadioItem(ID_FORMAT_YUY2, _("YUY2"));
    itemYUY2->Enable(true);
    itemYUY2->Check(false);
    auto itemYVYU = menuFormat->AppendRadioItem(ID_FORMAT_YVYU, _("YVYU"));
    itemYVYU->Enable(true);
    itemYVYU->Check(false);
    auto itemI444 = menuFormat->AppendRadioItem(ID_FORMAT_I444, _("I444"));
    itemI444->Enable(true);
    itemI444->Check(false);
    return menuFormat;
}

wxMenu *MainFrame::CreateMenuFramerate()
{
    wxMenu *menuFR = new wxMenu;
    auto fr1 = menuFR->AppendRadioItem(ID_FRAMERATE_1, _(" 1 fps\t(Slow)"));
    fr1->Enable(true);
    fr1->Check(false);
    auto fr10 = menuFR->AppendRadioItem(ID_FRAMERATE_10, _("10 fps\t(Animation)"));
    fr10->Enable(true);
    fr10->Check(false);
    auto fr24 = menuFR->AppendRadioItem(ID_FRAMERATE_24, _("24 fps\t(Cinematic)"));
    fr24->Enable(true);
    fr24->Check(true);
    auto fr25 = menuFR->AppendRadioItem(ID_FRAMERATE_25, _("25 fps\t(PAL)"));
    fr25->Enable(true);
    fr25->Check(false);
    auto fr30 = menuFR->AppendRadioItem(ID_FRAMERATE_30, _("30 fps\t(NTSC)"));
    fr30->Enable(true);
    fr30->Check(true);
    framerate_ = 30;
    auto fr60 = menuFR->AppendRadioItem(ID_FRAMERATE_60, _("60 fps\t(Smooth)"));
    fr60->Enable(true);
    fr60->Check(false);
    return menuFR;
}

wxStatusBar *MainFrame::CreateStatusBar()
{
    wxStatusBar *statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);
    SetStatusBar(statusBar);

    int widths[] = {30, 30, 30, -1, 60, 120};
    statusBar->SetFieldsCount(WXSIZEOF(widths), widths);

    wxRect rcOpen;
    statusBar->GetFieldRect(STATUSBAR_OPEN, rcOpen);
    wxButton *openBt = new wxButton(statusBar, ID_BUTTON_OPEN, _("Open"), wxPoint(rcOpen.x, rcOpen.y),
                                    wxSize(rcOpen.width, rcOpen.height));
    openBt->Bind(wxEVT_BUTTON, &MainFrame::OnOpenFile, this);

    wxRect rcPlay;
    statusBar->GetFieldRect(STATUSBAR_PLAY, rcPlay);
    wxToggleButton *playBt = new wxToggleButton(statusBar, ID_BUTTON_PLAY, _("Play"), wxPoint(rcPlay.x, rcPlay.y),
                                                wxSize(rcPlay.width, rcPlay.height));
    playBt->Bind(wxEVT_TOGGLEBUTTON, &MainFrame::OnPlayButton, this);

    wxRect rcStop;
    statusBar->GetFieldRect(STATUSBAR_STOP, rcStop);
    wxButton *stopBt = new wxButton(statusBar, ID_BUTTON_STOP, _("Stop"), wxPoint(rcStop.x, rcStop.y),
                                    wxSize(rcStop.width, rcStop.height));
    stopBt->Bind(wxEVT_BUTTON, &MainFrame::OnStopButton, this);

    wxRect rcProcess;
    statusBar->GetFieldRect(STATUSBAR_PROCESS, rcProcess);
    wxSlider *progressBar = new wxSlider(statusBar, ID_BUTTON_PROCESSBAR, 0, 0, 100, wxPoint(rcProcess.x, rcProcess.y),
                                         wxSize(rcProcess.width, rcProcess.height), wxGA_HORIZONTAL);
    progressBar->SetTickFreq(1);
    progressBar->Bind(wxEVT_SLIDER, &MainFrame::OnProcessBarDrag, this);
    progressBar->Enable(false);

    statusBar->SetStatusText(_("0/0"), STATUSBAR_RATE);
    wxString paramStr = wxString::Format("%dx%d @ %d fps", width_, height_, framerate_);
    SetStatusBarInfoCenterText(statusBar, STATUSBAR_INFO, paramStr);
    return statusBar;
}

void MainFrame::SetStatusBarInfoCenterText(wxStatusBar *bar, int field, const wxString &text)
{
    wxRect rc;
    bar->GetFieldRect(field, rc);
    int fieldWidth = rc.GetWidth();
    int textWidth = bar->GetTextExtent(text).GetWidth();
    int spaceWidth = bar->GetTextExtent(" ").GetWidth();
    int padSpaces = (fieldWidth - textWidth) / (2 * std::max(spaceWidth, 1));
    wxString padStr(' ', padSpaces > 0 ? padSpaces : 0);
    bar->SetStatusText(padStr + text + padStr, field);
}

void MainFrame::OnChar(wxKeyEvent &event)
{
    switch (event.GetKeyCode()) {
        case WXK_SPACE: {
            wxMessageBox("key space");
            break;
        }
        default:
            break;
    }
}

void MainFrame::OnSize(wxSizeEvent &event)
{
    wxRect rcProcess;
    statusBar_->GetFieldRect(STATUSBAR_PROCESS, rcProcess);

    wxSlider *progressBar = (wxSlider *)wxWindow::FindWindowById(ID_BUTTON_PROCESSBAR);
    progressBar->SetPosition(wxPoint(rcProcess.x, rcProcess.y));
    progressBar->SetClientSize(wxSize(rcProcess.width, rcProcess.height));
    auto size = event.GetSize();
    LOG("OnSize: %d * %d", size.GetWidth(), size.GetHeight());
    auto csize = GetClientSize();
    LOG("OnSize: client %d * %d", csize.GetWidth(), csize.GetHeight());

    canvas_->SetSize(csize);
    canvas_->SetImageSize(width_, height_);
    canvas_->Refresh();
    canvas_->CenterOnParent();
}

void MainFrame::OnTimer(wxTimerEvent &event)
{
    if (canvas_) {
        canvas_->Refresh();
    }
}

int MainFrame::GetFrameSize()
{
    int pixelDepth;
    switch (pixelFormat_) {
        case ID_FORMAT_I420:
        case ID_FORMAT_YV12:
        case ID_FORMAT_NV12:
        case ID_FORMAT_NV21:
            pixelDepth = 12;
            break;
        case ID_FORMAT_I422:
        case ID_FORMAT_UYVY:
        case ID_FORMAT_YUY2:
        case ID_FORMAT_YVYU:
            pixelDepth = 16;
            break;
        case ID_FORMAT_I444:
            pixelDepth = 24;
            break;
        default:
            pixelDepth = 12;
            break;
    }

    return width_ * height_ * pixelDepth / 8;
}

std::pair<char *, int> MainFrame::GetOneFrame()
{
    char *data = nullptr;
    LOG("GetOneFrame");

    if (frameSeq_ != 0) {
        if (running_ && reader_) {
            data = reader_->GetNextFrame();
            if (data) {
                LOG("Get a frame OK");
                frameSeq_++;
                statusBar_->SetStatusText(wxString::Format("%d/%d", frameSeq_, frameNum_), STATUSBAR_RATE);
                wxSlider *progressBar = (wxSlider *)wxWindow::FindWindowById(ID_BUTTON_PROCESSBAR);
                progressBar->SetValue(frameSeq_ * 100 / frameNum_);
            }
        }
    } else {
        if (reader_) {
            LOG("---");
            data = reader_->GetNthFrame(1);
            if (data) {
                LOG("Get first frame OK");
                frameSeq_++;
                statusBar_->SetStatusText(wxString::Format("%d/%d", frameSeq_, frameNum_), STATUSBAR_RATE);
                wxSlider *progressBar = (wxSlider *)wxWindow::FindWindowById(ID_BUTTON_PROCESSBAR);
                progressBar->SetValue(frameSeq_ * 100 / frameNum_);
            }
        }
    }
    return {data, GetFrameSize()};
}

void MainFrame::DoPreviewMode()
{
    if (mediaFile_.empty()) {
        return;
    }

    if (!reader_) {
        reader_ = new FileReader();
        if (!reader_->Open(mediaFile_)) {
            wxMessageBox("Failed to open file: %s", mediaFile_.c_str());
            return;
        }
    }

    LOG("Set Frame Size: %d", GetFrameSize());
    reader_->SetFrameSize(GetFrameSize());

    frameNum_ = reader_->GetFrameNum();
    frameSeq_ = 0;

    if (statusBar_) {
        wxString paramStr = wxString::Format("%dx%d @ %d fps", width_, height_, framerate_);
        SetStatusBarInfoCenterText(statusBar_, STATUSBAR_INFO, paramStr);
    }

    canvas_->SetImageSize(width_, height_);
    canvas_->SetPixelFormat(pixelFormat_);
    canvas_->Refresh();
}

void MainFrame::ResetPlayStatus()
{
    wxToggleButton *playBt = (wxToggleButton *)wxWindow::FindWindowById(ID_BUTTON_PLAY);
    playBt->SetValue(false);
    playBt->SetLabelText("Play");

    wxSlider *progressBar = (wxSlider *)wxWindow::FindWindowById(ID_BUTTON_PROCESSBAR);
    progressBar->SetValue(0);
    progressBar->Enable(false);

    reader_->SetFrameSize(GetFrameSize());
    frameNum_ = reader_->GetFrameNum();
    if (frameNum_ <= 0) {
        wxMessageBox("Invalid file");
        return;
    }

    statusBar_->SetStatusText(wxString::Format("%d/%d", frameSeq_, frameNum_), STATUSBAR_RATE);
    running_ = false;
    pause_ = false;
}