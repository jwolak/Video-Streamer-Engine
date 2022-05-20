#include "Video-Buffer.h"

#include <cstring>

#include <sys/mman.h> /* PROT_READ, MAP_SHARED */

bool video_streamer::VideoBuffer::MapBufferToMemory() {

    for (uint32_t index = 0; index < v4l2BufferSize; ++index) {

        std::memset(&video_buffer_, 0, sizeof(video_buffer_));
        video_buffer_.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        video_buffer_.memory = V4L2_MEMORY_MMAP;
        video_buffer_.index  = index;

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_QUERYBUF, &video_buffer_)) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QUERYBUF by SetBuffreForDevice");
            return false;
        }

        void *tmp_mmap_reusult = mmap(NULL, video_buffer_.length, PROT_READ, MAP_SHARED, video_device_handler_->GetDeviceFd(), video_buffer_.m.offset);

        if (MAP_FAILED == tmp_mmap_reusult) {

            LOG_DEBUG("%s", "VideoBuffer::MapBufferToMemory(): mmap() has returned an error");
            LOG_ERROR("%s", "VideoBuffer::MapBufferToMemory() has failed");
            return false;
        }

        v4l2buffer_->v4l2Buffer2[index] = tmp_mmap_reusult;
        v4l2buffer_->v4l2BufferSize2[index] = video_buffer_.length;
    }

    return true;
}


bool video_streamer::VideoBuffer::SetUpBuffer() {

    for (uint32_t index = 0; index < v4l2BufferSize; ++index) {
        memset(&video_buffer_, 0, sizeof(video_buffer_));
        video_buffer_.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        video_buffer_.memory = V4L2_MEMORY_MMAP;
        video_buffer_.index  = index;

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_QBUF, &video_buffer_)) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QBUF by SetBuffreForDevice");
            return false;
        }
    }

    return true;
}


bool video_streamer::VideoBuffer::UnmapBuffer() {

    for (uint32_t index = 0; index < v4l2BufferSize; ++index) {
        if (munmap(v4l2buffer_->v4l2Buffer2[index], v4l2buffer_->v4l2BufferSize2[index]) < 0) {

            LOG_DEBUG("%s", "VideoBuffer::UnmapBuffer(): munmap()  returned an error");
            LOG_ERROR("%s", "VideoBuffer::UnmapBuffer() has failed");
            return false;
        }
    }

    return true;
}
