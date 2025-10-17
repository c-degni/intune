#include <vector>
#include <complex>
#include <cmath>

using complex_number = std::complex<double>;
using complex_vector = std::vector<std::complex<double>>;

/**
 * Fast Fourier Transform (FFT) translates the coefficient representation of a polynomial into
 * its respective value representation.
 * 
 * @param p The coefficient representation of a polynomial p. 
 * @return The value representation of polynomial p.
 * 
 * @note In the context of signals, p is a signal as a amplitude/time vector in its time domain
 * and performing an FFT on p will result in the signal in it's frequency domain.
 */
complex_vector FFT(std::vector<double> p);

/**
 * This will split a coefficient represented polynomial into its even and odd degree terms.
 * 
 * @param p The coefficient representation of a polynomial p.  
 * @param pEven The complex vector to be populated with the coefficient representation of even degree terms of p.
 * @param pOdd The complex vector to be populated with the coefficient representation of odd degree terms of p.
 */
void splitPolynomialByEvenOddDegrees(complex_vector &p, complex_vector &pEven, complex_vector &pOdd);

/**
 * This is the recursive implementation of FFT, translating a polynomial in coefficient representation to 
 * its value representation.
 * 
 * @param p The coefficient representation of a polynomial p. 
 * @return The value representation of polynomial p.
 */
complex_vector recFFT(complex_vector p);