#include "WinRTMidi.h"

using namespace WinRT;
using namespace Windows::Devices::Midi;
using namespace Windows::Storage::Streams;

WinRTMidi::WinRTMidi()
    : mMessageReceivedCallback(nullptr)
{
}


WinRTMidi::~WinRTMidi()
{
}

//Blocks until port is open
void WinRTMidi::OpenMidiInPort(Platform::String^ id)
{
    try
    {
        mLastMessageTime = 0;
        mFirstMessage = true;
        auto task = MidiInPort::FromIdAsync(id);
        mMidiInPort = task->GetResults();
        mMidiInPort->MessageReceived += ref new Windows::Foundation::TypedEventHandler<MidiInPort ^, MidiMessageReceivedEventArgs ^>(this, &WinRTMidi::OnMidiInMessageReceived);
    }
    catch (Platform::Exception^ ex)
    {


    }

}

void WinRTMidi::CloseMidiInPort(void) 
{
    mMidiInPort = nullptr;
}

//Blocks until port is open
void WinRTMidi::OpenMidiOutPort(Platform::String^ id)
{
    try
    {
        auto task = MidiOutPort::FromIdAsync(id);
        mMidiOutPort = task->GetResults();
    }
    catch (Platform::Exception^ ex)
    {


    }
}

void WinRTMidi::CloseMidiOutPort(void)
{
    mMidiOutPort = nullptr;
}

void WinRTMidi::OnMidiInMessageReceived(MidiInPort^ sender, MidiMessageReceivedEventArgs^ args)
{
    if (mMessageReceivedCallback)
    {
        if (mFirstMessage)
        {
            mFirstMessage = false;
            mLastMessageTime = args->Message->Timestamp.Duration;
        }

        double timestamp = (args->Message->Timestamp.Duration - mLastMessageTime) * .0001;
        mLastMessageTime = args->Message->Timestamp.Duration;

        auto buffer = args->Message->RawData;
        DataReader^ reader = DataReader::FromBuffer(buffer);
        mMidiMessage.resize(buffer->Length);

        reader->ReadBytes(Platform::ArrayReference<unsigned char>(&mMidiMessage[0], buffer->Length));

        mMessageReceivedCallback((double)timestamp, &mMidiMessage);
        mMidiMessage.clear();
    }
}

