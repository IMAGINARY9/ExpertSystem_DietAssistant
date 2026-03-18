#include "questionsdialog.h"
#include "qmessagebox.h"
#include "ui_questionsdialog.h"

#include <QCheckBox>

#include <QString>


QuestionsDialog::QuestionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionsDialog),
    questionsMap(new QMap<QString, QStringList>()),
    questionsList(new QList<QCheckBox *>)
{
    ui->setupUi(this);

    connect(ui->okeyButton, &QPushButton::clicked, this, &QuestionsDialog::onResult);
}

void QuestionsDialog::SetQuestions(QMap<QString, QStringList> questions)
{
    questionsMap->insert(questions);


    for (const QString &question : questionsMap->keys()) {

        QCheckBox *chBox = new QCheckBox(this);
        chBox->setText(question);
        questionsList->push_back(chBox);
        ui->questionsLayout->addWidget(chBox);
    }
}

QuestionsDialog::~QuestionsDialog()
{
    delete ui;
}

void QuestionsDialog::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this,tr("Confirmation"),tr("Your choice will be lost!\nContinue?"),QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes)
    {
        onClose();
        event->accept();
    }
    else
        event->ignore();
    return;
}

void QuestionsDialog::result()
{
    for (const QCheckBox *chBox : *questionsList) {
        if (!chBox->isChecked())
            for (const QString &key : questionsMap->keys()) {
                if (key == chBox->text())
                {
                    questionsMap->remove(chBox->text());
                    break;
                }
            }
    }
    onClose();
}

void QuestionsDialog::onResult()
{
    int counter = 0;
    for (const QCheckBox *chBox : *questionsList) {
        if (chBox->isChecked()) counter++;
    }
    if (counter == 0)
    {
        if (QMessageBox::question(this,tr("Confirmation"),tr("Your didn't choose\nContinue?"),QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes)
            onClose();

        return;
    }
    result();
}

void QuestionsDialog::onClose()
{
    QStringList result;
    for (const QStringList &value : questionsMap->values())
        result.append(value);

    emit finished(result);

    for (const QCheckBox *chBox : *questionsList)
        delete chBox;

    delete questionsMap;
    delete questionsList;
}
