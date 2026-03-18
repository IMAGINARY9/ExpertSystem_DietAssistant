#ifndef QUESTIONSDIALOG_H
#define QUESTIONSDIALOG_H

#include <QCloseEvent>
#include <QCheckBox>
#include <QDialog>
#include <QList>
#include <QMap>

namespace Ui {
class QuestionsDialog;
}

class QuestionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionsDialog(QWidget *parent = nullptr);
    void SetQuestions(QMap<QString, QStringList> questions);
    ~QuestionsDialog();

    void closeEvent(QCloseEvent *event);
signals:
    void finished(QStringList &list);

private:
    Ui::QuestionsDialog *ui;
    QMap<QString, QStringList> *questionsMap;
    QList<QCheckBox *> *questionsList;
    void result();
    void onResult();
    void onClose();
};

#endif // QUESTIONSDIALOG_H
