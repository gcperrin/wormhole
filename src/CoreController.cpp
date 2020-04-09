#include "CoreController.h"
#include "coreid.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/ibstream.h"
#include "base/source/fstreamer.h"

namespace Steinberg {
  namespace Vst {
    DEF_CLASS_IID (ICoreTestController)

      struct PerSocketData {
        /* Fill with user data */
    };

    tresult CoreController::initialize (FUnknown* context)
    {
      tresult result = EditController::initialize (context);
      if (result == kResultTrue)
      {
        parameters.addParameter (STR16 ("Bypass"), 0, 1, 0, ParameterInfo::kCanAutomate|ParameterInfo::kIsBypass, kBypassId);

        parameters.addParameter (STR16 ("Delay"), STR16 ("sec"), 0, 1, ParameterInfo::kCanAutomate, kDelayId);
      }

      webSocket.ws<PerSocketData>("/*", {
        /* Settings */
        .compression = uWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024,
        .idleTimeout = 10,
        .maxBackpressure = 1 * 1024 * 1204,
        /* Handlers */
        .open = [](auto *ws, auto *req) {
            /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
        },
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            ws->send(message, opCode);
        },
        .drain = [](auto *ws) {
            /* Check ws->getBufferedAmount() here */
        },
        .ping = [](auto *ws) {
            /* Not implemented yet */
        },
        .pong = [](auto *ws) {
            /* Not implemented yet */
        },
        .close = [](auto *ws, int code, std::string_view message) {
            /* You may access ws->getUserData() here */
        }
    }).listen(9001, [](auto *token) {
        if (token) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    }).run();
      return kResultTrue;
    }

    int32 CoreController::setComponentState (IBStream* state)
    {
      // we receive the current state of the component (processor part)
      // we read only the gain and bypass value...
      if (!state)
        return kResultFalse;

      IBStreamer streamer (state, kLittleEndian);
      float savedDelay = 0.f;
      if (streamer.readFloat (savedDelay) == false)
        return kResultFalse;
      setParamNormalized (kDelayId, savedDelay);

      int32 bypassState = 0;
      if (streamer.readInt32 (bypassState) == false)
      {
        // could be an old version, continue
      }
      setParamNormalized (kBypassId, bypassState ? 1 : 0);

      return kResultOk;
    }

    bool CoreController::doTest ()
    {
      // this is called when running thru the validator
      // we can now run our own test cases
      return true;
    }

  } // namespace Vst
} // namespace Steinberg
