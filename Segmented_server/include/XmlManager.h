/*
    /!\ This class isn't universal, and has been written for this program ONLY.
    Plus, it is NOT a XML parser that can be used by another system than the demo one. Indeed, even the config cannot be parsed using this class.

    It allows to :
        - Write a new demo
        - Edit a demo
        - Remove a demo

        - Parse the save file and create the program's demo (and folder) arborescence
*/

#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include <QFile>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QTextStream>
#include "Folder.h"
#include <QDebug>
#include "Out.h"

class XmlManager : public Out
{
    public:
        XmlManager(QString XmlPath, QString demosPath, Folder *m_rootFolder);

        QMap<int, Folder*> *parseXmlFile();
        void parseFolderXml(QDomNode *folderNode, Folder *parent, QMap<int, Folder *> *folderByIdList);

        void writeNewDemo(Demo *demo);
        void writeNewFolder(Folder *folder);

        void editDemo(Demo *demo, DemoInfo_s *newInfos);
        void moveDemo(Demo *demo, Folder *newFolder);
        void removeDemo(Demo *demo);

        void removeFolder(Folder *folder);


        // Getters :
        QFile *getXmlFile();
        QMap<int, Folder *> *getFolderByIdList();

        virtual ~XmlManager();

    protected:
        Folder *getFolderById(int id);
        QDomNode *getDemoNode(Demo *demo);
        void save();

        void out(QString msg);
    private:
        QString m_XmlPath;
        QString m_demosPath;
        Folder *m_rootFolder;


        QFile *m_foldersSaveFile;
        QDomDocument *m_foldersDocXml;
        QDomElement *m_foldersElementXml;

        QMap<Folder *, QDomNode *> *m_nodeByFolderList;
        QMap<int, Folder *> *m_folderByIdList;
};

#endif // XMLMANAGER_H
