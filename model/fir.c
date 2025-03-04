#include "fir.h"

#include <stdio.h> //error logs
#include <stdlib.h> // exit
#include <memory.h>
#define __USE_MISC
#include <math.h>
#include "combiner.h"
#include "generator.h"

static real_signal_t fir_filter_get_lpf_impulse_response_win_rectangular(double samplingFrequency, double cutoffFrequency, uint64_t filterOrder) {
    real_signal_t response = {
        .info = {
            .num_samples = filterOrder,
            .sampling_frequency = samplingFrequency,
            .start_time = 0.0
        }
    }; real_signal_alloc_values(&response);

    double frequencyScaler = samplingFrequency / cutoffFrequency; //K
    uint64_t sincCenterSampleIndex = filterOrder >> 1;
    for (uint64_t i = 0; i < sincCenterSampleIndex; i++) {
        double* pValue = response.pValues + i;
        long indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue = sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler);
        *pValue /= M_PI * indexDiff;
    }
    response.pValues[sincCenterSampleIndex] = 2.0 / frequencyScaler;
    for (uint64_t i = sincCenterSampleIndex + 1; i < response.info.num_samples; i++) {
        double* pValue = response.pValues + i;
        long indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue = sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler);
        *pValue /= M_PI * indexDiff;
    }

    return response;
}

static real_signal_t fir_filter_get_lpf_impulse_response_win_hamming(double samplingFrequency, double cutoffFrequency, uint64_t filterOrder) {
    real_signal_t response = {
        .info = {
            .num_samples = filterOrder,
            .sampling_frequency = samplingFrequency,
            .start_time = 0.0
        }
    }; real_signal_alloc_values(&response);

    for (uint64_t i = 0; i < filterOrder; i++) {
        double* pValue = response.pValues + i;
        *pValue = 0.53836 - 0.46164 * cos(2 * M_PI * ((double)i) / (double)filterOrder);
    }

    return response;
    /*real_signal_t response = {
        .info = {
            .num_samples = filterOrder,
            .sampling_frequency = samplingFrequency,
            .start_time = 0.0
        }
    }; real_signal_alloc_values(&response);

    double frequencyScaler = samplingFrequency / cutoffFrequency; //K
    uint64_t sincCenterSampleIndex = filterOrder >> 1;
    //
    //    alpha = 0.53836
    //    beta = -0.46164
    //
    double alpha = 0.53836;
    double beta = -0.46164;

    long indexDiff;
    for (uint64_t i = 0; i < sincCenterSampleIndex; i++) {
        double* pValue = response.pValues + i;
        indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue = alpha * sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (double)indexDiff;
        *pValue += ((double)indexDiff)*beta*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - 0.25 * frequencyScaler * frequencyScaler);
    }
    indexDiff = ((long)sincCenterSampleIndex) - (long)sincCenterSampleIndex;
    response.pValues[sincCenterSampleIndex] = alpha * 2.0 / frequencyScaler;
    response.pValues[sincCenterSampleIndex] += ((double)indexDiff)*beta*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - 0.25 * frequencyScaler * frequencyScaler);
    for (uint64_t i = sincCenterSampleIndex + 1; i < response.info.num_samples; i++) {
        double* pValue = response.pValues + i;
        indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue = alpha * sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (double)indexDiff;
        *pValue += ((double)indexDiff)*beta*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - 0.25 * frequencyScaler * frequencyScaler);
    }

    return response;*/
    /*real_signal_t response = fir_filter_get_lpf_impulse_response_win_rectangular(samplingFrequency, cutoffFrequency, filterOrder);

    //double frequencyScaler = samplingFrequency / cutoffFrequency; //K
    uint64_t sincCenterSampleIndex = filterOrder >> 1;
    for (uint64_t i = 0; i < response.info.num_samples; i++) {
        double* pValue = response.pValues + i;
        long indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue *= 0.53836 - 0.46164 * cos(2.0 * M_PI * ((double)indexDiff) / filterOrder);
    }

    return response;*/
}

