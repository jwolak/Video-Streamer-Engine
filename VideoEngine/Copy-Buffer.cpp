#include "Copy-Buffer.h"

v4l2_buffer & video_streamer::CopyBuffer::GetBuffer() {

    return cpy_buffer_;
}
