#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addButton);
    connect(ui->detailsTaskButton, &QPushButton::clicked, this, &MainWindow::showDetailsButton);
    connect(ui->deleteTaskButton, &QPushButton::clicked, this, &MainWindow::deleteButton);

    // Загрузка задач при запуске приложения
    loadTasks();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Сохранение задач перед закрытием приложения
    saveTasks();
    event->accept();
}

void MainWindow::saveTasks()
{
    QSettings settings("MyCompany", "MyApp"); // Название организации и приложения

    // Очищаем старые настройки
    settings.remove("tasks");

    // Сохраняем количество задач
    int taskCount = ui->tasksListWidget->count();
    settings.beginWriteArray("tasks");
    for (int i = 0; i < taskCount; ++i) {
        settings.setArrayIndex(i);
        QListWidgetItem *item = ui->tasksListWidget->item(i);
        settings.setValue("title", item->text());
        settings.setValue("description", item->data(Qt::UserRole).toString());
        settings.setValue("date", item->data(Qt::UserRole + 1).toDate());
    }
    settings.endArray();
}

void MainWindow::loadTasks()
{
    QSettings settings("MyCompany", "MyApp"); // Название организации и приложения

    // Загружаем количество задач
    int taskCount = settings.beginReadArray("tasks");
    for (int i = 0; i < taskCount; ++i) {
        settings.setArrayIndex(i);
        QString title = settings.value("title").toString();
        QString description = settings.value("description").toString();
        QDate date = settings.value("date").toDate();

        // Создаем новый элемент и добавляем его в список
        QListWidgetItem *newItem = new QListWidgetItem(title);
        newItem->setData(Qt::UserRole, description);
        newItem->setData(Qt::UserRole + 1, date);
        ui->tasksListWidget->addItem(newItem);
    }
    settings.endArray();
}

void MainWindow::showDetailsButton()
{
    QListWidgetItem *item = ui->tasksListWidget->currentItem();
    if (item) {
        QString taskDetails = item->text();
        QString description = item->data(Qt::UserRole).toString();
        QDate taskDate = item->data(Qt::UserRole + 1).toDate();
        QString details = QString("<b>Задача:</b> %1\n<b>Описание:</b> %2\n<b>Дата выполнения:</b> %3")
            .arg(taskDetails)
            .arg(description)
            .arg(taskDate.toString("dd.MM.yyyy"));
        QMessageBox::information(this, "Детали задачи", details);
    } else {
        QMessageBox::warning(this, "Просмотр задачи", "Выберите задачу для просмотра.");
    }
}

void MainWindow::addButton()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить задачу");
    QVBoxLayout layout(&dialog);

    QLineEdit titleEdit;
    titleEdit.setPlaceholderText("Название задачи");
    layout.addWidget(&titleEdit);

    QTextEdit descriptionEdit;
    descriptionEdit.setPlaceholderText("Описание задачи");
    layout.addWidget(&descriptionEdit);

    QDateEdit dateEdit;
    dateEdit.setCalendarPopup(true);
    layout.addWidget(&dateEdit);

    QPushButton addButton("Добавить");
    layout.addWidget(&addButton);

    connect(&addButton, &QPushButton::clicked, [&]() {
        QString title = titleEdit.text().trimmed();
        QString description = descriptionEdit.toPlainText().trimmed();
        QDate date = dateEdit.date();

        // Проверка на пустое название задачи
        if (title.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Название задачи не может быть пустым!");
            return; // Не закрываем диалог, если название пустое
        }

        QListWidgetItem *newItem = new QListWidgetItem(title);
        newItem->setData(Qt::UserRole, description);
        newItem->setData(Qt::UserRole + 1, date);
        ui->tasksListWidget->addItem(newItem);
        dialog.accept();
    });

    dialog.exec();
}

void MainWindow::deleteButton()
{
    QListWidgetItem *item = ui->tasksListWidget->currentItem();
    if (item) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Удаление задачи");
        msgBox.setText("Вы уверены, что хотите удалить задачу?");
        QPushButton* yesButton = msgBox.addButton(QMessageBox::Yes);
        QPushButton* noButton = msgBox.addButton(QMessageBox::No);
        yesButton->setText("Да");
        noButton->setText("Нет");
        msgBox.exec();
        if (msgBox.clickedButton() == yesButton) {
            delete item;
        }
    } else {
        QMessageBox::warning(this, "Удаление задачи", "Выберите задачу для удаления.");
    }
}
