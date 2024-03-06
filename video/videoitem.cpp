#include "videoitem.h"
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <QImage>
#include <QPainter>

VideoItem::VideoItem(QQuickItem *parent) : QQuickPaintedItem(parent), port(5600) {
    qRegisterMetaType<cv::Mat>("cv::Mat");
    _frame = cv::Mat(480, 640, CV_8UC3); // Initialize a 480x640 color (3-channel) matrix
    gst_init(nullptr, nullptr);
    run();
}

void VideoItem::paint(QPainter *painter) {
    if (!_frame.empty()) { // Check if the frame is not empty
        QImage img(_frame.data, _frame.cols, _frame.rows, _frame.step, QImage::Format_RGB888);
        painter->drawImage(boundingRect(), img);
    }
}


GstFlowReturn VideoItem::callback(GstElement *sink, gpointer data) {
    VideoItem *self = static_cast<VideoItem*>(data);
    GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
    GstBuffer *buf = gst_sample_get_buffer(sample);
    GstCaps *caps = gst_sample_get_caps(sample);
    GstStructure *structure = gst_caps_get_structure(caps, 0);

    int width = 0, height = 0;
    gst_structure_get_int(structure, "width", &width);
    gst_structure_get_int(structure, "height", &height);

    GstMapInfo map;
    gst_buffer_map(buf, &map, GST_MAP_READ);

    self->_frame = cv::Mat(height, width, CV_8UC3, map.data).clone();

    gst_buffer_unmap(buf, &map);
    gst_sample_unref(sample);

    //self->update(); // Trigger repaint

    QMetaObject::invokeMethod(self, "updateFrame", Qt::QueuedConnection, Q_ARG(cv::Mat, self->_frame));
    //emit self->update();
    return GST_FLOW_OK;
}

void VideoItem::start_gst() {
    gchar *descr = g_strdup_printf("udpsrc port=%d ! application/x-rtp, payload=96 ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! video/x-raw,format=BGR ! videoconvert ! appsink name=sink", port);
    GError *error = nullptr;

    video_pipe = gst_parse_launch(descr, &error);
    g_free(descr);

    if (error) {
        g_printerr("Error creating pipeline: %s\n", error->message);
        g_error_free(error);
        return;
    }

    GstElement *sink = gst_bin_get_by_name(GST_BIN(video_pipe), "sink");
    if (!sink) {
        g_printerr("Error: appsink not found in the pipeline\n");
        return;
    }else {
        qDebug() << "göasd";
    }
    gst_app_sink_set_emit_signals(GST_APP_SINK(sink), true);
    gst_app_sink_set_drop(GST_APP_SINK(sink), true);
    gst_app_sink_set_max_buffers(GST_APP_SINK(sink), 2);
    gst_base_sink_set_sync(GST_BASE_SINK(sink), false);
    g_signal_connect(sink, "new-sample", G_CALLBACK(callback), this);

    gst_element_set_state(video_pipe, GST_STATE_PLAYING);
}

void VideoItem::updateFrame(const cv::Mat &frame) {
    _frame = frame.clone();
    update(); // Trigger repaint in the GUI thread
}

void VideoItem::run() {
    start_gst();
}
