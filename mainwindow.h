#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override; // Переопределяем closeEvent

private slots:
    void addButton();              // Слот для добавления задачи
    void showDetailsButton();      // Слот для просмотра деталей задачи
    void deleteButton();           // Слот для удаления задачи

private:
    Ui::MainWindow *ui;

    // Объявляем функции для сохранения и загрузки задач
    void saveTasks();              // Сохранение задач в QSettings
    void loadTasks();              // Загрузка задач из QSettings
};

#endif // MAINWINDOW_H
