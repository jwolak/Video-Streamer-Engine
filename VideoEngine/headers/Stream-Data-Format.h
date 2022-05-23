#ifndef STREAMDATAFORMAT_H
#define STREAMDATAFORMAT_H

#include <linux/videodev2.h>

namespace video_streamer {

class StreamDataFormat
{
public:
    StreamDataFormat() {}
    v4l2_format &GetSrcFormat();
    v4l2_format &GetDstFormat();
    void SetPixelformat(__u32);

private:
    v4l2_format src_frmt_;
    v4l2_format dst_frmt_;
};

} /* namespace video_streamer */

#endif // STREAMDATAFORMAT_H
