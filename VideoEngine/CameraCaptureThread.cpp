#include "CameraCaptureThread.h"

#include <QImage>
#include <QMutex>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <QDebug>
#include <libv4lconvert.h>
#include <stdint.h>

struct CameraCaptureThread::Private {
	QMutex mutex;
	QImage image;
	v4lconvert_data *convert_data = nullptr;
};

CameraCaptureThread::CameraCaptureThread()
    : m(new Private),
      video_device_handler_ { new video_streamer::VideoDeviceHandler },
      device_buffer_control_ { new video_streamer::DeviceBuffersControl(video_device_handler_)},
      temporary_video_capture_buffer_ {new video_streamer::TemporaryVideoCaptureBuffer },
      request_video_capture_buffer_ { new video_streamer::RequestVideoCaptureBuffer },
      v42lbuffer_ { new video_streamer::V4L2Buffer },
      video_buffer_ { new video_streamer::VideoBuffer(device_buffer_control_, v42lbuffer_, video_device_handler_) },
      copy_buffer_ { new video_streamer::CopyBuffer },
      stream_data_format_ { new video_streamer::StreamDataFormat }
{

}

CameraCaptureThread::~CameraCaptureThread()
{
	delete m;
    delete stream_data_format_;
    delete copy_buffer_;
    delete video_buffer_;
    delete v42lbuffer_;
    delete request_video_capture_buffer_;
    delete temporary_video_capture_buffer_;
    delete device_buffer_control_;
    delete video_device_handler_;
}

void CameraCaptureThread::startCapture()
{
    if (!video_device_handler_->OpenDevice("/dev/video0")) {

        LOG_ERROR("%s", "Failed to open the camera device");
        return;
    }

    if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_G_FMT, &temporary_video_capture_buffer_->GetBuffer())) {

        LOG_ERROR("%s", "Failed to set buffer type VIDIOC_G_FMT by SetBuffreForDevice()");
        return;
    }

    stream_data_format_->GetDstFormat() = stream_data_format_->GetSrcFormat() = temporary_video_capture_buffer_->GetBuffer();
    stream_data_format_->SetPixelformat(V4L2_PIX_FMT_RGB24);

    m->convert_data = v4lconvert_create(video_device_handler_->GetDeviceFd());
    v4lconvert_try_format(m->convert_data, &stream_data_format_->GetDstFormat(), &stream_data_format_->GetSrcFormat());
    stream_data_format_->GetSrcFormat() = temporary_video_capture_buffer_->GetBuffer();

    if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_REQBUFS, &request_video_capture_buffer_->GetBuffer())) {

        LOG_ERROR("%s", "Failed to set buffer type VIDIOC_REQBUFS by SetBuffreForDevice");
        return;
    }

    if (!video_buffer_->MapBufferToMemory()) {

        LOG_ERROR("%s", "Failed to map video device buffer to memory");
        return;
    }

    if (!video_buffer_->SetUpBuffer()) {

        LOG_ERROR("%s", "Failed to setup video buffer");
        return;
    }

	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_STREAMON, &type)) {

        LOG_ERROR("%s", "Failed to set buffer type VIDIOC_STREAMON by SetBuffreForDevice");
        return;
    }

}

void CameraCaptureThread::copyBuffer()
{
	fd_set fds;
	FD_ZERO(&fds);
    FD_SET(video_device_handler_->GetDeviceFd(), &fds);

    while (select(video_device_handler_->GetDeviceFd() + 1, &fds, NULL, NULL, NULL) < 0) {
		if (isInterruptionRequested()) return;
	}

    if (FD_ISSET(video_device_handler_->GetDeviceFd(), &fds)) {

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_DQBUF, &copy_buffer_->GetBuffer())) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_DQBUF by SetBuffreForDevice");
            return;
        }

        {
            QMutexLocker lock(&m->mutex);
            m->image = QImage(stream_data_format_->GetDstFormat().fmt.pix.width, stream_data_format_->GetDstFormat().fmt.pix.height, QImage::Format_RGB888);
            v4lconvert_convert(m->convert_data, &stream_data_format_->GetSrcFormat(), &stream_data_format_->GetDstFormat(),
                               (unsigned char *)v42lbuffer_->v4l2Buffer2[copy_buffer_->GetBuffer().index], copy_buffer_->GetBuffer().bytesused, m->image.bits(),
                                stream_data_format_->GetDstFormat().fmt.pix.sizeimage);
        }

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_QBUF, &copy_buffer_->GetBuffer())) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QBUF by SetBuffreForDevice");
            return;
        }
	}
}

void CameraCaptureThread::stopCapture()
{
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_STREAMOFF, &type)) {

        LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QBUF by SetBuffreForDevice");
        return;
    }

    if (!video_buffer_->UnmapBuffer()) {

        LOG_WARNING("%s", "Failed to unmap video device buffer to memory ");
    }

	v4lconvert_destroy(m->convert_data);

    if (!video_device_handler_->CloseDevice()) {
        LOG_WARNING("%s", "Failed to close the camera device");
    }
}

void CameraCaptureThread::run()
{
	startCapture();
	while (1) {
		if (isInterruptionRequested()) break;
		copyBuffer();
	}
	stopCapture();
}

QImage CameraCaptureThread::image()
{
	QMutexLocker lock(&m->mutex);
	QImage im;
	std::swap(im, m->image);
	return im;
}
