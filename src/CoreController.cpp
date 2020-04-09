#include "CoreController.h"
#include "coreid.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/ibstream.h"

#if TARGET_OS_IPHONE
#include "interappaudio/iosEditor.h"
#endif
#include "base/source/fstreamer.h"

namespace Steinberg {
namespace Vst {

DEF_CLASS_IID (ICoreTestController)

//-----------------------------------------------------------------------------
tresult PLUGIN_API CoreController::initialize (FUnknown* context)
{
	tresult result = EditController::initialize (context);
	if (result == kResultTrue)
	{
		parameters.addParameter (STR16 ("Bypass"), 0, 1, 0, ParameterInfo::kCanAutomate|ParameterInfo::kIsBypass, kBypassId);

		parameters.addParameter (STR16 ("Delay"), STR16 ("sec"), 0, 1, ParameterInfo::kCanAutomate, kDelayId);
	}
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CoreController::setComponentState (IBStream* state)
{
	// we receive the current state of the component (processor part)
	// we read only the gain and bypass value...
	if (!state)
		return kResultFalse;

  IBStreamer streamer (state, kLittleEndian);
	/* float savedDelay = 0.f; */
	/* if (streamer.readFloat (savedDelay) == false) */
		/* return kResultFalse; */
	/* setParamNormalized (kDelayId, savedDelay); */

	int32 bypassState = 0;
	if (streamer.readInt32 (bypassState) == false)
	{
		// could be an old version, continue
	}
	setParamNormalized (kBypassId, bypassState ? 1 : 0);

	return kResultOk;
}

//------------------------------------------------------------------------
bool PLUGIN_API CoreController::doTest ()
{
	// this is called when running thru the validator
	// we can now run our own test cases
	return true;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
