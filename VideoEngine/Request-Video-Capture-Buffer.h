#ifndef REQUESTVIDEOCAPTUREBUFFER_H
#define REQUESTVIDEOCAPTUREBUFFER_H

#include <linux/videodev2.h> /* v4l2_requestbuffers */
#include <cstring>

static const int v4l2BufferNumSize = 2;

namespace video_streamer {

class RequestVideoCaptureBuffer
{
public:
    RequestVideoCaptureBuffer() {

        std::memset(&request_buffer_, 0, sizeof(request_buffer_));
        request_buffer_.count  = v4l2BufferNumSize;
        request_buffer_.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        request_buffer_.memory = V4L2_MEMORY_MMAP;
    }

    v4l2_requestbuffers &GetBuffer();

private:
    struct v4l2_requestbuffers request_buffer_;
};

} /* namespace video_streamer */

#endif // REQUESTVIDEOCAPTUREBUFFER_H
