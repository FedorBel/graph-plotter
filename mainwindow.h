#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot/qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void on_btn_zoomFull_clicked();
    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);
    void slotMousePress(QMouseEvent * event);
    void slotMouseMove(QMouseEvent * event);
    void selectionChanged();
    void mousePress();
    void mouseWheel();


    void on_checkBox_useTracer_stateChanged(int arg1);

    void on_checkBox_fixGraph_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    QVector<double> qv_x;
    QVector<double> qv_y;

    bool enAutoScale;
    bool enTracer;
    bool fixGraph;

    QCPItemTracer *tracer;
};
#endif // MAINWINDOW_H
