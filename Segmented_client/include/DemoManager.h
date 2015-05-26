#ifndef DEMOMANAGER_H
#define DEMOMANAGER_H

#include "Demo.h"
#include "Folder.h"

#include <QMap>
#include <QString>
#include <QDataStream>
#include "XmlManager.h"

class DemoManager
{
    public:
        DemoManager();

        void init(); // Initializes the manager (loads demos from files with meta datas)

        void addDemo(Demo *demo);

        /// Only dem_owner, dem_owner_ip, dem_custom_name, and dem_desc have to be written into the DemoInfos_s
        Demo* createDemo(QByteArray *datas, DemoInfo_s *infos);
        void removeDemo(Demo *demo);
        void moveDemo(Demo *demo, Folder *newFolder);

        void createFolder(QString name);

        Demo *getDemo(int id);
        XmlManager *getXmlManager();
        Folder *getFolderById(int id);

        virtual ~DemoManager();
    protected:

    private:
        Folder *m_rootFolder;

        QMap <int, Demo *> *m_demoByIdList;
        QMap<int, Folder *> *m_folderByIdList;
        XmlManager *m_XmlManager;
};

#endif // DEMOMANAGER_H
