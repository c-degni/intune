#include <vector>
#include <complex>
#include <cmath>

using complex_number = std::complex<double>;
using complex_vector = std::vector<std::complex<double>>;

// TODO: May want to generalize the definition to work make sense in all contexts of using FFT
/**
 * @param inputSignal A signal as a amplitude/time vector in its time domain. 
 * @return The input signal in it's frequency domain as a complex vector (COEFFCIENT => VALUE 
 * polynomial representation).
 */
complex_vector FFT(std::vector<double> inputSignal);

/**
 * @param p A coefficient complex vector representation of polynomial. 
 * @param pEven The complex vector to be populated with the coefficient representation of even degree terms of p.
 * @param pOdd The complex vector to be populated with the coefficient representation of odd degree terms of p.
 */
void splitPolynomialByEvenOddDegrees(complex_vector &p, complex_vector &pEven, complex_vector &pOdd);

/**
 * @param complexInput A signal as a complex vector in its time domain. 
 * @return The input signal in it's frequency domain as a complex vector.
 */
complex_vector recFFT(complex_vector complexInput);