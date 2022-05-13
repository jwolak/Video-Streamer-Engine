#ifndef V4L2BUFFER_H
#define V4L2BUFFER_H

#include <cstdint>

namespace video_streamer {

static const int v4l2BufferSize = 2;

class V4L2Buffer
{
public:
    V4L2Buffer() {}

public:
    void *v4l2Buffer2[v4l2BufferSize];
    uint32_t v4l2BufferSize2[v4l2BufferSize];
};

} /* namespace video_streamer */

#endif // V4L2BUFFER_H
