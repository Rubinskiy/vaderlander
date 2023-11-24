#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

int main()
{
    // Close the console window
    FreeConsole();

    // Read the JSON file
    ifstream f("settings.json");
    json data = json::parse(f);
    if (!f.is_open())
    {
        cout << "Bestand niet gevonden." << endl;
        return 0;
    }
    else
    {
        cout << "Instellingen lezen.." << endl;
    }

    int hour = data["alarm"]["hour"];
    int minute = data["alarm"]["minute"];
    int second = data["alarm"]["second"];

    cout << "Wekker ingesteld op: " << hour << ":" << minute << ":" << second << endl;

    while (true)
    {
        auto currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
        struct tm *localTime = localtime(&currentTime);

        if (localTime->tm_hour == hour && localTime->tm_min == minute && localTime->tm_sec == second)
        {
            cout << "Alarm actief." << endl;
            const char *mp3FilePath = "wilhelmus.mp3";
            std::string openCommand = "open \"" + std::string(mp3FilePath) + "\" type MPEGVideo alias mp3";
            mciSendStringA(openCommand.c_str(), nullptr, 0, nullptr);

            std::string playCommand = "play mp3";
            mciSendStringA(playCommand.c_str(), nullptr, 0, nullptr);

            std::string statusCommand = "status mp3 mode";
            char status[128];
            do
            {
                mciSendStringA(statusCommand.c_str(), status, 128, nullptr);
                this_thread::sleep_for(chrono::milliseconds(100));
            } while (strcmp(status, "playing") == 0);

            std::string closeCommand = "close mp3";
            mciSendStringA(closeCommand.c_str(), nullptr, 0, nullptr);
        }
        else
        {
            printf("%d:%d:%d\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
        }
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}
