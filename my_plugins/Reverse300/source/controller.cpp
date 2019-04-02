#include "../include/controller.h"

namespace Reverse300 {
tresult PLUGIN_API Controller::initialize(FUnknown *context) {
  tresult result = EditController::initialize(context);

  if (result != kResultOk) {
    return result;
  }

  parameters.addParameter(STR16("Bypass"), 0, 1, 0,
                          ParameterInfo::kCanAutomate |
                              ParameterInfo::kIsBypass,
                          Parameters::kBypassId);

  GainParameter *dry =
      new GainParameter("Dry", ParameterInfo::kCanAutomate, Parameters::kDryId);
  parameters.addParameter(dry);

  GainParameter *wet =
      new GainParameter("Wet", ParameterInfo::kCanAutomate, Parameters::kWetId);
  parameters.addParameter(wet);

  MIDIChannelParameter *noteChannel =
      new MIDIChannelParameter("Note On Channel", ParameterInfo::kCanAutomate,
                               Parameters::kNoteChannelId);
  parameters.addParameter(noteChannel);

  NoteNumberParameter *noteName = new NoteNumberParameter(
      "Note Name", ParameterInfo::kCanAutomate, Parameters::kNoteNumberId);
  parameters.addParameter(noteName);

  return result;
}

tresult PLUGIN_API Controller::setComponentState(IBStream *state) {
  if (!state) {
    return kResultFalse;
  }

  IBStreamer streamer(state, kLittleEndian);
  int32 savedBypass;
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

  setParamNormalized(Parameters::kBypassId, savedBypass ? 1 : 0);
  setParamNormalized(Parameters::kNoteChannelId, savedNoteChannel);
  setParamNormalized(Parameters::kNoteNumberId, savedNoteNumber);
  setParamNormalized(Parameters::kDryId, savedDry);
  setParamNormalized(Parameters::kWetId, savedWet);

  return kResultOk;
}

IPlugView *PLUGIN_API Controller::createView(const char *name) {
  if (name && strcmp(name, "editor") == 0) {
    auto *view = new VST3Editor(this, "Reverse300", "main.uidesc");
    return view;
  }
  return nullptr;
}
} // namespace Reverse300
