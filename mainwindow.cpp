#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>

/// Create a global matrix for store transverse X * X
float gloabl_matrix[2][2];
float transverse_x[2][4];
float dst[2][2];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    preSetup();
    connect(ui->x00, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->x01, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->x10, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->x11, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->x20, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->x21, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->x30, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->x31, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->y0, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->y1, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->y2, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
    connect(ui->y3, SIGNAL(editingFinished()), this, SLOT(revMatrixValue()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::revMatrixValue()
{
    x[0][0] = ui->x00->text().toFloat();
    x[0][1] = ui->x01->text().toFloat();
    x[1][0] = ui->x10->text().toFloat();
    x[1][1] = ui->x11->text().toFloat();
    x[2][0] = ui->x20->text().toFloat();
    x[2][1] = ui->x21->text().toFloat();
    x[3][0] = ui->x30->text().toFloat();
    x[3][1] = ui->x31->text().toFloat();

    y[0] = ui->y0->text().toFloat();
    y[1] = ui->y1->text().toFloat();
    y[2] = ui->y2->text().toFloat();
    y[3] = ui->y3->text().toFloat();

}

void MainWindow::generateGlobalMatrix()
{
    float sum = 0.0;

    /// transverse X
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 4; j++)
            transverse_x[i][j] = x[j][i];

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            sum = 0.0;
            for(int k = 0; k < 4; k++) {
                float tmp = transverse_x[i][k] * x[k][j];
                sum += tmp;
            }
            gloabl_matrix[i][j] = sum;
        }
    }
}

void MainWindow::preSetup()
{
    x[0][0] = ui->x00->text().toFloat();
    x[0][1] = ui->x01->text().toFloat();
    x[1][0] = ui->x10->text().toFloat();
    x[1][1] = ui->x11->text().toFloat();
    x[2][0] = ui->x20->text().toFloat();
    x[2][1] = ui->x21->text().toFloat();
    x[3][0] = ui->x30->text().toFloat();
    x[3][1] = ui->x31->text().toFloat();

    y[0] = ui->y0->text().toFloat();
    y[1] = ui->y1->text().toFloat();
    y[2] = ui->y2->text().toFloat();
    y[3] = ui->y3->text().toFloat();
}

void MainWindow::on_actionCalculation_triggered()
{
    generateGlobalMatrix();
    inverseMatrix();
    matrixMultiply();


    ui->t0->setText(QString::number(t[0]));
    ui->t1->setText(QString::number(t[1]));

}

void MainWindow::inverseMatrix()
{
    size_t i, j, k, is, js, a, b;
        float pivot, tmp;
        float row_tmp = 0.0;

        for (a = 0; a < 2; a++)
        for (b = 0; b < 2; b++)
            dst[a][b] = 0.0;
        for (a = 0; a < 2; a++) dst[a][a] = 1.0;

        for (k = 0; k < 2; ++k) {
            pivot = 0.0;
            for (i = k; i < 2; ++i) for (j = k; j < 2; ++j)
            if (pivot < (tmp = std::abs(gloabl_matrix[i][k])))
                pivot = tmp, is = i, js = j;

            if (pivot <= 1e-9) {
                ui->statusBar->showMessage("No solution.");
                return;
            }

            if (is != k) {
                for (a = 0; a < 2; a++) {
                    row_tmp = gloabl_matrix[is][a];
                    gloabl_matrix[is][a] = gloabl_matrix[k][a];
                    gloabl_matrix[k][a] = row_tmp;
                }
                for (a = 0; a < 2; a++) {
                    row_tmp = dst[is][a];
                    dst[is][a] = dst[k][a];
                    dst[k][a] = row_tmp;
                }
            }

            if (js != k) {
                for (a = 0; a < 2; ++a) {
                    std::swap(gloabl_matrix[a][k], gloabl_matrix[a][js]);
                    std::swap(dst[a][k], dst[a][js]);
                }
            }

            pivot = gloabl_matrix[k][k];

            for (a = 0; a < 2; a++) {
                gloabl_matrix[k][a] /= pivot;
                dst[k][a] /= pivot;
            }

            for (i = 0; i < 2; ++i) {
                if (i != k) {
                    tmp = -gloabl_matrix[i][k];
                    for (a = 0; a < 2; a++) {
                        gloabl_matrix[i][a] += tmp * gloabl_matrix[k][a];
                        dst[i][a] += tmp * dst[k][a];
                    }
                }

            }
        }

}

void MainWindow::matrixMultiply()
{
    float tmp[2][4];
    float sum;
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 4; j++) {
                sum = 0.0;
                for(int k = 0; k < 2; k++) {
                    float val = dst[i][k] * transverse_x[k][j];
                    sum += val;
                }
                tmp[i][j] = sum;
        }
    }


    for(int i = 0; i < 2; i++) {
        sum = 0.0;
        for(int j = 0; j < 4; j++) {
            float val = tmp[i][j] * y[j];
            sum += val;
        }
        t[i] = sum;
    }

    qDebug() << t[0] << t[1];
}





