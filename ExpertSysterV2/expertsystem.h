#ifndef EXPERTSYSTEM_H
#define EXPERTSYSTEM_H

#include <QObject>
#include <QFile>
#include <QDebug>

class ExpertSystem : public QObject
{
    Q_OBJECT
public:
    explicit ExpertSystem(QObject *parent = nullptr);
    ~ExpertSystem();
    void search(const QString &input);
    void setCategory(const QString &category);
    void finish();
    void select(const QString &item);
    void remove(const QString &item);
    void clear();
    QStringList &getSymptomsList() const;
    QStringList &getSel_symptomsList() const;

    QList<QString> getCategories() const;

    QString getCategoryBreak() const;

private:
    void mapBuild(QMap<QString, QStringList> &to, const QStringList &from);
    void listBuild(QStringList &to, const QMap<QString, QStringList> &from);
    void listBuild(QStringList &to, const QMap<QString, QStringList> &from, const QList<QString> &categories);
    void listClearning(QStringList &patient, const QStringList &exclusions);
    void listClearningByCategory(QStringList &patient, const QList<QString> &categories);
    void mapClearning(QMap<QString, QStringList> &patient, const QStringList &exclusion);
    void checkRepeat();
    void exclusion_of_superfluous();
    void reset_selection();

signals:
    void systemError(const QString &error);
    void systemSuccess(const QStringList &missing, const QStringList &medicines);

private:
    QMap<QString, QStringList> *nutries_symptomsMap;
    QMap<QString, QStringList> *sel_nutries_symptomsMap;
    QMap<QString, QStringList> *nutries_productsMap;
    QMap<QString, QStringList> *categories_symptomsMap;
    QStringList *symptomsList;
    QStringList *sel_symptomsList;
    QString categoryBreak = "All";
};

#endif // EXPERTSYSTEM_H
