#include "XmlManager.h"

XmlManager::XmlManager(QString XmlPath, QString demosPath, Folder *rootFolder) : m_XmlPath(XmlPath), m_demosPath(demosPath), m_rootFolder(rootFolder)
{
    m_foldersSaveFile = new QFile(m_XmlPath);

    m_foldersDocXml = new QDomDocument("foldersDocXml");
    if(m_foldersDocXml->setContent(m_foldersSaveFile)) {
        m_foldersElementXml = new QDomElement(m_foldersDocXml->documentElement());
    } else {
        Out::failure("Error while parsing the XML file. The arborescence cannot be rebuild.", FATAL);
    }
}

QMap<int, Folder *> *XmlManager::parseXmlFile()
{
    out("Parsing XML...");
    QMap<int, Folder *> *folderByIdList = new QMap<int, Folder *>;
    folderByIdList->insert(m_rootFolder->getID(), m_rootFolder);

    QDomNode node = m_foldersElementXml->firstChild();
    while(!node.isNull()) {
        if(node.nodeName() == "folder") {
            parseFolderXml(&node, m_rootFolder, folderByIdList);
        }

        node = node.nextSibling();
    }

    m_folderByIdList = folderByIdList; // Updating the folderByIdList ENTIRELY

    Out::success("XML file parsed successfully !");
    return folderByIdList;

    // XML File Structure :
    /*  <folders>
            <folder name="blabla">
                <demo name="blabla" id="3" ip="82.33.23.422" owner="ThomRI98">
                    // Demo description
                </demo>

                <folder name="sub_blabla">
                    ...
                </folder>
            </folder>

            <folder name="blabla2>
            ...
        </folders>
    */
}

void XmlManager::parseFolderXml(QDomNode *folderNode, Folder *parent, QMap<int, Folder *> *folderByIdList)
{
    int folderID = m_folderByIdList->size();
    // If there is 1 element in foldersByIdList, the index of the next one is 1 (as the index of the first is 0).
    // So the index of the next element is always folderByIdList->size() (as we don't want to overwrite anything!)

    Folder *folder = parent->createFolder(folderNode->toElement().attribute("name"), folderID);
    m_nodeByFolderList->insert(folder, folderNode); // Updating the nodeByFolder with the current new folder in.

    folderNode = new QDomNode(folderNode->firstChild()); // This node was the folder node so we get the first_thing_inside_the_folder's node

    while(!folderNode->isNull()) {
        // It's a demo
        if(folderNode->nodeName() == "demo") {
            // We create the demo
            DemoInfo_s *infos = new DemoInfo_s;

            QDomElement element = folderNode->toElement();

            infos->dem_owner = element.attribute("owner").toStdString();
            infos->dem_owner_ip = element.attribute("ip").toStdString();
            infos->dem_id = element.attribute("id").toInt();
            infos->dem_folder_id = element.attribute("folder_id").toInt();
            infos->dem_custom_name = element.attribute("custom_name").toStdString();
            infos->dem_desc = element.text().toStdString();

            //Demo(filepath, infos);
            Demo *demo = new Demo(QString(QString(m_demosPath) + QString(element.attribute("id"))).toStdString(), infos);
            folder->addDemo(demo);

            //delete(*element); // Deletes the POINTER
        }

        else if(folderNode->nodeName() == "folder") {
            parseFolderXml(folderNode, folder, m_folderByIdList);
        }

        folderNode = new QDomNode(folderNode->nextSibling());
    }

    folderByIdList->insert(folderID, folder); // Inserting the new parsed folder in the list
}

void XmlManager::writeNewDemo(Demo *demo)
{
    out("Writing new demo...");
    Folder *folder = m_folderByIdList->value(demo->getDemoInfos()->dem_folder_id);
    QDomNode *folderNode = m_nodeByFolderList->value(folder);

    // Creating the new node
    QDomNode *node = new QDomNode(folderNode->appendChild(m_foldersDocXml->createElement("demo")));

    // Adding attributes
    node->toElement().setAttribute("owner", QString(demo->getDemoInfos()->dem_owner.c_str()));
    node->toElement().setAttribute("ip", QString(demo->getDemoInfos()->dem_owner_ip.c_str()));
    node->toElement().setAttribute("custom_name", QString(demo->getDemoInfos()->dem_custom_name.c_str()));
    node->toElement().setAttribute("folder_id", demo->getDemoInfos()->dem_folder_id);
    node->toElement().setAttribute("id", demo->getDemoInfos()->dem_id);

    //Adding description
    QDomText desc = m_foldersDocXml->createTextNode(QString(demo->getDemoInfos()->dem_desc.c_str()));
    node->appendChild(desc);

    save(); // Saves the xml file

    Out::success("Done writing new demo.");
}

