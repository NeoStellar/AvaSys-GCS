#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include "gst/gstelement.h"
#include <QQuickPaintedItem>
#include <opencv4/opencv2/core/mat.hpp>
class VideoItem : public QQuickPaintedItem {
    Q_OBJECT
public:
    VideoItem(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;
    Q_INVOKABLE void start_gst();
public slots:
    void updateFrame(const cv::Mat &frame);
signals:
    Q_INVOKABLE void frameUpdated();
private:
    int port;
    cv::Mat _frame;
    GstElement *video_pipe;

    static GstFlowReturn callback(GstElement *sink, gpointer data);


    void run();
};

#endif // VIDEOITEM_H
