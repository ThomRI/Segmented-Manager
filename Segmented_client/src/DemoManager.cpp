#include "DemoManager.h"

DemoManager::DemoManager()
{
    m_rootFolder = new Folder("Root", NULL, 0);
    m_XmlManager = new XmlManager("folders.xml", "Demos/", m_rootFolder);
}

void DemoManager::init()
{
    m_folderByIdList = m_XmlManager->parseXmlFile();
}

void DemoManager::addDemo(Demo *demo)
{
    Folder *folder = m_folderByIdList->value(demo->getDemoInfos()->dem_folder_id);
    if(demo->isOk()) {
        m_XmlManager->writeNewDemo(demo);

        // Adding the demo into its folder (in memory)
        folder->addDemo(demo);
    }
}

Demo *DemoManager::createDemo(QByteArray *datas, DemoInfo_s *infos)
{
    //Saving the datas into a file BY ITS ID
    QString demoFilePath = "demos/" + QString::number(infos->dem_id);
    QFile file(demoFilePath);
    if(!file.open(QIODevice::WriteOnly))
        return NULL;
    QDataStream out(&file);
    out << datas;
    file.close();

    Demo *demo = new Demo(datas, infos);

    m_XmlManager->writeNewDemo(demo); //  Writes the demo into the XML file
    m_demoByIdList->insert(infos->dem_id, demo);

    Folder *folder = m_folderByIdList->value(demo->getDemoInfos()->dem_folder_id);

    // Adding the demo into its folder
    folder->addDemo(demo);

    return demo;
}

void DemoManager::removeDemo(Demo* demo)
{
    m_XmlManager->removeDemo(demo); // Removes the demo from the XML tree.

    // Removing the demo from the folder in memory
    Folder *folder = m_folderByIdList->value(demo->getDemoInfos()->dem_folder_id);
    folder->removeDemo(demo);
    // m_folderByIdList->value(...)->removeDemo(demo);
}

void DemoManager::moveDemo(Demo *demo, Folder *newFolder)
{
    // We have to save a pointer to the old folder BEFORE using XmlManager::moveDemo!
    // Because XmlManager::moveDemo() changes the dem_folder_id of the demo in order to make the XmlManager::writeNewDemo() work.
    Folder *oldFolder = m_folderByIdList->value(demo->getDemoInfos()->dem_folder_id);

    m_XmlManager->moveDemo(demo, newFolder); // Moves the demo into the XML file ONLY! (And changing the dem_folder_id!)

    // Moving the demo into memory
    oldFolder->removeDemo(demo);
    newFolder->addDemo(demo);
}

/// ~~~ GETTERS ~~~

Demo *DemoManager::getDemo(int id)
{
    return m_demoByIdList->value(id);
}

XmlManager *DemoManager::getXmlManager()
{
    return m_XmlManager;
}

Folder *DemoManager::getFolderById(int id)
{

    return ((m_folderByIdList->size() >= id) ? m_folderByIdList->value(id) : m_rootFolder);
}

DemoManager::~DemoManager()
{
    //dtor
}
