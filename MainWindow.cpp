#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QPushButton>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,7);
    ui->splitter->setStretchFactor(1,3);

    QAction* action = new QAction(this);

    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return));
    addAction(action);
    connect(action, &QAction::triggered, this, [&] {
        setWindowTitle("等待回复中...");
        ui->text2->setReadOnly(true);
        const QString send_message = ui->text2->toPlainText();
         client.sendRequest(send_message);
         ui->text1->append(
             "request:\n" + send_message + "\n"
             );
         ui->text2->clear();
    });

    QObject::connect(&client, &DeepSeekClient::get_message,[&](const QString& message) {
        ui->text1->append(
            "message:\n" + message + "\n"
            );
        setWindowTitle("可发送消息");
        ui->text2->setReadOnly(false);
    });


}

MainWindow::~MainWindow() {
    delete ui;
}


