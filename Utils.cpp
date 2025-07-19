//
// Copyright 2022-2025 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#include "Utils.h"

#include <chrono>
#include <fstream>

void SaveLog(char *data, int size)
{
    static std::ofstream out("yuvplayer.log", std::ios::out | std::ios::binary | std::ios::app);
    if (!out.is_open()) {
        printf("Open yuvplayer.log failed\n");
        return;
    }
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    auto tt = std::chrono::system_clock::to_time_t(now);
    struct tm *ptm = localtime(&tt);
    char date[40] = {0};
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d.%03d - ", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1,
            (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)(ms % 1000));
    out.write(date, strlen(date));
    out.write(data, size);
    out.flush();
}

void I422ToI420(uint8_t *data, int w, int h)
{
    uint8_t *srcU = data + w * h;
    uint8_t *srcV = srcU + w * h / 2;
    uint8_t *dstU = data + w * h;
    uint8_t *dstV = dstU + w * h / 4;

    for (int row = 0; row < h / 2; row++) {
        memcpy(dstU + row * w / 2, srcU + row * w, w / 2);
    }

    for (int row = 0; row < h / 2; row++) {
        memcpy(dstV + row * w / 2, srcV + row * w, w / 2);
    }
}

void I444ToI420(uint8_t *data, int w, int h)
{
    uint8_t *srcU = data + w * h;
    uint8_t *srcV = srcU + w * h;
    uint8_t *dstU = data + w * h;
    uint8_t *dstV = dstU + w * h / 4;

    for (int j = 0; j < h / 2; j++) {
        for (int i = 0; i < w / 2; i++) {
            *dstU++ = *(srcU + 2 * j * w + 2 * i);
        }
    }

    for (int j = 0; j < h / 2; j++) {
        for (int i = 0; i < w / 2; i++) {
            *dstV++ = *(srcV + 2 * j * w + 2 * i);
        }
    }
}

// UYVY UYVY UYVY UYVY -> YYYYYYYY UUUU VVVV
void UYVYToI422(uint8_t *data, int w, int h)
{
    uint8_t *dst = new uint8_t[w * h * 2];
    uint8_t *dstY = dst;
    uint8_t *dstU = dstY + w * h;
    uint8_t *dstV = dstU + w * h / 2;

    for (int i = 0; i < w * h; ++i) {
        *dstY++ = *(data + 2 * i + 1);
    }

    for (int j = 0; j < w * h / 2; ++j) {
        *dstU++ = *(data + 4 * j);
        *dstV++ = *(data + 4 * j + 2);
    }

    memcpy(data, dst, w * h * 2);
    delete[] dst;
}

// YUYV YUYV YUYV YUYV -> YYYYYYYY UUUU VVVV
void YUY2ToI422(uint8_t *data, int w, int h)
{
    uint8_t *dst = new uint8_t[w * h * 2];
    uint8_t *dstY = dst;
    uint8_t *dstU = dstY + w * h;
    uint8_t *dstV = dstU + w * h / 2;

    for (int i = 0; i < w * h; ++i) {
        *dstY++ = *(data + 2 * i);
    }

    for (int j = 0; j < w * h / 2; ++j) {
        *dstU++ = *(data + 4 * j + 1);
        *dstV++ = *(data + 4 * j + 3);
    }

    memcpy(data, dst, w * h * 2);
    delete[] dst;
}

// YVYU YVYU YVYU YVYU -> YYYYYYYY UUUU VVVV
void YVYUToI422(uint8_t *data, int w, int h)
{
    uint8_t *dst = new uint8_t[w * h * 2];
    uint8_t *dstY = dst;
    uint8_t *dstU = dstY + w * h;
    uint8_t *dstV = dstU + w * h / 2;

    for (int i = 0; i < w * h; ++i) {
        *dstY++ = *(data + 2 * i);
    }

    for (int j = 0; j < w * h / 2; ++j) {
        *dstU++ = *(data + 4 * j + 3);
        *dstV++ = *(data + 4 * j + 1);
    }

    memcpy(data, dst, w * h * 2);
    delete[] dst;
}