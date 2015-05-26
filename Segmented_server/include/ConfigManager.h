#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H


class ConfigManager : public Out
{
    public:
        ConfigManager(QString filepath);
        virtual ~ConfigManager();

        void parse();
    protected:
    private:
        QString m_filepath; // Config file path

        QFile *m_file;
        QDomDocument *m_document;
        QDomElement *m_element;

};

#endif // CONFIGMANAGER_H
