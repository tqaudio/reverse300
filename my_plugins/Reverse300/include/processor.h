#pragma once

#include <math.h>

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioeffect.h"

#include "buffer.h"
#include "constants.h"
#include "id.h"
#include "parameter.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace Reverse300 {
class Processor : public AudioEffect {
public:
  static FUnknown *createInstance(void *) {
    return (IAudioProcessor *)new Processor();
  }

  Processor();

  tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
  tresult PLUGIN_API setBusArrangements(SpeakerArrangement *inputs,
                                        int32 numIns,
                                        SpeakerArrangement *outputs,
                                        int32 numOuts) SMTG_OVERRIDE;
  tresult PLUGIN_API setupProcessing(ProcessSetup &newSetup) SMTG_OVERRIDE;
  tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
  tresult PLUGIN_API process(ProcessData &data) SMTG_OVERRIDE;
  tresult PLUGIN_API setState(IBStream *state) SMTG_OVERRIDE;
  tresult PLUGIN_API getState(IBStream *state) SMTG_OVERRIDE;

private:
  AutomationParameter *mNoteOns = nullptr;
  AutomationParameter *mNoteOffs = nullptr;
  AutomationParameter *mDries = nullptr;
  AutomationParameter *mWets = nullptr;

  bool mBypass = false;
  ParamValue mDry = exp(log(10.0) * Constants::defaultDry / 20.0);
  ParamValue mWet = exp(log(10.0) * Constants::defaultWet / 20.0);

  Buffer **mBuffer = nullptr;
};
} // namespace Reverse300
