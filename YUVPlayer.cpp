//
// Copyright 2022 SHAO Liming <lmshao@163.com>. All rights reserved.
//

#include "YUVPlayer.h"
#include "MainFrame.h"
#include "Utils.h"

bool YUVPlayer::OnInit()
{
    LOG("------------- YUVPlayer start --------------------");
    MainFrame *frame = new MainFrame();
    frame->Show(true);
    return true;
}