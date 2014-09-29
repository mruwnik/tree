#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void print(QString str);
    void angleChanged(double hor, double vert);
    void coordsChanged(double x, double y, double z);

    void updateTemp(int temp);
    void updateDeltaTemp(int temp);
    void updateAvgTemp(int temp);

    void updateTime(int newTime);
    void updateYear(int newTime);
    void updateTimeStep(int newTime);

    void updateAuxin(int auxin);
    void updateSegmentAuxinUsed(int auxin);
    void updateSegmentAuxinProd(int auxin);
    void updateBudAuxinUsed(int auxin);
    void updateBudAuxinProd(int auxin);
    void updateLeafAuxinUsed(int auxin);

    void tick();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
