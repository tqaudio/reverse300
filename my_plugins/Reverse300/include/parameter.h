#pragma once

#include "pluginterfaces/base/ustring.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace Reverse300 {
enum Parameters : ParamID {
  kBypassId = 100,
  kDryId = 101,
  kWetId = 102,
};

struct AutomationParameter {
  ParamValue value = 0.0;
  bool hasChanged = false;
};

class GainParameter : public Parameter {
public:
  GainParameter(const char *title, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 string) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;
};
} // namespace Reverse300
