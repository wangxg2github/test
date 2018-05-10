#ifndef DOMXML_H
#define DOMXML_H

#include <QString>
#include <QStringList>

#include <QFile>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QDomElement>
#include <QTextStream>

#include <QDomNode>
#include "common/comm.h"


class DomXml
{
public:
    DomXml();

    /**
     * @brief creatXML
     * @param filePath
     */
    static void creatXML(QString filePath);

    /**
     * @brief appendXML
     * @param filePath
     * @param list
     */
    static void appendXML(QString filePath, QStringList list);

    /**
     * @brief writeUserToXML
     * @param doc
     * @param parsent
     * @param list
     */
    static void writeUserToXML(QDomDocument& doc, QDomElement& parsent, QStringList& list);

    /**
     * @brief readDataByUserNameFromXML
     * @param filePath
     * @param list
     * @param user
     */
    static void readDataByUserNameFromXML(QString filePath, QStringList& list, QString user = "admin");
};

#endif // DOMXML_H
