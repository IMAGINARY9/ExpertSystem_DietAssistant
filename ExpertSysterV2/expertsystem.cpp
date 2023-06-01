#include "expertsystem.h"
#include "questionsdialog.h"

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
bool ExpertSystem::listBuild(QStringList &to, const QMap<QString, QStringList> &from, const QList<QString> &categories)
{
    QStringList res;

    for (const auto &key : from.keys()) {
        for (const QString &factor : from.value(key)) {
            if (categories.contains(factor))
            {
                bool uniq = true;
                for (const QString &str : res) {
                    if (str == factor)
                        uniq = false;
                }

                if (uniq)
                    res.push_back(factor);
            }
        }
    }

    if (res.size() >= 1)
    {
        to.clear();
        to.append(res);
        return true;
    }
    else{
        emit resetCategory();
        return false;
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

void ExpertSystem::mapCategoryClearning(QMap<QString, QStringList> &patient, const QStringList &exclusion)
{
    QMap<QString, QStringList> res;
    for (const QString &el : exclusion)
        res.insert(el, patient.value(el));

    patient.clear();
    patient.insert(res);

}

void ExpertSystem::exclusion_of_superfluous()
{
    mapCategoryClearning(*sel_nutries_symptomsMap, *sel_nutries_questionsList);
    mapClearning(*sel_nutries_symptomsMap, *sel_symptomsList);
    listBuild(*symptomsList, *sel_nutries_symptomsMap);
    listClearning(*symptomsList, *sel_symptomsList);
    emit needsUpdate();

    if (sel_nutries_symptomsMap->size() == 1 && !isFinished)
        finish();
}

void ExpertSystem::reset_selection()
{
    sel_nutries_symptomsMap->clear();
    sel_nutries_symptomsMap->insert(*nutries_symptomsMap);
}

void ExpertSystem::init()
{
    clear();
    sel_nutries_symptomsMap->insert(*nutries_symptomsMap);

    isFinished = false;
    QuestionsDialog *questions = new QuestionsDialog();

    connect(questions, &QuestionsDialog::finished, questions, &QuestionsDialog::deleteLater);
    connect(questions, &QuestionsDialog::finished, this, &ExpertSystem::questionsResult);

    questions->SetQuestions(*nutries_questionsMap);

    questions->setModal(true);
    questions->show();
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

    if (listBuild(*symptomsList, *sel_nutries_symptomsMap, categories_symptomsMap->value(category)))
    {
        listClearning(*symptomsList, *sel_symptomsList);
    }
}

void ExpertSystem::finish()
{
    if (sel_nutries_symptomsMap->keys().size() >= nutries_symptomsMap->keys().size())
    {
        emit systemError("Select more symptoms!");
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
    isFinished = true;
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
QString ExpertSystem::getCategoryBreak() const
{
    return categoryBreak;
}

void ExpertSystem::questionsResult(QStringList &list)
{
    sel_nutries_questionsList->clear();
    sel_nutries_questionsList->append(list);
    exclusion_of_superfluous();
}

ExpertSystem::ExpertSystem(QObject *parent)
    : QObject{parent}
{
    nutries_symptomsMap = new QMap<QString, QStringList>();
    sel_nutries_symptomsMap = new QMap<QString, QStringList>();

    nutries_productsMap = new QMap<QString, QStringList>();
    categories_symptomsMap = new QMap<QString, QStringList>();

    nutries_questionsMap = new QMap<QString, QStringList>();
    sel_nutries_questionsList = new QStringList();

    symptomsList = new QStringList();
    sel_symptomsList = new QStringList();

    QStringList nutries_symptomsList = Read(":/database/nutries&symptoms.txt", "\n");
    QStringList nutries_productsList = Read(":/database/nutries&products.txt", "\n");
    QStringList categoriesList = Read(":/database/sortCategories.txt", "\n");
    QStringList nutries_questionsList = Read(":/database/nutries&questions.txt", "\n");

    mapBuild(*nutries_symptomsMap, nutries_symptomsList);

    mapBuild(*nutries_productsMap, nutries_productsList);
    mapBuild(*categories_symptomsMap, categoriesList);

    mapBuild(*nutries_questionsMap, nutries_questionsList);

    init();
}


ExpertSystem::~ExpertSystem()
{

    delete nutries_symptomsMap;
    delete sel_nutries_symptomsMap;

    delete nutries_productsMap;
    delete categories_symptomsMap;

    delete nutries_questionsMap;
    delete sel_nutries_questionsList;

    delete symptomsList;
    delete sel_symptomsList;
}