static real_signal_t fir_filter_get_lpf_impulse_response_win_hanning(double samplingFrequency, double cutoffFrequency, uint64_t filterOrder) {
    real_signal_t response = {
        .info = {
            .num_samples = filterOrder,
            .sampling_frequency = samplingFrequency,
            .start_time = 0.0
        }
    }; real_signal_alloc_values(&response);

    for (uint64_t i = 0; i < filterOrder; i++) {
        double* pValue = response.pValues + i;
        *pValue = 0.5 - 0.5 * cos(2 * M_PI * ((double)i) / (double)filterOrder);
    }

    return response;

    /*real_signal_t response = {
        .info = {
            .num_samples = filterOrder,
            .sampling_frequency = samplingFrequency,
            .start_time = 0.0
        }
    }; real_signal_alloc_values(&response);

    double frequencyScaler = samplingFrequency / cutoffFrequency; //K
    uint64_t sincCenterSampleIndex = filterOrder >> 1;
    //
    //    alpha = 0.53836
    //    beta = -0.46164
    //
    double alpha = 0.5;
    double beta = -0.5;

    long indexDiff;
    for (uint64_t i = 0; i < sincCenterSampleIndex; i++) {
        double* pValue = response.pValues + i;
        indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue = alpha * sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (double)indexDiff;
        *pValue += ((double)indexDiff)*beta*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - 0.25 * frequencyScaler * frequencyScaler);
    }
    indexDiff = ((long)sincCenterSampleIndex) - (long)sincCenterSampleIndex;
    response.pValues[sincCenterSampleIndex] = alpha * 2.0 / frequencyScaler;
    response.pValues[sincCenterSampleIndex] += ((double)indexDiff)*beta*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - 0.25 * frequencyScaler * frequencyScaler);
    for (uint64_t i = sincCenterSampleIndex + 1; i < response.info.num_samples; i++) {
        double* pValue = response.pValues + i;
        indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue = alpha * sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (double)indexDiff;
        *pValue += ((double)indexDiff)*beta*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - 0.25 * frequencyScaler * frequencyScaler);
    }

    return response;*/

    /*real_signal_t response = fir_filter_get_lpf_impulse_response_win_rectangular(samplingFrequency, cutoffFrequency, filterOrder);

    //double frequencyScaler = samplingFrequency / cutoffFrequency; //K
    uint64_t sincCenterSampleIndex = filterOrder >> 1;
    for (uint64_t i = 0; i < response.info.num_samples; i++) {
        double* pValue = response.pValues + i;
        long indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue *= 0.5 - 0.5 * cos(2.0 * M_PI * ((double)indexDiff) / filterOrder);
    }

    return response;*/
    
}

