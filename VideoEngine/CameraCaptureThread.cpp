#include "CameraCaptureThread.h"

CameraCaptureThread::CameraCaptureThread()
    :
      video_device_handler_ { new video_streamer::VideoDeviceHandler },
      device_buffer_control_ { new video_streamer::DeviceBuffersControl(video_device_handler_)},
      temporary_video_capture_buffer_ {new video_streamer::TemporaryVideoCaptureBuffer },
      request_video_capture_buffer_ { new video_streamer::RequestVideoCaptureBuffer },
      v42lbuffer_ { new video_streamer::V4L2Buffer },
      video_buffer_ { new video_streamer::VideoBuffer(device_buffer_control_, v42lbuffer_, video_device_handler_) },
      copy_buffer_ { new video_streamer::CopyBuffer },
      stream_data_format_ { new video_streamer::StreamDataFormat },
      convert_data_ { new video_streamer::ConvertData(video_device_handler_) },
      image_handler_ { new video_streamer::ImageHandler }
{
}

CameraCaptureThread::~CameraCaptureThread()
{
    delete image_handler_;
    delete convert_data_;
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

    convert_data_->CreateConvertData();
    convert_data_->TryFormat(&stream_data_format_->GetDstFormat(), &stream_data_format_->GetSrcFormat());
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
            QMutexLocker lock(&image_handler_->GetImageMutex());
            image_handler_->GetImage() = QImage(stream_data_format_->GetDstFormat().fmt.pix.width, stream_data_format_->GetDstFormat().fmt.pix.height, QImage::Format_RGB888);
            v4lconvert_convert(convert_data_->GetConvertData(), &stream_data_format_->GetSrcFormat(), &stream_data_format_->GetDstFormat(),
                               (unsigned char *)v42lbuffer_->v4l2Buffer2[copy_buffer_->GetBuffer().index], copy_buffer_->GetBuffer().bytesused, image_handler_->GetImage().bits(),
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

    convert_data_->DisposeConvertData();

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
    QMutexLocker lock(&image_handler_->GetImageMutex());
	QImage im;
    std::swap(im, image_handler_->GetImage());
	return im;
}
