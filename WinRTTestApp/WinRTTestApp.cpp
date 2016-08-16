// WinRTTestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RtMidi.h"
#include <string>
#include <iostream>

void printPortNames(RtMidi* port)
{
    int nPorts = port->getPortCount();
    for (int i = 0; i < nPorts; i++)
    {
        std::string name = port->getPortName(i);
        std::cout << name << std::endl;
    }
}


int main(Platform::Array<Platform::String^>^ args)
{
    RtMidiIn *midiin = 0;
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

    RtMidiOut *midiout = 0;
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
