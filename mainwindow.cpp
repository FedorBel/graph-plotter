#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->plot->xAxis->setLabel("t, [s]");

    plotFakeData();
}

void MainWindow::plotFakeData()
{
    size_t sin_ampl = 5;
    size_t sin_arr_size = 20;
    double delta_angl_rad = 2*3.141592/sin_arr_size;
    qv_x.resize(sin_arr_size);
    qv_y.resize(sin_arr_size);
    for (size_t i = 0; i < sin_arr_size; i++)
    {
        qv_y[i] = sin_ampl * sin(i * delta_angl_rad);
        qv_x[i] = i;
    }
    autoScale();
    plot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addPoint(double x, double y)
{
    qv_x.append(x);
    qv_y.append(y);
}

void MainWindow::clearData()
{
    qv_x.clear();
    qv_y.clear();
}

void MainWindow::plot()
{
    ui->plot->graph(0)->setData(qv_x, qv_y);
    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::autoScale()
{
    ui->plot->xAxis->setRange(*std::min_element(qv_x.begin(), qv_x.end()), *std::max_element(qv_x.begin(), qv_x.end()));
    ui->plot->yAxis->setRange(*std::min_element(qv_y.begin(), qv_y.end()), *std::max_element(qv_y.begin(), qv_y.end()));
}


void MainWindow::on_btn_zoomFull_clicked()
{
    autoScale();
    plot();
}