static real_signal_t fir_filter_get_lpf_impulse_response_win_blackman(double samplingFrequency, double cutoffFrequency, uint64_t filterOrder) {

    real_signal_t response = {
        .info = {
            .num_samples = filterOrder,
            .sampling_frequency = samplingFrequency,
            .start_time = 0.0
        }
    }; real_signal_alloc_values(&response);

    for (uint64_t i = 0; i < filterOrder; i++) {
        double* pValue = response.pValues + i;
        *pValue = 0.42 - 0.5 * cos(2 * M_PI * ((double)i) / (double)filterOrder);
        *pValue += 0.08 * cos(4 * M_PI * ((double)i) / (double)filterOrder);
    }

    return response;

    /*real_signal_t response = {
        .info = {
            .num_samples = filterOrder,
            .sampling_frequency = samplingFrequency,
            .start_time = 0.0
        }
    }; real_signal_alloc_values(&response);

    double frequencyScaler = samplingFrequency / cutoffFrequency; //K
    uint64_t sincCenterSampleIndex = filterOrder >> 1;
    //
    //    alpha = 0.53836
    //    beta = -0.46164
    //
    double alpha = 0.42;
    double beta = -0.5;
    double gamma = 0.08;

    long indexDiff;
    for (uint64_t i = 0; i < sincCenterSampleIndex; i++) {
        double* pValue = response.pValues + i;
        indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue = alpha * sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (double)indexDiff;
        *pValue += ((double)indexDiff)*beta*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - 0.25 * frequencyScaler * frequencyScaler);
        *pValue += ((double)indexDiff)*gamma*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - frequencyScaler * frequencyScaler);
    }
    indexDiff = ((long)sincCenterSampleIndex) - (long)sincCenterSampleIndex;
    response.pValues[sincCenterSampleIndex] = alpha * 2.0 / frequencyScaler;
    response.pValues[sincCenterSampleIndex] += ((double)indexDiff)*beta*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - 0.25 * frequencyScaler * frequencyScaler);
    response.pValues[sincCenterSampleIndex] += ((double)indexDiff)*gamma*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - frequencyScaler * frequencyScaler);
    for (uint64_t i = sincCenterSampleIndex + 1; i < response.info.num_samples; i++) {
        double* pValue = response.pValues + i;
        indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue = alpha * sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (double)indexDiff;
        *pValue += ((double)indexDiff)*beta*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - 0.25 * frequencyScaler * frequencyScaler);
        *pValue += ((double)indexDiff)*gamma*sin(2.0 * M_PI * ((double)indexDiff) / frequencyScaler) / M_PI / (((double)indexDiff) * ((double)indexDiff) - frequencyScaler * frequencyScaler);
    }

    return response;*/

    /*real_signal_t response = fir_filter_get_lpf_impulse_response_win_rectangular(samplingFrequency, cutoffFrequency, filterOrder);

    //double frequencyScaler = samplingFrequency / cutoffFrequency; //K
    uint64_t sincCenterSampleIndex = filterOrder >> 1;
    for (uint64_t i = 0; i < response.info.num_samples; i++) {
        double* pValue = response.pValues + i;
        long indexDiff = ((long)i) - (long)sincCenterSampleIndex;
        *pValue *= 0.42 - 0.5 * cos(2.0 * M_PI * ((double)indexDiff) / filterOrder) + 0.08 * cos(4.0 * M_PI * ((double)indexDiff) / filterOrder);

    }

    return response;*/
}

static real_signal_t fir_filter_get_lpf_impulse_response(double samplingFrequency, double cutoffFrequency, uint64_t filterOrder, fir_windowing_window_type_t winType) {
    switch (winType) {
        case FIR_WINDOWING_WINDOW_TYPE_RECTANGULAR:
            return fir_filter_get_lpf_impulse_response_win_rectangular(samplingFrequency, cutoffFrequency, filterOrder);
        case FIR_WINDOWING_WINDOW_TYPE_HAMMING:
            return fir_filter_get_lpf_impulse_response_win_hamming(samplingFrequency, cutoffFrequency, filterOrder);
        case FIR_WINDOWING_WINDOW_TYPE_HANNING:
            return fir_filter_get_lpf_impulse_response_win_hanning(samplingFrequency, cutoffFrequency, filterOrder);
        case FIR_WINDOWING_WINDOW_TYPE_BLACKMAN:
            return fir_filter_get_lpf_impulse_response_win_blackman(samplingFrequency, cutoffFrequency, filterOrder);
        default:
            fprintf(stderr, "Unknown window type detected!");
            exit(EXIT_FAILURE);
            break;
    }
}

static real_signal_t fir_filter_get_hpf_impulse_response(double samplingFrequency, double cutoffFrequency, uint64_t filterOrder, fir_windowing_window_type_t winType) {
    double lpfCutoffFrequency = 0.5 * samplingFrequency - cutoffFrequency;
    real_signal_t impulseResponse = fir_filter_get_lpf_impulse_response(samplingFrequency, lpfCutoffFrequency, filterOrder, winType);
    real_signal_t sinewave = generate_cosine((generator_info_t){
        .sampling_frequency = impulseResponse.info.sampling_frequency
    }, 1, 2.0 / samplingFrequency, impulseResponse.info.start_time, ((double)impulseResponse.info.num_samples) / impulseResponse.info.sampling_frequency);
    multiply_signal(&impulseResponse, &sinewave);

    /*real_signal_t modulationWave = {
        .info = {
            .num_samples = filterOrder, 
            .sampling_frequency = impulseResponse.info.sampling_frequency,
            .start_time = impulseResponse.info.start_time
        },
        .pValues = 0
    };
    real_signal_alloc_values(&modulationWave);
    for (uint64_t i = 0; i < modulationWave.info.num_samples; i += 2) {
        modulationWave.pValues[i] = 1.0;
    } for (uint64_t i = 1; i < modulationWave.info.num_samples; i += 2) {
        modulationWave.pValues[i] = -1.0;
    }

    multiply_signal(&impulseResponse, &modulationWave);*/
    
    return impulseResponse;
}

