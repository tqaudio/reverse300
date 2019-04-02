#include "../include/processor.h"

namespace Reverse300 {
Processor::Processor()
    : mDry(DEFAULT_DRY_NORMALIZED), mWet(DEFAULT_WET_NORMALIZED),
      mNoteChannel(DEFAULT_MIDI_CHANNEL_NORMALIZED),
      mNoteNumber(DEFAULT_MIDI_NOTE_NORMALIZED) {
  setControllerClass(ControllerID);
}

tresult PLUGIN_API Processor::initialize(FUnknown *context) {
  tresult result = AudioEffect::initialize(context);

  if (result != kResultTrue) {
    return kResultFalse;
  }

  addEventInput(STR16("Event Input"), 1);
  addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
  addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);

  return kResultTrue;
}

tresult PLUGIN_API Processor::setBusArrangements(SpeakerArrangement *inputs,
                                                 int32 numIns,
                                                 SpeakerArrangement *outputs,
                                                 int32 numOuts) {
  if (numIns == 1 && numOuts == 1 && inputs[0] == outputs[0]) {
    return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
  }

  return kResultFalse;
}

tresult PLUGIN_API Processor::setupProcessing(ProcessSetup &newSetup) {
  return AudioEffect::setupProcessing(newSetup);
}

tresult PLUGIN_API Processor::setActive(TBool state) {
  SpeakerArrangement arrangement;
  if (getBusArrangement(kOutput, 0, arrangement) != kResultTrue) {
    return kResultFalse;
  }

  int channelCount = SpeakerArr::getChannelCount(arrangement);

  if (channelCount != 2) {
    return kResultFalse;
  }
  if (state) {
    for (int channel = 0; channel < channelCount; channel++) {
      mBuffer[channel] = new Buffer(processSetup.sampleRate, MAX_DURATION);
    }
    mWets = new AutomationParameter[processSetup.maxSamplesPerBlock];
    mDries = new AutomationParameter[processSetup.maxSamplesPerBlock];
    mNoteOns = new AutomationParameter[processSetup.maxSamplesPerBlock];
    mNoteOffs = new AutomationParameter[processSetup.maxSamplesPerBlock];
  } else {
    for (int channel = 0; channel < channelCount; channel++) {
      delete mBuffer[channel];
    }
    delete mWets;
    delete mDries;
    delete mNoteOns;
    delete mNoteOffs;
  }

  return AudioEffect::setActive(state);
}

