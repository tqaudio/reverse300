#include <math.h>

#include "../include/constants.h"
#include "../include/parameter.h"

namespace Reverse300 {
GainParameter::GainParameter(const char *title, int32 flags, int32 id) {
  UString(info.title, USTRINGSIZE(info.title)).fromAscii(title);
  UString(info.units, USTRINGSIZE(info.units)).fromAscii("dB");
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.unitId = kRootUnitId;
}

void GainParameter::toString(ParamValue normValue, String128 text) const {
  char buffer[32];

  if (normValue > 0.0) {
    double db = 20 * log10(normValue);
    sprintf(buffer, "%.2f", db);
  } else {
    strcpy(buffer, "-infinity");
  }

  UString(text, 128).fromAscii(buffer);
}

bool GainParameter::fromString(const TChar *text, ParamValue &normValue) const {
  double gain{0.0};
  char buffer[32];
  UString wrapper((TChar *)text, -1);
  wrapper.toAscii(buffer, 32);

  if (strcmp(buffer, "-oo") == 0 || strcmp(buffer, "-infinity") == 0) {
    normValue = 0;
    return true;
  }
  if (!wrapper.scanFloat(gain)) {
    normValue = 0.0;
    return true;
  }
  if (gain > 0.0) {
    gain = -gain;
  }

  normValue = exp(log(10.0) * gain / 20.0);

  return true;
}
} // namespace Reverse300
