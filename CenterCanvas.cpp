//
// Copyright 2022-2025 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#include "CenterCanvas.h"

#include <fstream>

#include "Utils.h"

CenterCanvas::CenterCanvas(MainFrame *parent) : wxPanel(parent), parent_(parent)
{
    window_ = SDL_CreateWindowFrom(GetHandle());
    if (!window_) {
        wxLogMessage("error %s", SDL_GetError());
        return;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    if (!renderer_) {
        wxLogMessage("error %s", SDL_GetError());
        return;
    }

    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    Bind(wxEVT_PAINT, &CenterCanvas::OnPaint, this);
    Bind(wxEVT_SIZE, &CenterCanvas::OnSize, this);
}

void CenterCanvas::OnSize(wxSizeEvent &event)
{
    surface_ = nullptr;
    Refresh();
    event.Skip();
}

bool CenterCanvas::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed. SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void CenterCanvas::OnPaint(wxPaintEvent &event)
{
    LOG("OnPaint enter");

    if (!texture_) {
        LOG("SDL_CreateTexture with %d x %d, format: %d", width_, height_, (int)pixelFormat_ - (int)ID_FORMAT_I420);
        SDL_PixelFormatEnum format;
        switch (pixelFormat_) {
            case ID_FORMAT_I420:
                format = SDL_PIXELFORMAT_IYUV;
                break;
            case ID_FORMAT_YV12:
                format = SDL_PIXELFORMAT_YV12;
                break;
            case ID_FORMAT_NV12:
                format = SDL_PIXELFORMAT_NV12;
                break;
            case ID_FORMAT_NV21:
                format = SDL_PIXELFORMAT_NV21;
                break;
            case ID_FORMAT_I422:
                needTranscode_ = true;
                format = SDL_PIXELFORMAT_IYUV;
                break;
            case ID_FORMAT_UYVY:
                needTranscode_ = true;
                format = SDL_PIXELFORMAT_IYUV;
                break;
            case ID_FORMAT_YUY2:
                needTranscode_ = true;
                format = SDL_PIXELFORMAT_IYUV;
                break;
            case ID_FORMAT_YVYU:
                needTranscode_ = true;
                format = SDL_PIXELFORMAT_IYUV;
                break;
            case ID_FORMAT_I444:
                needTranscode_ = true;
                format = SDL_PIXELFORMAT_IYUV;
                break;
            default:
                format = SDL_PIXELFORMAT_IYUV;
                break;
        }

        texture_ = SDL_CreateTexture(renderer_, format, SDL_TEXTUREACCESS_STREAMING, width_, height_);
        if (!texture_) {
            printf("SDL_CreateTexture failed, SDL_Error: %s\n", SDL_GetError());
            return;
        }
    }

    static bool hasPlayed = false;
    auto frame = parent_->GetOneFrame();
    if (frame.first) {
        LOG("after texture");

        hasPlayed = true;
        if (needTranscode_) {
            if (pixelFormat_ == ID_FORMAT_I422) {
                I422ToI420((uint8_t *)frame.first, width_, height_);
            } else if (pixelFormat_ == ID_FORMAT_I444) {
                I444ToI420((uint8_t *)frame.first, width_, height_);
            } else if (pixelFormat_ == ID_FORMAT_UYVY) {
                UYVYToI422((uint8_t *)frame.first, width_, height_);
                I422ToI420((uint8_t *)frame.first, width_, height_);
            } else if (pixelFormat_ == ID_FORMAT_YUY2) {
                YUY2ToI422((uint8_t *)frame.first, width_, height_);
                I422ToI420((uint8_t *)frame.first, width_, height_);
            } else if (pixelFormat_ == ID_FORMAT_YVYU) {
                YVYUToI422((uint8_t *)frame.first, width_, height_);
                I422ToI420((uint8_t *)frame.first, width_, height_);
            }
        }
        int res = SDL_UpdateTexture(texture_, nullptr, (uint8_t *)frame.first, width_);

        if (res != 0) {
            printf("SDL_UpdateYUVTexture failed, SDL_Error: %s\n", SDL_GetError());
            return;
        }
        LOG("after SDL_UpdateYUVTexture");
        SDL_RenderClear(renderer_);

        int canvasW = 0, canvasH = 0;
        GetClientSize(&canvasW, &canvasH);
        float scale = std::min((float)canvasW / width_, (float)canvasH / height_);
        int drawW = (int)(width_ * scale);
        int drawH = (int)(height_ * scale);
        int drawX = (canvasW - drawW) / 2;
        int drawY = (canvasH - drawH) / 2;
        rectangle_ = {drawX, drawY, drawW, drawH};
        SDL_RenderCopy(renderer_, texture_, nullptr, &rectangle_);
        SDL_RenderPresent(renderer_);
    } else {
        if (hasPlayed) {
            return;
        }
        LOG("set color");
        if (!surface_) {
            surface_ = SDL_GetWindowSurface(window_);
        }

        if (!surface_) {
            LOG("Surface is null");
            return;
        }
        SDL_FillRect(surface_, nullptr, SDL_MapRGB(surface_->format, 0x93, 0xb8, 0xe5));
        SDL_UpdateWindowSurface(window_);
    }
}

void CenterCanvas::SetImageSize(int width, int height)
{
    LOG("Update Image Size enter, %dx%d => %dx%d", width_, height_, width, height);
    if (width == width_ && height_ == height) {
        return;
    }

    width_ = width;
    height_ = height;
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }

    surface_ = nullptr;
    Refresh();
}

void CenterCanvas::SetPixelFormat(PIXEL_FORMAT format)
{
    if (pixelFormat_ == format)
        return;

    pixelFormat_ = format;
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}
