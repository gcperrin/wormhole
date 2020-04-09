#include "CoreController.h"
#include "CoreProcessor.h"
#include "coreid.h"
/* #include "exampletest.h" */
#include "version.h"	// for versioning
#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "Core"


BEGIN_FACTORY_DEF ("Steinberg Media Technologies",
                   "http://www.steinberg.net",
                   "mailto:info@steinberg.de")

	DEF_CLASS2 (INLINE_UID_FROM_FUID(Steinberg::Vst::CoreProcessorUID),
				PClassInfo::kManyInstances,
				kVstAudioEffectClass,
				stringPluginName,
				Vst::kDistributable,
				"Fx|Delay",
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,
				Steinberg::Vst::CoreProcessor::createInstance)

  DEF_CLASS2 (INLINE_UID_FROM_FUID(Steinberg::Vst::CoreControllerUID),
				PClassInfo::kManyInstances,
				kVstComponentControllerClass,
				stringPluginName "CoreController",	// controller name (could be the same than component name)
				0,						// not used here
				"",						// not used here
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,
				Steinberg::Vst::CoreController::createInstance)

	/* // add Test Factory */
	/* DEF_CLASS2 (INLINE_UID_FROM_FUID(Steinberg::Vst::CoreTestFactory::cid), */
	/* 			PClassInfo::kManyInstances, */
	/* 			kTestClass, */
	/* 			stringPluginName "Test Factory", */
	/* 			0, */
	/* 			"", */
	/* 			"", */
	/* 			"", */
	/* 			Steinberg::Vst::CoreTestFactory::createInstance) */

END_FACTORY

bool InitModule () { return true; }
bool DeinitModule () { return true; }

