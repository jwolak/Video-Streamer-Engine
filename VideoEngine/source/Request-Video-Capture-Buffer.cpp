#include "headers/Request-Video-Capture-Buffer.h"

v4l2_requestbuffers &video_streamer::RequestVideoCaptureBuffer::GetBuffer() {

    return request_buffer_;
}
