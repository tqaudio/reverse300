/*
 * Author: Yoshiyuki Koyanagi <moutend@gmail.com>
 * Copyright: Yoshiyuki Koyanagi
 * License: GNU General Public License version 3
 */

#include "public.sdk/source/main/pluginfactoryvst3.h"

#include "../include/controller.h"
#include "../include/id.h"
#include "../include/processor.h"
#include "../include/version.h"

#include "public.sdk/source/main/pluginfactoryvst3.h"

BEGIN_FACTORY_DEF(stringCompanyName, stringCompanyWeb, stringCompanyEmail)
DEF_CLASS2(INLINE_UID_FROM_FUID(Reverse300::ProcessorID),
           PClassInfo::kManyInstances, kVstAudioEffectClass, stringPluginName,
           Vst::kDistributable, Vst::PlugType::kFx, FULL_VERSION_STR,
           kVstVersionString, Reverse300::Processor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(Reverse300::ControllerID),
           PClassInfo::kManyInstances, kVstComponentControllerClass,
           stringPluginName "Controller", 0, "", FULL_VERSION_STR,
           kVstVersionString, Reverse300::Controller::createInstance)
END_FACTORY

bool InitModule() { return true; }

bool DeinitModule() { return true; }
