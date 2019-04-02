#pragma once

#define MAX_DURATION 10000.0f /* ms */

#define DEFAULT_DRY 0.0f /* dB */
#define DEFAULT_DRY_NORMALIZED expf(logf(10.0f) * DEFAULT_DRY / 20.0f)

#define DEFAULT_WET 0.0f /* dB */
#define DEFAULT_WET_NORMALIZED expf(logf(10.0f) * DEFAULT_WET / 20.0f)

#define DEFAULT_MIDI_CHANNEL_NORMALIZED 0.0f /* OMNI */
#define DEFAULT_MIDI_NOTE_NORMALIZED 1.0f    /* ALL */

#include "pluginterfaces/base/ustring.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include <math.h>
#include <string.h>

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace Reverse300 {
enum Parameters : ParamID {
  kBypassId = 100,
  kDryId = 101,
  kWetId = 102,
  kNoteChannelId = 103,
  kNoteNumberId = 104
};

struct AutomationParameter {
public:
  ParamValue value;
  bool hasChanged;
  AutomationParameter() : value(0.0f), hasChanged(false){};
};

class GainParameter : public Parameter {
public:
  GainParameter(const char *title, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 string) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;
};

class LinearParameter : public Parameter {
public:
  LinearParameter(const char *title, const char *unit, double maxValue,
                  int32 precision, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 text) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;

private:
  double mMaxValue;
  int32 mPrecision;
};

class NoteNumberParameter : public Parameter {
public:
  NoteNumberParameter(const char *title, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 text) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;
};

class MIDIChannelParameter : public Parameter {
public:
  MIDIChannelParameter(const char *title, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 text) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;
};
} // namespace Reverse300
