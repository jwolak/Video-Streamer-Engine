#include "Temporary-Video-Capture-Buffer.h"

#include "Logger.h"

v4l2_format & video_streamer::TemporaryVideoCaptureBuffer::GetBuffer() {

    return tmp_frmt_buffer_;
}
