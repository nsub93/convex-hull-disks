#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <convexhullalgorithm.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
       void run();
       void info();
       void clear();
       void load();
       void random();
       void manual();

private:
    int randomInt(int,int);

    ConvexHullAlgorithm* algorithm;
    ConvexHull result;
    QVector<Disc*> discs;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
