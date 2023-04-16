#pragma once

#include <string>
#include <fstream>


class LogManager;
class AssetManager;

class SerializationManager
{
private:

    LogManager* log_;
    AssetManager* assets_;

    std::string path_ {"D:/Prog/game_project/game/game.sav"};
    std::ofstream ofs;  // Output stream
    std::ifstream ifs;  // Input stream

public:

    SerializationManager() {}
    SerializationManager(LogManager* log, AssetManager* assets);

    bool startUp();
    bool shutDown();

    bool open_out();
    bool close_out();
    bool open_in();
    bool close_in();

    template<typename T> inline void write(const T& data);
    template<typename T> inline void read(T& data);

};

/*
    Header implementation
*/

template<typename T>
inline void SerializationManager::write(const T& data) { ofs << data << " "; }

template<typename T>
inline void SerializationManager::read(T& data) { ifs >> data; }