#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QString>
#include <QPixmap>
#include <QVBoxLayout>
#include <QSize>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Photo)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(this->ui->image);
    this->ui->centralwidget->setLayout(layout);
    connect(this->ui->checkResize, &QCheckBox::clicked, this, &MainWindow::setResizingState);
    this->resizeEnabled = true;
}

void MainWindow::loadImage(std::string filename)
{
    this->imagePath = filename;
    QPixmap *img = new QPixmap(QString::fromStdString(filename));
    this->resize(img->size());
    if (this->ui->checkKeepAR->checkState() == Qt::Checked)
    {
        if (this->ui->checkResize->checkState() == Qt::Checked)
        {
            this->ui->image->setPixmap(img->scaled(this->width(), this->height(), Qt::KeepAspectRatio));
        }
    }
    else
    {
        this->ui->image->setPixmap(*img);
    }
    // this->ui->image->setPixmap(*img);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (this->resizeEnabled == true)
    {
        QPixmap *img = new QPixmap(QString::fromStdString(this->imagePath));
        if (this->ui->checkKeepAR->checkState() == Qt::Checked)
        {
            if (this->ui->checkResize->checkState() == Qt::Checked)
            {
                this->resizeEnabled = false;
                this->resize(img->scaled(this->ui->centralwidget->width(), this->ui->centralwidget->height(), Qt::KeepAspectRatio).size());
                this->ui->image->setPixmap(img->scaled(this->width(), this->height(), Qt::KeepAspectRatio));
            }
        }
        else
        {
            this->ui->image->setPixmap(*img);
        }
    }
    else
    {
        this->resizeEnabled = true;
    }
    this->ui->checkResize->setGeometry(0, this->height() - 22, 87, 20);
    this->ui->checkKeepAR->setGeometry(88, this->height() - 22, 116, 20);
    // this->ui->image->setPixmap(*img);
}

void MainWindow::setResizingState()
{
    if (this->ui->checkResize->checkState() == Qt::Checked)
    {
        this->ui->image->setScaledContents(true);
    }
    else
    {
        this->ui->image->setScaledContents(false);
        this->resize(this->ui->image->size());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
