//
// Copyright 2022 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#ifndef _YUVPLAYER_MAIN_FRAME_H_
#define _YUVPLAYER_MAIN_FRAME_H_

#include <wx/wx.h>
#include "FileReader.h"

enum LocalItemId {
    ID_CustomSize = wxID_HIGHEST + 1,
    ID_SIZE_1080P,
    ID_SIZE_720P,
    ID_SIZE_480P,
    ID_SIZE_360P,
    ID_FRAMERATE_1,
    ID_FRAMERATE_10,
    ID_FRAMERATE_24,
    ID_FRAMERATE_25,
    ID_FRAMERATE_30,
    ID_FRAMERATE_60,
    ID_BUTTON_OPEN,
    ID_BUTTON_PLAY,
    ID_BUTTON_STOP,
    ID_BUTTON_PROCESSBAR,
    ID_LOCAL_ITEM_END,
};

enum PIXEL_FORMAT {
    ID_FORMAT_I420 = ID_LOCAL_ITEM_END + 1,
    ID_FORMAT_YV12, // YYYYYYYY VV UU
    ID_FORMAT_NV12, // YYYYYYYY UVUV
    ID_FORMAT_NV21, // YYYYYYYY VUVU
    ID_FORMAT_I422, // YYYYYYYY UUUU VVVV
    ID_FORMAT_UYVY, // UYVY UYVY UYVY UYVY
    ID_FORMAT_YUY2, // YUYV YUYV YUYV YUYV
    ID_FORMAT_YVYU, // YVYU YVYU YVYU YVYU
    ID_FORMAT_I444, // YYYYYYYY UUUUUUUU VVVVVVVV
};

class CenterCanvas;

class MainFrame : public wxFrame {
public:
    MainFrame();
    ~MainFrame() = default;
    std::pair<char *, int> GetOneFrame();

private:
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnOpenFile(wxCommandEvent &event);
    void OnCustomSize(wxCommandEvent &event);
    void OnSizeRadioSelected(wxCommandEvent &event);
    void OnPixelFormatRadioSelected(wxCommandEvent &event);
    void OnFramerateRadioSelected(wxCommandEvent &event);
    void OnProcessBarDrag(wxCommandEvent &event);
    void OnPlayButton(wxCommandEvent &event);
    void OnStopButton(wxCommandEvent &event);

    void OnChar(wxKeyEvent &event); // keyboard
    void OnSize(wxSizeEvent &event);
    void OnTimer(wxTimerEvent &event);

    wxMenu *CreateMenuSize();
    wxMenu *CreateMenuPixelFormat();
    wxMenu *CreateMenuFramerate();
    wxStatusBar *CreateStatusBar();

    int GetFrameSize();
    void DoPreviewMode();
    void ResetPlayStatus();

    bool isRunning() const
    {
        return running_;
    }

private:
    int width_;
    int height_;
    PIXEL_FORMAT pixelFormat_;
    int framerate_;
    bool running_ = false;
    bool pause_ = false;
    wxStatusBar *statusBar_ = nullptr;
    std::string mediaFile_;
    wxTimer *timer_ = nullptr;
    CenterCanvas *canvas_ = nullptr;
    FileReader *reader_ = nullptr;
    int frameSeq_ = 0;
    int frameNum_ = 0;
};

#endif // _YUVPLAYER_MAIN_FRAME_H_