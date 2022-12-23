//
// Copyright 2022 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#ifndef _YUVPLAYER_CENTER_CANVAS_H_
#define _YUVPLAYER_CENTER_CANVAS_H_

#include <wx/wx.h>
#include "MainFrame.h"
#include "SDL.h"

class CenterCanvas : public wxPanel {
public:
    CenterCanvas() = default;
    CenterCanvas(MainFrame *parent);

    static bool InitSDL();
    void OnPaint(wxPaintEvent &event);

    void SetImageSize(int width, int height);
    void SetPixelFormat(PIXEL_FORMAT format);

private:
    int width_;
    int height_;
    bool needTranscode_ = false;
    PIXEL_FORMAT pixelFormat_ = ID_FORMAT_I420;

    MainFrame *parent_ = nullptr;
    SDL_Window *window_ = nullptr;
    SDL_Surface *surface_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    SDL_Texture *texture_ = nullptr;
    SDL_Rect rectangle_;
};

#endif // _YUVPLAYER_CENTER_CANVAS_H_