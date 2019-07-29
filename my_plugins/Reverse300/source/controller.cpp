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

  return result;
}

tresult PLUGIN_API Controller::setComponentState(IBStream *state) {
  if (!state) {
    return kResultFalse;
  }

  IBStreamer streamer(state, kLittleEndian);
  int32 savedBypass;
  float savedDry;
  float savedWet;

  if (!streamer.readInt32(savedBypass)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedDry)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedWet)) {
    return kResultFalse;
  }

  setParamNormalized(Parameters::kBypassId, savedBypass ? 1 : 0);
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
