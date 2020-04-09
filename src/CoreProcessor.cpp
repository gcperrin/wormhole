#include "CoreProcessor.h"
#include "coreid.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>
#include <cstdlib>
#include "base/source/fstreamer.h"

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
CoreProcessor::CoreProcessor ()
: mDelay (1)
, mBuffer (0)
, mBufferPos (0)
, mBypass (false)
{
	setControllerClass (CoreControllerUID);
}

//-----------------------------------------------------------------------------
tresult CoreProcessor::initialize (FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result == kResultTrue)
	{
		addAudioInput (STR16 ("AudioInput"), SpeakerArr::kStereo);
		addAudioOutput (STR16 ("AudioOutput"), SpeakerArr::kStereo);
	}
	return result;
}

//-----------------------------------------------------------------------------
tresult CoreProcessor::setBusArrangements (SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
{
	// we only support one in and output bus and these buses must have the same number of channels
	if (numIns == 1 && numOuts == 1 && inputs[0] == outputs[0])
		return AudioEffect::setBusArrangements (inputs, numIns, outputs, numOuts);
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult CoreProcessor::setActive (TBool state)
{
	SpeakerArrangement arr;
	if (getBusArrangement (kOutput, 0, arr) != kResultTrue)
		return kResultFalse;
	int32 numChannels = SpeakerArr::getChannelCount (arr);
	if (numChannels == 0)
		return kResultFalse;

	if (state)
	{
		mBuffer = (float**)std::malloc (numChannels * sizeof (float*));

		size_t size = (size_t)(processSetup.sampleRate * sizeof (float) + 0.5);
		for (int32 channel = 0; channel < numChannels; channel++)
		{
			mBuffer[channel] = (float*)std::malloc (size);	// 1 second delay max
			memset (mBuffer[channel], 0, size);
		}
		mBufferPos = 0;
	}
	else
	{
		if (mBuffer)
		{
			for (int32 channel = 0; channel < numChannels; channel++)
			{
				std::free (mBuffer[channel]);
			}
			std::free (mBuffer);
			mBuffer = 0;
		}
	}
	return AudioEffect::setActive (state);
}

//-----------------------------------------------------------------------------
tresult CoreProcessor::process (ProcessData& data)
{
	if (data.inputParameterChanges)
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
		for (int32 index = 0; index < numParamsChanged; index++)
		{
			IParamValueQueue* paramQueue = data.inputParameterChanges->getParameterData (index);
			if (paramQueue)
			{
				ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				switch (paramQueue->getParameterId ())
				{
					case kDelayId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							mDelay = value;
						break;
					case kBypassId:
						if (paramQueue->getPoint (numPoints - 1,  sampleOffset, value) == kResultTrue)
						{
							mBypass = (value > 0.5f);
			}
						break;
		}
	}
		}
	}

	if (data.numSamples > 0)
	{
		SpeakerArrangement arr;
		getBusArrangement (kOutput, 0, arr);
		int32 numChannels = SpeakerArr::getChannelCount (arr);

		// TODO do something in Bypass : copy inpuit to output if necessary...

		// apply delay
		int32 delayInSamples = std::max<int32> (1, (int32)(mDelay * processSetup.sampleRate)); // we have a minimum of 1 sample delay here
		for (int32 channel = 0; channel < numChannels; channel++)
		{
			float* inputChannel = data.inputs[0].channelBuffers32[channel];
			float* outputChannel = data.outputs[0].channelBuffers32[channel];

			int32 tempBufferPos = mBufferPos;
			for (int32 sample = 0; sample < data.numSamples; sample++)
			{
				float tempSample = inputChannel[sample];
				outputChannel[sample] = mBuffer[channel][tempBufferPos];
				mBuffer[channel][tempBufferPos] = tempSample;
				tempBufferPos++;
				if (tempBufferPos >= delayInSamples)
					tempBufferPos = 0;
			}
		}
		mBufferPos += data.numSamples;
		while (delayInSamples && mBufferPos >= delayInSamples)
			mBufferPos -= delayInSamples;
	}
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult CoreProcessor::setState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	// called when we load a preset, the model has to be reloaded

	IBStreamer streamer (state, kLittleEndian);
	float savedDelay = 0.f;
	if (streamer.readFloat (savedDelay) == false)
		return kResultFalse;

	int32 savedBypass = 0;
	if (streamer.readInt32 (savedBypass) == false)
	{
		// could be an old version, continue
	}

	mDelay = savedDelay;
	mBypass = savedBypass > 0;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult CoreProcessor::getState (IBStream* state)
{
	// here we need to save the model

	IBStreamer streamer (state, kLittleEndian);

	streamer.writeFloat (mDelay);
	streamer.writeInt32 (mBypass ? 1 : 0);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
