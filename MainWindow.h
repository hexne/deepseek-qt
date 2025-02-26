#pragma once

#include <QWidget>
#include "DeepseekClient.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



struct Config {
    bool stream{};
    std::string model{};
};

class MainWindow : public QWidget
{
    Q_OBJECT


    DeepSeekClient client_;
    QString markdown_;
    Ui::MainWindow *ui;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
};


