#pragma once

#include <vector>
#include <memory>
#include <string>
#include <condition_variable>
#include <mutex>

namespace WinRT
{
    std::string PlatformStringToString(Platform::String^ s);

    ref class WinRTMidiPortWatcher;

    class WinRTMidiPortInfo
    {
    public:
        WinRTMidiPortInfo(Platform::String^ name, Platform::String^ id)
            : mName(name)
            , mID(id)
        {
        };

        virtual ~WinRTMidiPortInfo() {
        };

        Platform::String^ mName;
        Platform::String^ mID;
    };

    public enum class WinRTMidiPortType : int { In, Out };
    public enum class WinRTMidiPortUpdateType : int { PortAdded, PortRemoved, EnumerationComplete };
    delegate void MidiPortUpdateHandler(WinRTMidiPortWatcher^ sender, WinRTMidiPortUpdateType update);

    ref class WinRTMidiPortWatcher
    {
    public:
        unsigned int GetPortCount();
        Platform::String^ GetPortName(unsigned int portNumber);

        event MidiPortUpdateHandler^ mMidiPortUpdateEventHander;
        void OnMidiPortUpdated(WinRTMidiPortUpdateType update)
        {
            //Do something....

            // ...then fire the event:
            mMidiPortUpdateEventHander(this, update);
        }

    internal:
        WinRTMidiPortWatcher(WinRTMidiPortType type);

    private:
        void OnDeviceAdded(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformation^ args);
        void OnDeviceRemoved(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformationUpdate^ args);
        void OnDeviceUpdated(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformationUpdate^ args);
        void OnDeviceEnumerationCompleted(Windows::Devices::Enumeration::DeviceWatcher^ sender, Platform::Object^ args);

        void CheckForEnumeration();

        Windows::Devices::Enumeration::DeviceWatcher^ mPortWatcher;
        std::vector<std::shared_ptr<WinRTMidiPortInfo>> mPortInfo;
        std::mutex mEnumerationMutex;
        std::condition_variable mSleepCondition;

        bool mPortEnumerationComplete;
    };

    // Midi In Port Watcher (Singleton)
    ref class WinRTMidiInPortWatcher sealed : public WinRTMidiPortWatcher
    {
    public:
        // C++11 thread-safe Singleton pattern (VS2015 or later)
        static WinRTMidiInPortWatcher^ getInstance() {
            static WinRTMidiInPortWatcher^ instance = ref new WinRTMidiInPortWatcher();
            return instance;
        }

    private:
        WinRTMidiInPortWatcher()
            : WinRTMidiPortWatcher(WinRTMidiPortType::In)
        {
        }
    };

    // Midi Out Port Watcher (Singleton)
    ref class WinRTMidiOutPortWatcher sealed : public WinRTMidiPortWatcher
    {
    public:
        // C++11 thread-safe Singleton pattern (VS2015 or later)
        static WinRTMidiOutPortWatcher^ getInstance() {
            static WinRTMidiOutPortWatcher^ instance = ref new WinRTMidiOutPortWatcher();
            return instance;
        }

    private:
        WinRTMidiOutPortWatcher()
            : WinRTMidiPortWatcher(WinRTMidiPortType::Out)
        {}
    };
};


