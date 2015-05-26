#ifndef FOLDER_H
#define FOLDER_H

#include <QString>
#include "Demo.h"
#include <map>

class Folder
{
    public:
        Folder(QString name, Folder *parent, int id);

        Folder *createFolder(QString name, int id);
        void addDemo(Demo *demo);

        void removeFolder(int ID);
        void removeFolder(Folder *folder);
        void removeDemo(Demo *demo);
        void removeDemo(int id);

        std::map<int, Demo *> *getDemos();
        std::map<int, Folder *> *getSubfolders();

        Demo *getDemo(int id);

        Folder *getSubFolder(int id); // Get a sub folder by its id

        Folder *getParent();
        QString getName();
        int getID();

        virtual ~Folder();
    protected:
    private:
        std::map<int, Folder *> *m_subfolders;
        std::map<int, Demo *> *m_demos;

        Folder *m_parent; // A pointer to the parent of this folder. Is used to get back to the previous folder

        QString *m_name;

        int m_id;
};

#endif // FOLDER_H
