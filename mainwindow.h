#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot/qcustomplot.h>
#include <QSerialPort>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addPoint(double x, double y);
    void clearData();
    void plot();
    void plotFakeData();
    void autoScale();

private slots:
    // Plot
    void on_btn_zoomFull_clicked();
    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);
    void slotMousePress(QMouseEvent * event);
    void slotMouseMove(QMouseEvent * event);
    void selectionChanged();
    void mousePress();
    void mouseWheel();

    // Real-time plotting
    // void realtimeDataSlot();

    // Serial
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);

    // Other
    void on_checkBox_useTracer_stateChanged(int arg1);

    void on_checkBox_fixGraph_stateChanged(int arg1);

    void on_checkBox_stopPlotting_stateChanged(int arg1);

private:
    void initActionsConnections();

private:
    void showStatusMessage(const QString &message);

    Ui::MainWindow *ui;

    QVector<double> qv_x;
    QVector<double> qv_y;

    bool enAutoScale;
    bool enTracer;
    bool fixGraph;

    QCPItemTracer *tracer;

    // Real-time plotting
    QTimer dataTimer;
    double currentValue = 0;
    void plotRealTimeData();

    // Serial
    QLabel *m_status = nullptr;
    SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;
//    QByteArray serialData;
    QString serialBuffer = "";
    bool firstRead = true;
};
#endif // MAINWINDOW_H
