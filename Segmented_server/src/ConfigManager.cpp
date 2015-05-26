#include "ConfigManager.h"

ConfigManager::ConfigManager(QString filepath) : m_filepath(filepath)
{
    m_file = new QFile(m_filepath);

    m_document = new QDomDocument("configDoc");
    if(m_document->setContent(m_file)) {
        m_element = new QDomElement(m_document->documentElement());
    } else {
        Out("Config file impossible to parse. Cannot define required privilege level for commands.", FATAL);
    }
}

void ConfigManager::parse()
{

}

ConfigManager::~ConfigManager()
{
    //dtor
}
