#include <soxr.h>

#include "EMULATOR_SPU.H"
#include "Core/Debug/EMULATOR_LOG.H"

unsigned char* Resample_PCM(int src_rate, int dst_rate, short* in, int numSamples, int* outNumSamples)
{
#define AL(a) (sizeof(a)/sizeof((a)[0])) 
    soxr_io_spec_t spec = soxr_io_spec(SOXR_INT16_I, SOXR_INT16_I);
    soxr_quality_spec_t qualitySpec = soxr_quality_spec(SOXR_QQ, 0);
    const int inChannelCount = 1;
    const int outChannelCount = 2;
    const int standardRate = src_rate;
    const int resampledRate = dst_rate;
#if defined(OLD_SYSTEM)
    const int nInputSamples = numSamples;
    int nOutputSamples = numSamples * sizeof(short) * (1.0 + (dst_rate / standardRate));
#else
    const int nInputSamples = numSamples / 2;
    int nOutputSamples = (size_t)((numSamples * dst_rate * outChannelCount) / (standardRate * outChannelCount));
#endif
    short* resampled = new short[nOutputSamples];
    const int16_t* receivedSamples = reinterpret_cast<const int16_t*>(in);
    soxr_error_t soxError = soxr_oneshot(standardRate, resampledRate, inChannelCount,
        receivedSamples, nInputSamples, NULL,
        reinterpret_cast<int16_t*>(resampled), nOutputSamples, NULL,
        &spec, &qualitySpec, 0);
    if (soxError) {
        eprinterr("ERR");
    }
    *outNumSamples = nOutputSamples;
    return (unsigned char*)resampled;
}

void Resample_Free(unsigned char* data)
{
    delete[] data;
}
