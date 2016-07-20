#ifndef _OPEN_DSP_PLOTWINDOW_HPP_
#define _OPEN_DSP_PLOTWINDOW_HPP_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>

#include <OpenDsp/Gui/qcustomplot/qcustomplot.h>

#include <QApplication>
#include <QtWidgets/QVBoxLayout>

namespace opendsp
{
template<typename Sample_T>
class PlotWindow
{
public:
    PlotWindow(Signal<Sample_T>& signal)
    {
        char* args;
        int argc = 0;
        app = new QApplication(argc,&args);

        mainWindow = new QMainWindow();
        centralWidget = new QWidget(mainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        customPlot = new QCustomPlot(centralWidget);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        verticalLayout->addWidget(customPlot);

        // generate some data:
        QVector<double> x(101), y(101); // initialize with entries 0..100
        for (int i=0; i<101; ++i)
        {
            x[i] = i/50.0 - 1; // x goes from -1 to 1
            y[i] = x[i]*x[i]; // let's plot a quadratic function
        }
        // create graph and assign data to it:
        customPlot->addGraph();
        customPlot->graph(0)->setData(x, y);
        // give the axes some labels:
        customPlot->xAxis->setLabel("x");
        customPlot->yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        customPlot->xAxis->setRange(-1, 1);
        customPlot->yAxis->setRange(0, 1);
        customPlot->replot();

        mainWindow->setCentralWidget(centralWidget);
        mainWindow->show();
        mainWindow->resize(512,512);

        app->exec();
    }

private:
    QApplication* app;
    QMainWindow* mainWindow;
    QWidget* centralWidget;
    QVBoxLayout* verticalLayout;
    QCustomPlot* customPlot;
};

}

#endif // PLOTWINDOW_HPP
