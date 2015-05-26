#ifndef DEMO_H
#define DEMO_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <QByteArray>
#include <QString>
#include <QFile>

/*
    /!\ A demo is necessarily stored in "./demos/<dem_id>"
*/

class DemoInfo_s
{
	public:
        std::string    dem_owner;          /// Demo owner
        std::string    dem_owner_ip;       /// IP of the owner
        std::string    dem_custom_name;    /// Demo custom name
        std::string    dem_desc;           /// Demo desctription
        std::string    dem_header;         // Demo header (in demo file)
        int            dem_folder_id;      /// Demo folder id
        int            dem_id;             /// Demo ID
        int            dem_prot;           // Demo protocol version (in demo file)
        int            net_prot;           // Network protocol version (in demo file)
        std::string    host_name;          // Server name (in demo file)
        std::string    client_name;        // Client name (in demo file)
        std::string    map_name;           // Map name (in demo file)
        std::string    gamedir;            // Root game directory (in demo file)
        float          time;               // Playback time (s) (in demo file)
        int            ticks;              // Number of ticks (in demo file)
        int            frames;             // Number of frames (in demo file)

        int            number_of_strings = 9;

        void write() {
            std::cout << "Demo type:\t\t\t"               << dem_header   << std::endl <<
                         "Demo protocol version:\t\t"     << dem_prot     << std::endl <<
                         "Network protocol version:\t"    << net_prot     << std::endl <<
                         "Server name:\t\t\t"             << host_name    << std::endl <<
                         "Client name:\t\t\t"             << client_name  << std::endl <<
                         "Map name:\t\t\t"                << map_name     << std::endl <<
                         "Game directory:\t\t\t"          << gamedir      << std::endl <<
                         "Time in seconds:\t\t"           << time         << std::endl <<
                         "Time in ticks: \t\t\t"          << ticks        << std::endl <<
                         "Number of game frames:\t\t"     << frames       << std::endl;
        }
};


class Demo
{
    public:
        Demo(std::string demoFilePath, std::string dem_custom_name = "Untitled", std::string dem_desc = "No description.");
        Demo(std::string demoFilePath, DemoInfo_s *infos);
        Demo(QByteArray *datas, std::string dem_custom_name = "Untitled", std::string dem_desc = "No description.");
        Demo(QByteArray *datas, DemoInfo_s *infos);
        Demo(DemoInfo_s *demoInfos);

        DemoInfo_s* getDemoInfos();
        std::ifstream* getDemoFileStream();
        QByteArray* getDatasArray();

        bool isOk() {
            return state == 1;
        }

        void setOwner(std::string owner);
        void setID(int id);
        std::string getDemoFilePath();

        char *serialize();

        virtual ~Demo();
    protected:
        bool isDemoFile();
        void readDemo(DemoInfo_s *demoInfos);

        std::string readString(int nbrChar = 260);
        int readInt();
        float readFloat();

        void readFromArray();

        // char *serialize() :
            // Adds the size of the string and right after it, the string itself into c_serialized
            void serializeString(std::string str, char *c_serialized);
        void unserialize(char *array);
    private:
        std::string      m_demoFilePath;
        std::ifstream   *m_demoFileStream;
        DemoInfo_s      *m_demoInfos;

        QByteArray *m_demoDatasArray;

        /*
            ## STATE ##
            0 : Non tested yet.
            1 : OK
           -1 : Error
        */
        int state = 0;

};

#endif // DEMO_H
