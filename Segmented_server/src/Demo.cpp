#include "../include/Demo.h"

using namespace std;

Demo::Demo(std::string demoFilePath, std::string dem_custom_name, std::string dem_desc) : m_demoFilePath(demoFilePath)
{
    m_demoInfos = new DemoInfo_s;
    m_demoFileStream = new ifstream(m_demoFilePath.c_str(), ifstream::in|ifstream::binary);
    if(isDemoFile()) {
        state = 1; // Ok
        m_demoInfos->dem_custom_name = dem_custom_name;
        m_demoInfos->dem_desc = dem_desc;
        readDemo(m_demoInfos);
    }
    else {
        cout << "Error while loading demo." << endl;
        state = -1; // Error
    }
}

Demo::Demo(std::string demoFilePath, DemoInfo_s *infos)
{
    /*
     *  Here, we call the other constructor just because infos is not supposed to contain anything else that what is needed in the other constructor.
     *  The demo will be read anyway, so datas would have been overwritten if we declared m_demoInfos as infos.
     */

    Demo(demoFilePath, infos->dem_custom_name, infos->dem_desc);
}

Demo::Demo(QByteArray *datas, std::string dem_custom_name, std::string dem_desc) : m_demoDatasArray(datas)
{
    m_demoInfos = new DemoInfo_s;
    m_demoInfos->dem_custom_name = dem_custom_name;
    m_demoInfos->dem_desc = dem_desc;

    m_demoDatasArray = datas;

    state = 1;
}

Demo::Demo(QByteArray *datas, DemoInfo_s *infos) : m_demoDatasArray(datas), m_demoInfos(infos)
{
    readFromArray(); // Read the demo info from the array of data
    state = 0; // We didn't create the stream yet !
}

Demo::Demo(DemoInfo_s *demoInfos) : m_demoInfos(demoInfos)
{

}

void Demo::readFromArray()
{
    //Creating a temp file to read the demo infos
    QString fileName = QString(m_demoInfos->dem_custom_name.c_str()) + QString(".temp");
    QFile tempFile(fileName);
    if(!tempFile.open(QIODevice::WriteOnly))
    {
        state = -1;
        return;
    }
    tempFile.write(*m_demoDatasArray);
    tempFile.close();

    //Reading the demo
    m_demoFileStream = new ifstream(QString(QString(m_demoInfos->dem_custom_name.c_str()) + QString(".temp")).toStdString().c_str(), ifstream::in|ifstream::binary);
    readDemo(m_demoInfos);

    //Removing the file
    QFile::remove(fileName);
}

bool Demo::isDemoFile()
{
    return (m_demoFileStream->good() &&
            m_demoFilePath.substr(m_demoFilePath.length() - 4, m_demoFilePath.length() - 1) == ".dem");
}

string Demo::readString(int nbrChar)
{
    char c_arr[nbrChar];
    m_demoFileStream->read(&c_arr[0], nbrChar);

    return (string)c_arr;
}

int Demo::readInt()
{
    int finalInt = 0;
    m_demoFileStream->read((char *)&finalInt, sizeof(int));

    return finalInt;
}

float Demo::readFloat()
{
    int finalFloat = 0;
    m_demoFileStream->read((char *)&finalFloat, sizeof(float));

    return finalFloat;
}

void Demo::readDemo(DemoInfo_s *demoInfos)
{
    demoInfos->dem_header = readString(8);
    demoInfos->dem_prot = readInt();
    demoInfos->net_prot = readInt();
    demoInfos->host_name = readString();
    demoInfos->client_name = readString();
    demoInfos->map_name = readString();
    demoInfos->gamedir = readString();
    demoInfos->time = readFloat();
    demoInfos->ticks = readInt();
    demoInfos->frames = readInt();
}

