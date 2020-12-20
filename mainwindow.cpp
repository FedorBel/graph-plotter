#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ui->plot->setInteraction(QCP::iRangeDrag, false);
//    ui->plot->setInteraction(QCP::iRangeZoom, false);
//    ui->plot->setInteraction(QCP::iSelectPlottables, true);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                     QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->plot->xAxis->setLabel("t, [s]");
    ui->plot->legend->setVisible(true);
    ui->plot->graph(0)->setName("U, [v]");

    ui->plot->plotLayout()->insertRow(0);
    QCPTextElement *title = new QCPTextElement(ui->plot, "Graph Example", QFont("sans", 17, QFont::Bold));
    ui->plot->plotLayout()->addElement(0, 0, title);

    // connect slot that ties some axis selections together (especially opposite axes):
    connect(ui->plot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(ui->plot, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));

    connect(ui->plot, &QCustomPlot::mousePress, this, &MainWindow::slotMousePress);
    connect(ui->plot, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);

    // Initialize the router
    tracer = new QCPItemTracer(ui->plot);
    tracer->setGraph(ui->plot->graph(0));
    tracer->setVisible(false);
    enTracer = false;

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
    ui->plot->xAxis->setRange(*std::min_element(qv_x.begin(), qv_x.end()) * 1.05, *std::max_element(qv_x.begin(), qv_x.end())* 1.05);
    ui->plot->yAxis->setRange(*std::min_element(qv_y.begin(), qv_y.end())* 1.1, *std::max_element(qv_y.begin(), qv_y.end())* 1.1);
}


void MainWindow::on_btn_zoomFull_clicked()
{
    autoScale();
    plot();
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
    if (enTracer)
        return;
    // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
    // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
    double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
    double dataTimeStamp = plottable->interface1D()->dataMainKey(dataIndex);
    QString message = QString("Clicked on graph '%1' at data point #%2 with coords [%3, %4].").arg(plottable->name()).arg(dataIndex).arg(dataTimeStamp).arg(dataValue);
    ui->statusBar->showMessage(message, 10000);
}

void MainWindow::slotMousePress(QMouseEvent *event)
{
    if (enTracer)
    {
        // Find X coordinate on the graph where the mouse being clicked
        double coordX = ui->plot->xAxis->pixelToCoord(event->pos().x());
        // According to the X coordinate mouse clicks define the next position for the tracer
        tracer->setGraphKey(coordX);
        ui->plot->replot(); // redraw

        double dataValue = ui->plot->graph(0)->dataMainValue(coordX);
        double dataTimeStamp = ui->plot->graph(0)->dataMainKey(coordX);
        QString message = QString("Clicked on graph '%1' at data point with coords [%2, %3].").arg(ui->plot->graph(0)->name()).arg(dataTimeStamp).arg(dataValue);
        ui->statusBar->showMessage(message, 0);
    }
}

void MainWindow::slotMouseMove(QMouseEvent *event)
{
    if (enTracer)
    {
        if (QApplication::mouseButtons()) slotMousePress(event);
    }
}


void MainWindow::on_checkBox_useTracer_stateChanged(int arg1)
{
//    qDebug() << "Use Tracer = " << ui->checkBox_useTracer->isChecked();
//    enTracer = ui->checkBox_useTracer->isChecked();
    enTracer = arg1;
    qDebug() << "Use Tracer = " << enTracer;
    tracer->setVisible(enTracer);
    ui->plot->replot();
    ui->plot->setInteraction(QCP::iSelectPlottables, !enTracer);
    ui->plot->setInteraction(QCP::iSelectPlottables, false);
    ui->statusBar->clearMessage();
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->plot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->plot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->plot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->plot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->plot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->plot->graph(i);
    QCPPlottableLegendItem *item = ui->plot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (ui->plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->plot->axisRect()->setRangeDrag(ui->plot->xAxis->orientation());
  else if (ui->plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->plot->axisRect()->setRangeDrag(ui->plot->yAxis->orientation());
  else
    ui->plot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->plot->axisRect()->setRangeZoom(ui->plot->xAxis->orientation());
  else if (ui->plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->plot->axisRect()->setRangeZoom(ui->plot->yAxis->orientation());
  else
    ui->plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}
