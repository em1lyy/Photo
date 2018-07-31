#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QResizeEvent>

namespace Ui {
class MainWindow;
class Photo;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::string imagePath;
    bool resizeEnabled;
    explicit MainWindow(QWidget *parent = 0);
    void loadImage(std::string filename);
    void resizeEvent(QResizeEvent* event);
    void setResizingState();
    ~MainWindow();

private:
    Ui::Photo *ui;
};

#endif // MAINWINDOW_H
