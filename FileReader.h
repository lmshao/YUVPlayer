//
// Copyright 2022 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#ifndef _YUVPLAYER_FILE_READER_H_
#define _YUVPLAYER_FILE_READER_H_

#include <fstream>
#include <string>

class FileReader {
public:
    FileReader() = default;
    ~FileReader();
    bool Open(const std::string &fileName);
    void SetFrameSize(int frameSize);
    int GetFrameNum() const;
    void Reset();
    char *GetNextFrame();
    char *GetNthFrame(int seq);

private:
    int frameSize_ = 0;
    std::fstream fd_;
    long long fileSize_ = 0;
    char *buffer_ = nullptr;
};

#endif // _YUVPLAYER_FILE_READER_H_