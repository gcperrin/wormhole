#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg {
  namespace Vst {

    class CoreProcessor : public AudioEffect
    {
      public:
        CoreProcessor ();

        tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
        tresult PLUGIN_API setBusArrangements (SpeakerArrangement* inputs,
                                               int32 numIns,
                                               SpeakerArrangement* outputs,
                                               int32 numOuts) SMTG_OVERRIDE;

        tresult PLUGIN_API setActive (TBool state) SMTG_OVERRIDE;
        tresult PLUGIN_API process (ProcessData& data) SMTG_OVERRIDE;

        tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;

        static FUnknown* createInstance (void*) { return (IAudioProcessor*)new CoreProcessor (); }

      protected:
        ParamValue mDelay;
        float** mBuffer;
        int32 mBufferPos;
        bool mBypass;
    };

  } // namespace Vst
} // namespace Steinberg