/**
 * Generate impulse response for a bandpass FIR filter
*/
static real_signal_t fir_filter_get_bpf_impulse_response(double samplingFrequency, double leftCutoffFrequency, double rightCutoffFrequency, uint64_t filterOrder, fir_windowing_window_type_t winType) {
    /*
        f_d = f_c - f_o
        f_g = f_c + f_o

        f_o = (f_g - f_d) / 2
        f_c = (f_g + f_d) / 2;
    */
    
    double relayFreqency = 0.5 * (rightCutoffFrequency + leftCutoffFrequency); //f_c (generalized from f_p / 4)
    double lpfCutoffFrequency = 0.5 * (rightCutoffFrequency - leftCutoffFrequency);
   
    real_signal_t impulseResponse = fir_filter_get_lpf_impulse_response(samplingFrequency, lpfCutoffFrequency, filterOrder, winType);
    real_signal_t sinewave = generate_cosine((generator_info_t){
        .sampling_frequency = impulseResponse.info.sampling_frequency
    }, 1, 1.0 / relayFreqency, impulseResponse.info.start_time, ((double)impulseResponse.info.num_samples) / impulseResponse.info.sampling_frequency);
    multiply_signal(&impulseResponse, &sinewave);

    return impulseResponse;
}

void fir_filter_real_signal_lowpass(real_signal_t* pSignal, fir_lowpass_config_t* pConfig) {
    //fprintf(stderr, "Error: Not implemented."); exit(EXIT_FAILURE);
    /*real_signal_t filteredSignal = {
        .info = {
            .num_samples = pInputSignal->info.num_samples,
            .start_time = pInputSignal->info.start_time,
            .sampling_frequency = pInputSignal->info.sampling_frequency
        },
        .pValues = 0
    };
    real_signal_alloc_values(&filteredSignal);*/

    if (pConfig->cutoff_frequency != 0.0) {
        real_signal_t impulseResponse = fir_filter_get_lpf_impulse_response(
                                            pSignal->info.sampling_frequency, 
                                            pConfig->cutoff_frequency,
                                            pConfig->windowing.num_fir_coeffs,
                                            pConfig->windowing.window_type
                                        );
        
        /*real_signal_t filtrationWorkspaceSignal = {
            .info = {
                .num_samples = impulseResponse.info.num_samples,
                .start_time = impulseResponse.info.start_time,
                .sampling_frequency = impulseResponse.info.sampling_frequency
            },
            .pValues = 0
        };*/

        real_signal_t* pFiltrationWorkspaceSignal = &impulseResponse;

        //real_signal_alloc_values(&filtrationWorkspaceSignal);

        convolve_signal(pFiltrationWorkspaceSignal, pSignal);
        pFiltrationWorkspaceSignal->info.start_time = pSignal->info.start_time;
        pSignal->info = pFiltrationWorkspaceSignal->info;
        real_signal_free_values(pSignal);
        pSignal->pValues = pFiltrationWorkspaceSignal->pValues;
    } else {
        fprintf(stdout, "Warning: Zero sampling frequency input signal detected in LPF FIR filter");
        for (uint64_t i = 0; i < pSignal->info.num_samples; i++) {
            pSignal->pValues[i] = 0;
        }
    }
    
    /*pOutputSignal->info = filteredSignal.info;
    pOutputSignal->pValues = (double*) realloc(pOutputSignal->pValues)*/
}

