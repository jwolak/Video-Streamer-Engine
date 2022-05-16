#ifndef COPYBUFFER_H
#define COPYBUFFER_H

#include <linux/videodev2.h>

#include <cstring>

namespace video_streamer {

class CopyBuffer
{
public:
    CopyBuffer() {
        std::memset(&cpy_buffer_, 0, sizeof(cpy_buffer_));
        cpy_buffer_.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        cpy_buffer_.memory = V4L2_MEMORY_MMAP;
    }

    v4l2_buffer & GetBuffer();

private:
    struct v4l2_buffer cpy_buffer_;
};

} /* namespace video_streamer */

#endif // COPYBUFFER_H
