#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QString>

class Language
{
public:
    Language();

    static void updateLanguage(const int &);
    static void updateLanguageByName(const QString &);

    static QStringList allLanguages;

private:
    static QHash<QString, QString> languageName;
};

#endif // LANGUAGE_H
