//
// Copyright 2022-2025 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#include "FileReader.h"

#include <cstring>

#include "Utils.h"

FileReader::~FileReader()
{
    if (fd_.is_open()) {
        LOG("fd.close()");
        fd_.close();
    }

    if (buffer_) {
        delete[] buffer_;
        buffer_ = nullptr;
    }
}

bool FileReader::Open(const std::string &fileName)
{
    fd_.open(fileName, std::ios::in | std::ios::binary);
    if (!fd_.is_open()) {
        return false;
    }

    fd_.seekg(0, std::ios::end);
    fileSize_ = (long long)fd_.tellg();
    fd_.seekg(0);
    LOG("FileReader::Open ok, size(%lld)", fileSize_);
    return true;
}

void FileReader::SetFrameSize(int frameSize)
{
    LOG("FileReader::SetFrameSize(%d)", frameSize);
    if (frameSize > 0 && frameSize_ != frameSize) {
        frameSize_ = frameSize;
        fd_.seekg(0);
        if (buffer_) {
            delete[] buffer_;
            buffer_ = nullptr;
        }
    }
}

void FileReader::Reset()
{
    fd_.seekg(0);
    if (buffer_) {
        delete[] buffer_;
        buffer_ = nullptr;
    }
}

char *FileReader::GetNextFrame()
{
    if (frameSize_ <= 0 || fd_.tellg() >= fileSize_)
        return nullptr;

    if (!buffer_) {
        buffer_ = new char[frameSize_];
    }

    memset(buffer_, 0, frameSize_);
    fd_.read(buffer_, frameSize_);
    if (fd_.eof()) {
        return nullptr;
    }

    return buffer_;
}

char *FileReader::GetNthFrame(int seq)
{
    if (seq <= 0 || seq * frameSize_ > fileSize_) {
        LOG("GetNthFrame(%d) param error, frameSize(%d) fileSize(%lld)", seq, frameSize_, fileSize_);
        return nullptr;
    }

    if (!buffer_) {
        buffer_ = new char[frameSize_];
    }

    fd_.seekg((seq - 1) * frameSize_, std::ios::beg);
    fd_.read(buffer_, frameSize_);
    if (fd_.eof()) {
        LOG("GetNthFrame(%d) eof", seq);
        return nullptr;
    }
    return buffer_;
}

int FileReader::GetFrameNum() const
{
    if (frameSize_ <= 0 || fileSize_ <= 0) {
        return -1;
    }

    return (int)(fileSize_ / frameSize_);
}
