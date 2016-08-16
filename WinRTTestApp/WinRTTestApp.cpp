// WinRTTestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RtMidi.h"
#include <string>
#include <iostream>

using namespace WinRT;

RtMidiIn *midiin = nullptr;
RtMidiOut *midiout = nullptr;

void printPortNames(RtMidi* port)
{
    if (port == nullptr)
    {
        return;
    }

    int nPorts = port->getPortCount();
    for (int i = 0; i < nPorts; i++) 
    {
        std::cout << i << ": " << port->getPortName(i) << std::endl;
    }
}

ref class Subscriber sealed
{
public:
    Subscriber() : eventCount(0)
    {
        // Instantiate the class that publishes the event.
        auto publisher = WinRTMidiInPortWatcher::getInstance();

        // Subscribe to the event and provide a handler function.
        publisher->mMidiPortUpdateEventHander +=
            ref new MidiPortUpdateHandler(
                this,
                &Subscriber::MyEventHandler);
    }

    void MyEventHandler(WinRTMidiPortWatcher^ mc, WinRTMidiPortUpdateType update)
    {
        switch (update)
        {
        case WinRTMidiPortUpdateType::PortAdded:
            std::cout << std::endl << "***MIDI port added***" << std::endl;
            break;

        case WinRTMidiPortUpdateType::PortRemoved:
            std::cout << std::endl << "***MIDI port removed***" << std::endl;
            break;

        case WinRTMidiPortUpdateType::EnumerationComplete:
            std::cout << std::endl << "***MIDI port enummeration complete***" << std::endl;
            break;
        }

        if (midiin != nullptr)
        {
            std::cout << "MIDI In Ports" << std::endl;
            printPortNames(midiin);
        }

        if (midiout != nullptr)
        {
            std::cout << std::endl << "MIDI Out Ports" << std::endl;
            printPortNames(midiout);
        }

        std::cout << std::endl;
    }

private:
    int eventCount;
};


int main(Platform::Array<Platform::String^>^ args)
{
    Subscriber^ subscriber = ref new Subscriber();

    // RtMidiIn constructor
    try {
        midiin = new RtMidiIn();
        std::cout << "MIDI In Ports" << std::endl;
        printPortNames(midiin);
    }
    catch (RtMidiError &error) {
        // Handle the exception here
        error.printMessage();
    }

    std::cout << std::endl;

    // RtMidiOut constructor
    try {
        midiout = new RtMidiOut();
        std::cout << "MIDI Out Ports" << std::endl;
        printPortNames(midiout);
    }
    catch (RtMidiError &error) {
        // Handle the exception here
        error.printMessage();
    }



    getchar();

    // Clean up
    delete midiin;
    delete midiout;
}