void XmlManager::editDemo(Demo *demo, DemoInfo_s *newInfos)
{
    QDomNode *folderNode = m_nodeByFolderList->value(m_folderByIdList->value(demo->getDemoInfos()->dem_folder_id));

    // Getting the demo node from the demo pointer
    QDomNode *node = getDemoNode(demo);

    // We CREATE a new node
    QDomElement *newElement = new QDomElement(m_foldersDocXml->createElement("demo"));
    QDomText *newNode = new QDomText(m_foldersDocXml->createTextNode(QString(newInfos->dem_desc.c_str())));
    newElement->appendChild(*newNode); // Writing description

    // Creating new attributes
    newElement->setAttribute("owner", QString(newInfos->dem_owner.c_str()));
    newElement->setAttribute("ip", QString(newInfos->dem_owner_ip.c_str()));
    newElement->setAttribute("custom_name", QString(newInfos->dem_custom_name.c_str()));
    newElement->setAttribute("folder_id", newInfos->dem_folder_id);
    newElement->setAttribute("id", newInfos->dem_id);

    // Overwriting the old one
    folderNode->replaceChild(*newNode, *node);

    // Saving the Xml file
    save();
}


/*!
 * \brief Removes the demo from the XML tree
 */
void XmlManager::removeDemo(Demo *demo)
{
    Folder *folder = m_folderByIdList->value(demo->getDemoInfos()->dem_folder_id);
    QDomNode *folderNode = m_nodeByFolderList->value(folder);

    QDomNode *node = getDemoNode(demo);

    // Removing the demo from the folder in Xml file
    folderNode->removeChild(*node);

    save(); // Saves the XML file
}

QDomNode *XmlManager::getDemoNode(Demo *demo)
{
    QDomNode *folderNode = m_nodeByFolderList->value(m_folderByIdList->value(demo->getDemoInfos()->dem_folder_id));

    // Parsing the folder to find the demo
    QDomNode *node = new QDomNode(folderNode->firstChild());
    while(!node->isNull()) {
        if(node->nodeName() == "demo" \
        && QString(node->toElement().attribute("id")).toInt() == demo->getDemoInfos()->dem_id) { // If we found the right demo (with the right id)
            return node;
        }

        node = new QDomNode(node->nextSibling()); // If we didn't execute the "break()" instruction, we get to this. (We change for the next demo node).
    }
}

void XmlManager::writeNewFolder(Folder *folder)
{
    QDomNode *parentNode = m_nodeByFolderList->value(folder->getParent());
    QDomElement *folderElement = new QDomElement(m_foldersDocXml->createElement("demo"));

    folderElement->setAttribute("name", folder->getName());

    parentNode->appendChild(*folderElement); // Possible as QDomElement is heritated from QDomNode

    save(); // Saves the XML file
}

void XmlManager::removeFolder(Folder *folder) // Recursive
{
    QDomNode *folderNode = m_nodeByFolderList->value(folder);
    QDomNode *current = new QDomNode(folderNode->firstChild());

    while(!current->isNull()) {
        if(current->nodeName() == "folder") {
            removeFolder(m_folderByIdList->value(current->toElement().attribute("id").toInt())); // Deleting the folder content if we find a folder (recursive)
        }

        QDomNode *next = new QDomNode(current->nextSibling());
        folderNode->removeChild(*current); // Removing the demo node or the folder node (the folder that is being removed is now empty)
        current = next;
    }

    save(); // Saves the XML file
}

void XmlManager::moveDemo(Demo *demo, Folder *newFolder)
{
    removeDemo(demo); // Removes the demo from the xml tree

    demo->getDemoInfos()->dem_folder_id = newFolder->getID(); // Changing the folder ID for the writeNewDemo() function
    writeNewDemo(demo); // Puts the demo in the XML tree, according to its dem_folder_id value.

    save(); // Saves the XML file
}

/*!
 * \brief Saves the XML file. Overwrites it.
 */
void XmlManager::save()
{
    if(m_foldersSaveFile->open(QIODevice::ReadOnly)) {
        // Overwriting
            m_foldersSaveFile->resize(0);
            m_foldersSaveFile->seek(0);

        QTextStream stream(m_foldersSaveFile);
        stream << m_foldersDocXml->toByteArray();

        m_foldersSaveFile->close();
    }
}

/*!
 * \return QFile *xmlFile
 */
QFile *XmlManager::getXmlFile()
{
    return m_foldersSaveFile;
}

QMap<int, Folder *> *XmlManager::getFolderByIdList()
{
    return m_folderByIdList;
}

void XmlManager::out(QString msg)
{
    std::cout << "\x1b[34m[XML Manager]\x1b[0m " << msg.toStdString() << std::endl;
}

XmlManager::~XmlManager()
{
    m_foldersSaveFile->close();
}
