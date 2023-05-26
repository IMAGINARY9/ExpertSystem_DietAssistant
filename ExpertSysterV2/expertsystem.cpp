#include "expertsystem.h"

QStringList Read(QString Filename, QString splitSymbol)
{
    QFile mFile(Filename);
    QStringList result;

    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "couldn't open file for reading";
    }
    else
    {
        QTextStream in(&mFile);
        QString mText = in.readAll();
        result = mText.split(splitSymbol);

        mFile.flush();
        mFile.close();
    }

    return result;
}

void ExpertSystem::mapBuild(QMap<QString, QStringList> &to, const QStringList &from)
{
    for (const QString &str : from) {
        QString strLeft = str.left(str.indexOf(": "));
        QStringList strRight = (str.right(str.length() - str.indexOf(": ") - 2)).split(", ");
        to.insert(strLeft, strRight);
    }
}

void ExpertSystem::listBuild(QStringList &to, const QMap<QString, QStringList> &from)
{
    to.clear();
    for (const auto &key : from.keys()) {
        for (const QString &factor : from.value(key)) {

            bool uniq = true;
            for (const QString &str : to) {
                if (str == factor)
                    uniq = false;
            }

            if (uniq)
                to.push_back(factor);

        }
    }
}
void ExpertSystem::listBuild(QStringList &to, const QMap<QString, QStringList> &from, const QList<QString> &categories)
{
    to.clear();
    for (const auto &key : from.keys()) {
        for (const QString &factor : from.value(key)) {
            if (categories.contains(factor))
            {
                bool uniq = true;
                for (const QString &str : to) {
                    if (str == factor)
                        uniq = false;
                }

                if (uniq)
                    to.push_back(factor);
            }
        }
    }
}

void ExpertSystem::listClearningByCategory(QStringList &patient, const QList<QString> &categories)
{
    for (const QString &sympt : categories) {
        bool toDelete = true;
        for (const QString &item : patient) {
            if(sympt == item)
            {
                toDelete = false;
                break;
            }
        }

    }
}

void ExpertSystem::listClearning(QStringList &patient, const QStringList &exclusions)
{
    for (const QString &excl : exclusions) {
        for (const QString &item : patient) {
            if(excl == item)
            {
                patient.removeAll(item);
                break;
            }
        }
    }
}

void ExpertSystem::mapClearning(QMap<QString, QStringList> &patient, const QStringList &exclusion)
{
    for (const QStringList &value : patient.values()) {
        int counter = 0;
        for (const QString &el : value) {
            if (exclusion.contains(el))
                counter++;
        }
        if (counter != exclusion.size()) {
            QString toDelete = patient.key(value);
            patient.remove(toDelete);
        }
    }
}

void ExpertSystem::exclusion_of_superfluous()
{
    mapClearning(*sel_nutries_symptomsMap, *sel_symptomsList);
    listBuild(*symptomsList, *sel_nutries_symptomsMap);
    listClearning(*symptomsList, *sel_symptomsList);

    if (sel_nutries_symptomsMap->size() == 1)
        finish();
}

void ExpertSystem::reset_selection()
{
    sel_nutries_symptomsMap->clear();
    sel_nutries_symptomsMap->insert(*nutries_symptomsMap);
}

QString ExpertSystem::getCategoryBreak() const
{
    return categoryBreak;
}

bool compareByMatches(const QString& str1, const QString& str2, const QString& input)
{
    return str1.count(input) > str2.count(input);
}

void ExpertSystem::search(const QString &input)
{
    if (!input.isEmpty())
        std::sort(symptomsList->begin(), symptomsList->end(),
                  [input](const QString &str1, const QString &str2) {
                      return compareByMatches (str1, str2, input);
        });
}

void ExpertSystem::setCategory(const QString &category)
{
    if (category == categoryBreak)
    {
        exclusion_of_superfluous();
        return;
    }

    listBuild(*symptomsList, *sel_nutries_symptomsMap, categories_symptomsMap->value(category));
    listClearning(*symptomsList, *sel_symptomsList);
}

void ExpertSystem::finish()
{
    if (sel_symptomsList->size() <= 0)
    {
        emit systemError("Select symptoms!");
        return;
    }
    QStringList medicines;
    for (const QString &symptomItem : sel_nutries_symptomsMap->keys()) {
        for (const QString &product : nutries_productsMap->value(symptomItem)) {

            bool isAdded = false;
            for (const QString &added : medicines) {
                if (added == product)
                    isAdded = true;
            }
            if (!isAdded)
                medicines.push_back(product);
        }
    }
    emit systemSuccess(sel_nutries_symptomsMap->keys(), medicines);
}

void ExpertSystem::select(const QString &item)
{
    sel_symptomsList->push_front(item);
    exclusion_of_superfluous();
}
void ExpertSystem::remove(const QString &item)
{
    sel_symptomsList->removeAll(item);
    reset_selection();
    exclusion_of_superfluous();
}

void ExpertSystem::clear()
{
    sel_symptomsList->clear();
    reset_selection();
    exclusion_of_superfluous();
}

QStringList &ExpertSystem::getSymptomsList() const
{
    return *symptomsList;
}

QStringList &ExpertSystem::getSel_symptomsList() const
{
    return *sel_symptomsList;
}

QList<QString> ExpertSystem::getCategories() const
{
    return categories_symptomsMap->keys();
}

ExpertSystem::ExpertSystem(QObject *parent)
    : QObject{parent}
{
    nutries_symptomsMap = new QMap<QString, QStringList>();
    sel_nutries_symptomsMap = new QMap<QString, QStringList>();
    nutries_productsMap = new QMap<QString, QStringList>();
    categories_symptomsMap = new QMap<QString, QStringList>();

    symptomsList = new QStringList();
    sel_symptomsList = new QStringList();

    QStringList nutries_symptomsList = Read(":/database/nutries&symptoms.txt", "\n");
    QStringList nutries_productsList = Read(":/database/nutries&products.txt", "\n");
    QStringList categoriesList = Read(":/database/categories.txt", "\n");

    mapBuild(*nutries_symptomsMap, nutries_symptomsList);
    sel_nutries_symptomsMap->insert(*nutries_symptomsMap);
    mapBuild(*nutries_productsMap, nutries_productsList);
    mapBuild(*categories_symptomsMap, categoriesList);

    listBuild(*symptomsList, *nutries_symptomsMap);



//    qDebug() << "\n\nsymptomsList\n";
//    for (const auto &el : *symptomsList) {
//        qDebug() << el;
//    }

//    qDebug() << "\n\nnutries_symptomsMap\n";
//    for (const auto &el : *nutries_symptomsMap) {
//        qDebug() << el;
//    }

//    qDebug() << "\n\nsel_nutries_symptomsMap\n";
//    for (const auto &el : *sel_nutries_symptomsMap) {
//        qDebug() << el;
//    }

//    qDebug() << "\n\nnutries_productsMap\n";
//    for (const auto &el : *nutries_productsMap) {
//        qDebug() << el;
//    }

//     qDebug() << "\n\ncategories_symptomsMap\n";
//     for (const auto &el : *categories_symptomsMap) {
//         qDebug() << el;
//     }

}


ExpertSystem::~ExpertSystem()
{
    delete nutries_symptomsMap;
    delete sel_nutries_symptomsMap;
    delete nutries_productsMap;
    delete categories_symptomsMap;
    delete symptomsList;
    delete sel_symptomsList;
}

