#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "expertsystem.h"

#include <QMainWindow>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QListView>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void models_update();
    void search(const QString &input);
    void select_category(const QString &category);
    void select_symptom();
    void remove_symptom();
    void clear_selection();
    void clear_category();
    QString get_modelItem(const QListView &view, const QStringListModel &model);

public slots:
    void successMessage(const QStringList &missing, const QStringList &medicines);
    void errorMessage(const QString &error);

private:
    Ui::MainWindow *ui;
    ExpertSystem *expSys;
    QStringListModel *m_model;
    QStringListModel *sel_model;
};
#endif // MAINWINDOW_H
