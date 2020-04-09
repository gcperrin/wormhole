#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg {
  namespace Vst {

    class CoreProcessor : public AudioEffect
    {
      public:
        CoreProcessor ();

        tresult initialize (FUnknown* context) SMTG_OVERRIDE;
        tresult setBusArrangements (SpeakerArrangement* inputs,
                                               int32 numIns,
                                               SpeakerArrangement* outputs,
                                               int32 numOuts) SMTG_OVERRIDE;

        tresult setActive (TBool state) SMTG_OVERRIDE;
        tresult process (ProcessData& data) SMTG_OVERRIDE;

        tresult setState (IBStream* state) SMTG_OVERRIDE;
        tresult getState (IBStream* state) SMTG_OVERRIDE;

        static FUnknown* createInstance (void*) { return (IAudioProcessor*)new CoreProcessor (); }

      protected:
        ParamValue mDelay;
        float** mBuffer;
        int32 mBufferPos;
        bool mBypass;
    };

  }
}
