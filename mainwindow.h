#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

    QVector<double> qv_x;
    QVector<double> qv_y;

    bool enAutoScale;
};
#endif // MAINWINDOW_H
