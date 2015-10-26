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
    void revMatrixValue();

private slots:
    void on_actionCalculation_triggered();

private:
    Ui::MainWindow *ui;
    double x[4][2];
    double y[4];
    float t[2];

    void preSetup();
    void generateGlobalMatrix();
    void inverseMatrix();
    void matrixMultiply();


};

#endif // MAINWINDOW_H