tresult PLUGIN_API Processor::process(ProcessData &data) {
  if (data.inputParameterChanges) {
    int32 numParamsChanged = data.inputParameterChanges->getParameterCount();

    for (int32 index = 0; index < numParamsChanged; index++) {
      IParamValueQueue *paramQueue =
          data.inputParameterChanges->getParameterData(index);

      if (paramQueue == nullptr) {
        continue;
      }

      int32 numPoints = paramQueue->getPointCount();
      int32 sampleOffset;
      ParamValue value;

      if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) !=
          kResultTrue) {
        continue;
      }
      if (sampleOffset < 0) {
        sampleOffset = 0;
      }
      if (sampleOffset > data.numSamples - 1) {
        sampleOffset = data.numSamples - 1;
      }
      switch (paramQueue->getParameterId()) {
      case Parameters::kBypassId:
        mBypass = (value > 0.5f);
        break;
      case Parameters::kDryId:
        mDry = value;
        mDries[sampleOffset].value = value;
        mDries[sampleOffset].hasChanged = true;
        break;
      case Parameters::kWetId:
        mWet = value;
        mWets[sampleOffset].value = value;
        mWets[sampleOffset].hasChanged = true;
        break;
      case Parameters::kNoteChannelId:
        mNoteChannel = value;
        break;
      case Parameters::kNoteNumberId:
        mNoteNumber = value;
        break;
      }
    }
  }
  if (data.inputEvents) {
    int eventCount = data.inputEvents->getEventCount();

    for (int i = 0; i < eventCount; i++) {
      Event incomingEvent;

      if (data.inputEvents->getEvent(i, incomingEvent) != kResultOk) {
        continue;
      }

      int sampleOffset = incomingEvent.sampleOffset;
      int noteNumber = round(mNoteNumber * 128.0f);
      int noteChannel = round(mNoteChannel * 16.0f);

      if (sampleOffset < 0) {
        sampleOffset = 0;
      }
      if (sampleOffset > data.numSamples - 1) {
        sampleOffset = data.numSamples - 1;
      }
      switch (incomingEvent.type) {
      case Event::kNoteOnEvent:
        if (noteNumber <= 127 && noteNumber != incomingEvent.noteOn.pitch ||
            noteChannel > 0 &&
                (noteChannel - 1) != incomingEvent.noteOn.channel) {
          break;
        }

        mNoteOns[sampleOffset].hasChanged = true;
        break;
      case Event::kNoteOffEvent:
        if (noteNumber <= 127 && noteNumber != incomingEvent.noteOn.pitch ||
            noteChannel > 0 &&
                (noteChannel - 1) != incomingEvent.noteOn.channel) {
          break;
        }

        mNoteOffs[sampleOffset].hasChanged = true;
        break;
      }
    }
  }
  if (data.numInputs == 0 || data.numOutputs == 0) {
    return kResultOk;
  }
  if (data.numSamples > 0) {
    SpeakerArrangement arrangement;
    getBusArrangement(kOutput, 0, arrangement);
    int32 channelCount = SpeakerArr::getChannelCount(arrangement);
    float *inputChannel = nullptr;
    float *outputChannel = nullptr;

    for (int32 sample = 0; sample < data.numSamples; sample++) {
      for (int32 channel = 0; channel < channelCount; channel++) {
        inputChannel = data.inputs[0].channelBuffers32[channel];
        outputChannel = data.outputs[0].channelBuffers32[channel];

        if (mNoteOns[sample].hasChanged) {
          mBuffer[channel]->rec();
          if (channel == channelCount - 1) {
            mNoteOns[sample].hasChanged = false;
          }
        }
        if (mNoteOffs[sample].hasChanged) {
          mBuffer[channel]->stop();
          if (channel == channelCount - 1) {
            mNoteOffs[sample].hasChanged = false;
          }
        }
        if (mDries[sample].hasChanged) {
          mDry = mDries[sample].value;
          if (channel == channelCount - 1) {
            mDries[sample].hasChanged = false;
          }
        }
        if (mWets[sample].hasChanged) {
          mWet = mWets[sample].value;
          if (channel == channelCount - 1) {
            mWets[sample].hasChanged = false;
          }
        }

        mBuffer[channel]->write(inputChannel[sample]);

        if (mBuffer[channel]->isPlaying()) {
          outputChannel[sample] = mBuffer[channel]->read() * mWet;
        } else {
          outputChannel[sample] = mBuffer[channel]->read() * mDry;
        }
      }
    }
  }

  return kResultOk;
}

tresult PLUGIN_API Processor::setState(IBStream *state) {
  if (!state) {
    return kResultFalse;
  }

  IBStreamer streamer(state, kLittleEndian);
  int32 savedBypass = 0;
  float savedNoteChannel = 0.0f;
  float savedNoteNumber = 0.0f;
  float savedDry = 0.0f;
  float savedWet = 0.0f;

  if (!streamer.readInt32(savedBypass)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedNoteChannel)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedNoteNumber)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedDry)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedWet)) {
    return kResultFalse;
  }

  mBypass = savedBypass > 0;
  mNoteChannel = savedNoteChannel;
  mNoteNumber = savedNoteNumber;
  mDry = savedDry;
  mWet = savedWet;

  return kResultOk;
}

tresult PLUGIN_API Processor::getState(IBStream *state) {
  IBStreamer streamer(state, kLittleEndian);
  int32 saveBypass = mBypass ? 1 : 0;
  float saveNoteChannel = mNoteChannel;
  float saveNoteNumber = mNoteNumber;
  float saveDry = mDry;
  float saveWet = mWet;

  streamer.writeInt32(saveBypass);
  streamer.writeFloat(saveNoteChannel);
  streamer.writeFloat(saveNoteNumber);
  streamer.writeFloat(saveDry);
  streamer.writeFloat(saveWet);

  return kResultOk;
}
} // namespace Reverse300
