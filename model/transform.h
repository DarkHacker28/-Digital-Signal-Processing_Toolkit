/**
 * @file transform.h
 * @attention As gnuplot can handle signal data conversion to prepare histogram, the following functions became redundant as for ZAD1:
 * rsignal_to_histogram_transform(), histogram_data_aloc_codomain(), histogram_data_free_codomain()
*/

#include <stdint.h>
#include "signal.h"

#define HISTOGRAM_TRANSFORM_USE_LINEAR_SEARCH
//#define HISTOGRAM_TRANSFORM_USE_BINARY_SEARCH

typedef void (*transform_progress_callback_fn)(void* pData);
typedef struct {
    double progress;
} transform_progress_report_t;

typedef struct {
    double domain_min; // Minimum signal-for-histogram value
    double domain_max; // Maximum signal-for-histogram value
    ////double interval_length;
    uint64_t num_intervals;
    uint64_t* codomain_values;
} histogram_data_t;

typedef struct {
    // Determines the number of steps when constructing the Walsh-Hadamard matrix
    uint64_t m; 
} walsh_hadamard_config_t;

typedef struct {

} dft_config_t;

typedef enum {
    TRANSFORM_TYPE_DFT,
    TRANSFORM_TYPE_WALSH_HADAMARD
} transform_type;

typedef enum {
    TRANSFORM_COMPUTATION_MODE_NAIVE,
    TRANSFORM_COMPUTATION_MODE_FAST
} transform_computation_mode;

typedef enum {
    TRANSFORM_DIRECTION_FORWARD,
    TRANSFORM_DIRECTION_REVERSE
} transform_direction;

typedef struct {
    transform_type transformType;
    transform_computation_mode computationMode;
    transform_direction direction;
    union {
        dft_config_t dftConfig;
        walsh_hadamard_config_t whConfig;
    };
} transform_common_config_t;

typedef histogram_data_t* pHistogram_data_t;

void histogram_data_aloc_codomain (pHistogram_data_t pHistogramData);
void histogram_data_free_codomain (pHistogram_data_t pHistogramData);

/**
 * Transforms real signal to histogram data by switching from the time domain to the amplitude domain
 * @note You need to call `histogram_data_free_codomain()` after the output struct is no longer used
*/
histogram_data_t rsignal_to_histogram_transform(real_signal_t* pRealSignal, uint64_t numIntervals);


double* transform_generate_matrix_walsh_hadamard_recursive(uint64_t m);
double* transform_generate_matrix_walsh_hadamard_normalized_recursive(uint64_t m);

uint64_t transform_bits_reverse(uint64_t x, uint64_t m);

complex_signal_t transform_dft_real_naive(real_signal_t* pRealSignal);
complex_signal_t transform_idft_complex_naive(complex_signal_t* pComplexSignal);
complex_signal_t transform_dft_real_fast_p2(real_signal_t* pRealSignal);
complex_signal_t transform_idft_complex_fast_p2(complex_signal_t* pComplexSignal);
real_signal_t transform_walsh_hadamard_real_naive(real_signal_t* pRealSignal, walsh_hadamard_config_t* pConfig);
real_signal_t transform_walsh_hadamard_unnormalized_real_fast(real_signal_t* pRealSignal, walsh_hadamard_config_t* pConfig);
real_signal_t transform_walsh_hadamard_real_fast(real_signal_t* pRealSignal, walsh_hadamard_config_t* pConfig);

void transform_set_progress_callback(transform_progress_callback_fn progressCallback);  // Set the callback function to be called during the transformation