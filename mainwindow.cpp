#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glade.h"
#include "parts.h"
#include "weather.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->glade->setFocusPolicy(Qt::StrongFocus);

    connect(ui->glade, SIGNAL(angleChanged(double, double)), this, SLOT(angleChanged(double, double)));
    connect(ui->glade, SIGNAL(coordsChanged(double, double, double)), this, SLOT(coordsChanged(double, double, double)));

    connect(ui->temperatureSetter, SIGNAL(valueChanged(int)), this, SLOT(updateTemp(int)));
    connect(ui->deltaTemperature, SIGNAL(valueChanged(int)), this, SLOT(updateDeltaTemp(int)));
    connect(ui->avgTemperature, SIGNAL(valueChanged(int)), this, SLOT(updateAvgTemp(int)));

    connect(ui->timeSetter, SIGNAL(valueChanged(int)), this, SLOT(updateTime(int)));
    connect(ui->year, SIGNAL(valueChanged(int)), this, SLOT(updateYear(int)));
    connect(ui->timeStep, SIGNAL(valueChanged(int)), this, SLOT(updateTimeStep(int)));
    connect(ui->resetTree, SIGNAL(clicked()), ui->glade, SLOT(resetTree()));

    connect(ui->auxin, SIGNAL(valueChanged(int)), this, SLOT(updateAuxin(int)));
    connect(ui->segmentAuxin, SIGNAL(valueChanged(int)), this, SLOT(updateSegmentAuxinUsed(int)));
    connect(ui->segmentAuxinProd, SIGNAL(valueChanged(int)), this, SLOT(updateSegmentAuxinProd(int)));
    connect(ui->budAuxin, SIGNAL(valueChanged(int)), this, SLOT(updateBudAuxinUsed(int)));
    connect(ui->budAuxinProd, SIGNAL(valueChanged(int)), this, SLOT(updateBudAuxinProd(int)));
    connect(ui->leafAuxin, SIGNAL(valueChanged(int)), this, SLOT(updateLeafAuxinUsed(int)));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    connect(timer, SIGNAL(timeout()), ui->glade, SLOT(tick()));
    timer->start(10);

    updateTemp(Weather::getTemp());
    updateDeltaTemp(Weather::getDeltaTemp());
    updateAvgTemp(Weather::getAvgTemp());
    updateTime(Weather::getTime());
    updateYear(Weather::getYear());
    updateTimeStep(Weather::getTimeStep());

    TreeParams params = ui->glade->getTreeParams(0);
    updateBudAuxinProd(params.getBudHormonesProd().auxin*10);
    updateBudAuxinUsed(params.getBudHormones().auxin*10);
    updateSegmentAuxinUsed(params.getSegmentHormones().auxin*10);
    updateSegmentAuxinProd(params.getSegmentHormonesProd().auxin*10);
 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::print(QString str){
   //ui->console->setPlainText(str);
}

void MainWindow::angleChanged(double hor, double vert){
    ui->angles->setText(QString::number(hor) + ", " + QString::number(vert));
}

void MainWindow::coordsChanged(double x, double y, double z){
    ui->positions->setText(QString::number(x) + ", " + QString::number(y) + ", " + QString::number(z));
}

void MainWindow::updateTemp(int temp){
    temp = temp * Weather::getBaseTempStep();
    Weather::setTemp(temp);
    ui->temperature->setText(QString::number(temp));
}

void MainWindow::updateDeltaTemp(int temp){
    temp = temp * Weather::getBaseTempStep();
    Weather::setDeltaTemp(temp);
    ui->deltaTemperatureLabel->setText(QString::number(temp));
}

void MainWindow::updateAvgTemp(int temp){
    temp = temp * Weather::getBaseTempStep();
    Weather::setAvgTemp(temp);
    ui->avgTemperatureVal->setText(QString::number(temp));
}

void MainWindow::updateTime(int newTime){
    int year = 24*365;
    Weather::setTime(newTime);
    if(Weather::getTime() > year){
        Weather::setYear(Weather::getYear() + Weather::getTime() / year);
        Weather::setTime(Weather::getTime() - year);
    }
    ui->time->setText(Weather::getFormatedTime());
}

void MainWindow::updateYear(int newTime){
    Weather::setYear(newTime);
    ui->yearLabel->setText(QString::number(newTime));
}

void MainWindow::updateTimeStep(int newTime){
    Weather::setTimeStep(newTime);
    ui->timeStepVal->setText(Weather::getFormatedTimeStep());
}

void MainWindow::updateAuxin(int auxin){
    ui->auxinValue->setText(QString::number(auxin/10.0));
}
void MainWindow::updateSegmentAuxinUsed(int auxin){
    TreeParams params = ui->glade->getTreeParams(0);
    hormones h = params.getSegmentHormones();
    h.auxin = auxin/10.0;
    params.setSegmentHormones(h);
    ui->glade->setTreeParams(0, params);
    ui->segmentAuxinLabel->setText(QString::number(h.auxin));
}
void MainWindow::updateSegmentAuxinProd(int auxin){
    TreeParams params = ui->glade->getTreeParams(0);
    hormones h = params.getSegmentHormonesProd();
    h.auxin = auxin/10.0;
    params.setSegmentHormonesProd(h);
    ui->glade->setTreeParams(0, params);
    ui->segmentAuxinProdLabel->setText(QString::number(h.auxin));
}
void MainWindow::updateBudAuxinUsed(int auxin){
    TreeParams params = ui->glade->getTreeParams(0);
    hormones h = params.getBudHormones();
    h.auxin = auxin/10.0;
    params.setBudHormones(h);
    ui->glade->setTreeParams(0, params);
    ui->budAuxinLabel->setText(QString::number(h.auxin));
}
void MainWindow::updateBudAuxinProd(int auxin){
    TreeParams params = ui->glade->getTreeParams(0);
    hormones h = params.getBudHormonesProd();
    h.auxin = auxin/10.0;
    params.setBudHormonesProd(h);
    ui->glade->setTreeParams(0, params);
    ui->budAuxinProdLabel->setText(QString::number(h.auxin));
}
void MainWindow::updateLeafAuxinUsed(int auxin){
    /*TreeParams params = ui->glade->getTreeParams(0);
    hormones h = params.getLeafUse();
    h.auxin = auxin/10.0;
    params.setLeafUse(h);
    ui->glade->setTreeParams(0, params);
    ui->leafAuxinLabel->setText(QString::number(h.auxin));*/
}


void MainWindow::tick(){
    Weather::setTime(Weather::getTime() + Weather::getTimeStep());
    updateTime(Weather::getTime());
    updateYear(Weather::getYear());

    const double year = 24*365;

    Weather::setTemp((Weather::getAvgTemp()
                        - cos((((double)Weather::getTime())/year)*2*3.14) * Weather::getDeltaTemp()/2)
                     / Weather::getBaseTempStep());
    updateTemp(Weather::getTemp());

    updateAuxin(ui->glade->getTreeHorms(0).auxin*10);
}