void fir_filter_real_signal_highpass(real_signal_t* pSignal, fir_highpass_config_t* pConfig) {
    if (pConfig->cutoff_frequency != 0.0) {
        real_signal_t impulseResponse = fir_filter_get_hpf_impulse_response(
                                            pSignal->info.sampling_frequency, 
                                            pConfig->cutoff_frequency,
                                            pConfig->windowing.num_fir_coeffs,
                                            pConfig->windowing.window_type
                                        );
        real_signal_t* pFiltrationWorkspaceSignal = &impulseResponse;
        convolve_signal(pFiltrationWorkspaceSignal, pSignal);
        pFiltrationWorkspaceSignal->info.start_time = pSignal->info.start_time;
        pSignal->info = pFiltrationWorkspaceSignal->info;
        real_signal_free_values(pSignal);
        pSignal->pValues = pFiltrationWorkspaceSignal->pValues;
    } else {
        fprintf(stdout, "Warning: Zero sampling frequency input signal detected in HPF FIR filter");
    }
}

void fir_filter_real_signal_bandpass(real_signal_t* pSignal, fir_bandpass_config_t* pConfig) {
    real_signal_t impulseResponse = fir_filter_get_bpf_impulse_response(
                                        pSignal->info.sampling_frequency, 
                                        pConfig->left_cutoff_frequency,
                                        pConfig->right_cutoff_frequency,
                                        pConfig->windowing.num_fir_coeffs,
                                        pConfig->windowing.window_type
                                    );
    real_signal_t* pFiltrationWorkspaceSignal = &impulseResponse;
    convolve_signal(pFiltrationWorkspaceSignal, pSignal);
    pFiltrationWorkspaceSignal->info.start_time = pSignal->info.start_time;
    pSignal->info = pFiltrationWorkspaceSignal->info;
    real_signal_free_values(pSignal);
    pSignal->pValues = pFiltrationWorkspaceSignal->pValues;
}

void fir_common_config_print(fir_common_config_t* pCommonConfig) {
    fir_windowing_window_type_t wtype;
    if ((pCommonConfig->filterType == FIR_FILTER_TYPE_LOWPASS) || (pCommonConfig->filterType == FIR_FILTER_TYPE_HIGHPASS)) {
        if (pCommonConfig->filterType == FIR_FILTER_TYPE_LOWPASS) {
            printf("Filter type: FIR_FILTER_TYPE_LOWPASS\n");
        } else {
            printf("Filter type: FIR_FILTER_TYPE_HIGHPASS\n");
        }
        printf("Cutoff frequency: %f\n", pCommonConfig->oneSidedConfig.cutoff_frequency);
        printf("Num FIR coeffs: %lu\n", pCommonConfig->oneSidedConfig.windowing.num_fir_coeffs);
        wtype = pCommonConfig->oneSidedConfig.windowing.window_type;
    } else {
        printf("Filter type: FIR_FILTER_TYPE_BANDPASS\n");
        printf("Left cutoff frequency: %f\n", pCommonConfig->doubleSidedConfig.left_cutoff_frequency);
        printf("Right cutoff frequency: %f\n", pCommonConfig->doubleSidedConfig.right_cutoff_frequency);
        printf("Num FIR coeffs: %lu\n", pCommonConfig->doubleSidedConfig.windowing.num_fir_coeffs);
        wtype = pCommonConfig->doubleSidedConfig.windowing.window_type;
    }

    switch (wtype) {
        case FIR_WINDOWING_WINDOW_TYPE_RECTANGULAR:
            printf("Windowing window type: RECTANGULAR\n");
            break;
        case FIR_WINDOWING_WINDOW_TYPE_HAMMING:
            printf("Windowing window type: HAMMING\n");
            break;
        case FIR_WINDOWING_WINDOW_TYPE_HANNING:
            printf("Windowing window type: HANNING\n");
            break;
        case FIR_WINDOWING_WINDOW_TYPE_BLACKMAN:
            printf("Windowing window type: BLACKMAN\n");
            break;
        default:
            printf("Windowing window type: [UNKNOWN]\n");
            break;
    }
}