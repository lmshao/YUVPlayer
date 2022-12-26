//
// Copyright 2022 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#ifndef _YUVPLAYER_UTILS_H_
#define _YUVPLAYER_UTILS_H_

#include <cstdio>
#include <stdint.h>
#include <string.h>

// clang-format off
#ifdef _WIN32
#define separator '\\'
#else
#define separator '/'
#endif

void SaveLog(char *data, int size);

#if (defined(_WIN32) && defined(_DEBUG)) || (!defined(_WIN32))
#define LOG(fmt, ...)                                                                  \
    do {                                                                               \
        char buffer[500];                                                              \
        std::string fname(__FILE__);                                                   \
        auto i = fname.rfind(separator);                                               \
        if (i != std::string::npos) {                                                  \
            fname = fname.substr(i + 1);                                               \
        }                                                                              \
        sprintf(buffer, "%s(%d): " fmt "\n", fname.c_str(), __LINE__, ##__VA_ARGS__); \
        SaveLog(buffer, strlen(buffer));                                               \
    } while (0);
#else
#define LOG(fmt, ...) {}
#endif

// clang-format on
void I422ToI420(uint8_t *data, int w, int h);
void I444ToI420(uint8_t *data, int w, int h);
void UYVYToI422(uint8_t *data, int w, int h);
void YUY2ToI422(uint8_t *data, int w, int h);
void YVYUToI422(uint8_t *data, int w, int h);

#endif // _YUVPLAYER_UTILS_H_