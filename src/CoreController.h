#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "App.h"

#if SMTG_OS_MACOS
#include <TargetConditionals.h>
#endif

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
class ICoreTestController : public FUnknown
{
public:
	virtual bool PLUGIN_API doTest () = 0;
//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (ICoreTestController, 0x9FC98F39, 0x27234512, 0x84FBC4AD, 0x618A14FD)

//-----------------------------------------------------------------------------
class CoreController : public EditController, public ICoreTestController
{
public:
//------------------------------------------------------------------------
	// create function required for Plug-in factory,
	// it will be called to create new instances of this controller
//------------------------------------------------------------------------
	static FUnknown* createInstance (void*) { return (IEditController*)new CoreController (); }

	//---from IPluginBase--------
	tresult initialize (FUnknown* context) SMTG_OVERRIDE;

//---from EditController-----
	tresult setComponentState (IBStream* state) SMTG_OVERRIDE;

	bool doTest () SMTG_OVERRIDE;

	//---Interface---------
	OBJ_METHODS (CoreController, EditController)
	DEFINE_INTERFACES
		DEF_INTERFACE (ICoreTestController)
	END_DEFINE_INTERFACES (EditController)
	REFCOUNT_METHODS (EditController)

private:
  uWS::App webSocket;
};

} // namespace Vst
} // namespace Steinberg