char *Demo::serialize()
{
    int size =  sizeof(m_demoInfos->dem_owner) + sizeof(int) +
                sizeof(m_demoInfos->dem_owner_ip) + sizeof(int) +
                sizeof(m_demoInfos->dem_custom_name) + sizeof(int) +
                sizeof(m_demoInfos->dem_desc) + sizeof(int) +
                sizeof(m_demoInfos->dem_header) + sizeof(int) +
                sizeof(int) * 6 + sizeof(float);

    /* Data structure :
        <Integer : size of dem_owner> <String : dem_owner>
        <Integer : size of dem_owner_ip> <String : <dem_owner_ip>
        ...
        <Integer : dem_folder_id>
        <Integer : dem_id>
        ...
    */

    // +sizeof(int) to put the array size in it, +1 for the \0 (end character)
    char c_serialized[size + 1];

    // Adding strings
    serializeString(m_demoInfos->dem_owner, &c_serialized[0]);
    serializeString(m_demoInfos->dem_owner_ip, &c_serialized[0]);
    serializeString(m_demoInfos->dem_custom_name, &c_serialized[0]);
    serializeString(m_demoInfos->dem_desc, &c_serialized[0]);
    serializeString(m_demoInfos->dem_header, &c_serialized[0]);
    serializeString(m_demoInfos->host_name, &c_serialized[0]);
    serializeString(m_demoInfos->client_name, &c_serialized[0]);
    serializeString(m_demoInfos->map_name, &c_serialized[0]);
    serializeString(m_demoInfos->gamedir, &c_serialized[0]);

    // Adding integers
    char intBuf[sizeof(int) + 1];
    sprintf(&intBuf[0], "%d", m_demoInfos->dem_folder_id);   strcat(&c_serialized[0], intBuf);
    sprintf(&intBuf[0], "%d", m_demoInfos->dem_id);          strcat(&c_serialized[0], intBuf);
    sprintf(&intBuf[0], "%d", m_demoInfos->dem_prot);        strcat(&c_serialized[0], intBuf);
    sprintf(&intBuf[0], "%d", m_demoInfos->net_prot);        strcat(&c_serialized[0], intBuf);
    sprintf(&intBuf[0], "%d", m_demoInfos->ticks);           strcat(&c_serialized[0], intBuf);
    sprintf(&intBuf[0], "%d", m_demoInfos->frames);          strcat(&c_serialized[0], intBuf);

    // Adding floatings (/!\ time float is written AT THE END !!! (Demo::unserialize() function manages it already))
    char floatBuf[sizeof(float) + 1];
    sprintf(&floatBuf[0], "%f", time);          strcat(&c_serialized[0], floatBuf);

    // Returning the serialized array. We can parse it at by extracting integers before strings (the sizes) ( c.f Demo::unserialize() )
    return &c_serialized[0];
}

void Demo::serializeString(string str, char *c_serialized)
{
    char sizeBuf[sizeof(str) + 1];
    sprintf(&sizeBuf[0], "%d", (int)sizeof(str.c_str()));

    strcat(&c_serialized[0], sizeBuf); // Adding the string size right before the string itself
    strcat(&c_serialized[0], str.c_str()); // Adding the string into the serialized array
}

void Demo::unserialize(char *array)
{
    int extractedSize(0), size(0); // Size keeps tracking where we are in the array
    char buf[260];

    // Unserializing the strings
    for(int seek = 0;seek < m_demoInfos->number_of_strings;seek++) {
        sscanf(&array[size], "%d", &extractedSize);
        size += sizeof(int) + extractedSize;

        memset(&buf[0], '\0', 260); // Clearing the buffer
        memcpy(buf, &array[size - extractedSize], extractedSize); // Getting the string from the array

        switch(seek) {
            case 0:
                m_demoInfos->dem_owner.assign(buf, extractedSize);
            break;

            case 1:
                m_demoInfos->dem_owner_ip.assign(buf, extractedSize);
            break;

            case 2:
                m_demoInfos->dem_custom_name.assign(buf, extractedSize);
            break;

            case 3:
                m_demoInfos->dem_desc.assign(buf, extractedSize);
            break;

            case 4:
                m_demoInfos->dem_header.assign(buf, extractedSize);
            break;

            case 5:
                m_demoInfos->host_name.assign(buf, extractedSize);
            break;

            case 6:
                m_demoInfos->client_name.assign(buf, extractedSize);
            break;

            case 7:
                m_demoInfos->map_name.assign(buf, extractedSize);
            break;

            case 8:
                m_demoInfos->gamedir.assign(buf, extractedSize);
            break;
        }
    }

    // Unserializing integers and float
    sscanf(&array[size], "%d%d%d%d%d%d%f", &m_demoInfos->dem_folder_id,
                                           &m_demoInfos->dem_id,
                                           &m_demoInfos->dem_prot,
                                           &m_demoInfos->net_prot,
                                           &m_demoInfos->ticks,
                                           &m_demoInfos->frames,
                                           &m_demoInfos->time); // Float at the end !

    // Demo infos unserialized !
}

void Demo::setOwner(string owner)
{
    m_demoInfos->dem_owner = owner;
}

void Demo::setID(int id)
{
    m_demoInfos->dem_id = id;
}

string Demo::getDemoFilePath()
{
    return m_demoFilePath;
}

DemoInfo_s* Demo::getDemoInfos()
{
    return m_demoInfos;
}

ifstream* Demo::getDemoFileStream()
{
    return m_demoFileStream;
}

QByteArray* Demo::getDatasArray()
{
    return m_demoDatasArray;
}

Demo::~Demo()
{
    m_demoFileStream->close();
}
