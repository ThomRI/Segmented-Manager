#include "Folder.h"

Folder::Folder(QString name, Folder *parent, int id) : m_name(&name), m_parent(parent), m_id(id)
{
    //ctor
}

Folder *Folder::createFolder(QString name, int id)
{
    Folder *folder = new Folder(name, this, id);
    m_subfolders->insert(std::pair<int, Folder *>(id, this));
    return folder;
}

void Folder::addDemo(Demo *demo)
{
    m_demos->insert(std::pair<int, Demo *>(demo->getDemoInfos()->dem_id, demo));
}

void Folder::removeFolder(Folder *folder)
{
    removeFolder(folder->getID());
}

void Folder::removeFolder(int id)
{
    m_subfolders->erase(id);
}

void Folder::removeDemo(Demo *demo)
{
    removeDemo(demo->getDemoInfos()->dem_id);
}

void Folder::removeDemo(int id)
{
    m_demos->erase(id);
}

std::map<int, Demo *> *Folder::getDemos()
{
    return m_demos;
}

std::map<int, Folder *> *Folder::getSubfolders()
{
    return m_subfolders;
}

Demo *Folder::getDemo(int id)
{
    return m_demos->at(id);
}

Folder *Folder::getSubFolder(int id)
{
    return m_subfolders->at(id);
}

// ## GETTERS ##

QString Folder::getName()
{
    return *m_name;
}

int Folder::getID()
{
    return m_id;
}

Folder *Folder::getParent()
{
    return m_parent;
}

Folder::~Folder()
{
    //dtor
}
