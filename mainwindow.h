#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dhcp_client.h"

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
    void on_button_release_clicked();

    void on_button_discover_clicked();

    void on_button_request_clicked();

    void update_gui();

private:
    Ui::MainWindow *ui;
    dhcp_client *cl;
};

#endif // MAINWINDOW_H
