#pragma once

#include "WinRTMidiportWatcher.h"
#include <functional>
#include <vector>

namespace WinRT
{
    typedef std::function<void(double timestamp, std::vector<unsigned char>* message)> MessageReceivedCallbackType;

    public ref class WinRTMidi sealed
    {
    public:
        WinRTMidi();
        virtual ~WinRTMidi();

        void OpenMidiInPort(Platform::String^ id);
        void CloseMidiInPort(void);
    
        void OpenMidiOutPort(Platform::String^ id);
        void CloseMidiOutPort(void);

    internal:
        void SetMidiInCallback(const MessageReceivedCallbackType& callback) {
            mMessageReceivedCallback = callback;
        };

        void RemoveMidiInCallback() {
            mMessageReceivedCallback = nullptr;
        };

    private:
        void OnMidiInMessageReceived(Windows::Devices::Midi::MidiInPort^ sender, Windows::Devices::Midi::MidiMessageReceivedEventArgs^ args);

        Windows::Devices::Midi::MidiInPort^ mMidiInPort;
        Windows::Devices::Midi::IMidiOutPort^ mMidiOutPort;

        long long mLastMessageTime;
        bool mFirstMessage;
        MessageReceivedCallbackType mMessageReceivedCallback;
        std::vector<unsigned char> mMidiMessage;
    };
};

