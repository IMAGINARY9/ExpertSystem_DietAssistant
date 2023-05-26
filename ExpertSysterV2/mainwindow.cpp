#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , expSys(new ExpertSystem(this))
{
    ui->setupUi(this);

    m_model = new QStringListModel(this);
    sel_model = new QStringListModel(this);

    ui->symtomsView->setModel(m_model);
    ui->symtomsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->selectedSymptomsView->setModel(sel_model);
    ui->selectedSymptomsView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->categoriesBox->addItem(expSys->getCategoryBreak());
    ui->categoriesBox->addItems(expSys->getCategories());

    models_update();
    connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::select_symptom);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::remove_symptom);

    connect(ui->finishButton, &QPushButton::clicked, expSys, &ExpertSystem::finish);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clear_selection);

    connect(ui->searchLine, &QLineEdit::textChanged, this, &MainWindow::search);
    connect(ui->categoriesBox, &QComboBox::currentTextChanged, this, &MainWindow::select_category);

    connect(expSys, &ExpertSystem::systemSuccess, this, &MainWindow::successMessage);
    connect(expSys, &ExpertSystem::systemError, this, &MainWindow::errorMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::models_update()
{
    m_model->setStringList(expSys->getSymptomsList());
    sel_model->setStringList(expSys->getSel_symptomsList());
}

void MainWindow::search(const QString &input)
{
    expSys->search(input);
    models_update();
}

void MainWindow::select_category(const QString &category)
{
    expSys->setCategory(category);
    models_update();
}

void MainWindow::select_symptom()
{
    QString item = get_modelItem(*(ui->symtomsView), *m_model);
    ui->categoriesBox->setCurrentIndex(-1);
    //update add
    expSys->select(item);
    models_update();
}

void MainWindow::remove_symptom()
{
    QString item = get_modelItem(*(ui->selectedSymptomsView), *sel_model);
    ui->categoriesBox->setCurrentIndex(-1);
    //update del
    expSys->remove(item);
    models_update();
}

void MainWindow::clear_selection()
{
    ui->categoriesBox->setCurrentIndex(-1);
    expSys->clear();
    models_update();
}

QString MainWindow::get_modelItem(const QListView &view, const QStringListModel &model)
{
    QModelIndex selectedIndex = view.currentIndex();
    if (!selectedIndex.isValid()) {
        selectedIndex = model.index(0, 0);
    }

    return model.data(selectedIndex).toString();
}



void MainWindow::successMessage(const QStringList &missing, const QStringList &medicines)
{
    QString result = "Your body lacks: ";
    for (const QString &item : missing) {
        result.append('\n' + item);
    }
    result.append("\n\nIt's recommended to add the following foods to your diet:\n");
    int last = medicines.size() - 1;
    for (int i = 0; i < last; i++) {
        result.append(medicines.at(i) + ", ");
    }
    result.append(medicines.at(last) + '.');

    QMessageBox::information(this, "System success", result);

    clear_selection();
}

void MainWindow::errorMessage(const QString &error)
{
    QMessageBox::critical(this, "System error", error);
}
