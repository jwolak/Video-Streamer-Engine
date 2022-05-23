#ifndef TEMPORARYVIDEOCAPTUREBUFFER_H
#define TEMPORARYVIDEOCAPTUREBUFFER_H

#include <linux/videodev2.h> /* V4L2_BUF_TYPE_VIDEO_CAPTURE */

#include <cstring> /* memset() */

namespace video_streamer {

class TemporaryVideoCaptureBuffer
{
public:
    TemporaryVideoCaptureBuffer() {

        std::memset(&tmp_frmt_buffer_, 0, sizeof(tmp_frmt_buffer_));
        tmp_frmt_buffer_.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    }

    v4l2_format &GetBuffer();

private:
    v4l2_format tmp_frmt_buffer_;
};

} /* namespace video_streamer */

#endif // TEMPORARYVIDEOCAPTUREBUFFER_H
