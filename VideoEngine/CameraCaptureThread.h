#ifndef CAMERACAPTURETHREAD_H
#define CAMERACAPTURETHREAD_H

#include "Video-Device-Handler.h"

#include <QThread>

class CameraCaptureThread : public QThread {
private:
	struct Private;
	Private *m;

	void startCapture();
	void copyBuffer();
	void stopCapture();

protected:
	void run();

public:
	CameraCaptureThread();
	~CameraCaptureThread();
	QImage image();

private:
video_streamer::VideoDeviceHandler *video_device_streamer_;

};

#endif // CAMERACAPTURETHREAD_H
