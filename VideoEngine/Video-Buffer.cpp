#include "Video-Buffer.h"

#include <cstring>

#include <sys/mman.h> /* PROT_READ, MAP_SHARED */

bool video_streamer::VideoBuffer::MapBufferToMemory() {

    for (uint32_t i = 0; i < v4l2BufferSize; i++) {

        std::memset(&video_buffer_, 0, sizeof(video_buffer_));
        video_buffer_.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        video_buffer_.memory = V4L2_MEMORY_MMAP;
        video_buffer_.index  = i;

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_QUERYBUF, &video_buffer_)) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QUERYBUF by SetBuffreForDevice");
            return false;
        }

        v4l2buffer_->v4l2Buffer2[i] = mmap(NULL, video_buffer_.length, PROT_READ, MAP_SHARED, video_device_handler_->GetDeviceFd(), video_buffer_.m.offset);
        v4l2buffer_->v4l2BufferSize2[i] = video_buffer_.length;
    }

    return true;
}


bool video_streamer::VideoBuffer::SetUpBuffer() {

    for (uint32_t i = 0; i < v4l2BufferSize; i++) {
        memset(&video_buffer_, 0, sizeof(video_buffer_));
        video_buffer_.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        video_buffer_.memory = V4L2_MEMORY_MMAP;
        video_buffer_.index  = i;

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_QBUF, &video_buffer_)) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QBUF by SetBuffreForDevice");
            return false;
        }
    }

    return true;
}


bool video_streamer::VideoBuffer::UnmapBuffer() {

    for (uint32_t i = 0; i < v4l2BufferSize; i++) {
        munmap(v4l2buffer_->v4l2Buffer2[i], v4l2buffer_->v4l2BufferSize2[i]);
    }

    return true;
}
