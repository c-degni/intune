#include "Fft.h"

complex_vector FFT(std::vector<double> inputSignal) {
    // Transform the input to be its complex counterpart (necessary for FFT algorithm)
    complex_vector complexInput;
    for (double &amp : inputSignal) {
        complexInput.push_back({amp, 0.0});
    }

    // Recursively compute FFT
    return recFFT(complexInput);
}

void splitPolynomialByEvenOddDegrees(complex_vector &p, complex_vector &pEven, complex_vector &pOdd) {
    for (int i = 0; i < p.size() / 2; i++) {
        pEven.push_back(p[i * 2]);
        pOdd.push_back(p[i * 2 + 1]);
    }
}

complex_vector recFFT(complex_vector p) {
    size_t n = p.size();

    // Only have one element meaning p is a degree 0 polynomial (constant)
    if (n <= 1) return p;
    
    // Split polynomial into even and odd degree terms
    complex_number w = {cos(2 * M_PI / n), sin(2 * M_PI / n)};
    complex_vector pEven;
    complex_vector pOdd;
    splitPolynomialByEvenOddDegrees(p, pEven, pOdd);

    // Recurse down, further splitting p
    complex_vector yEven = recFFT(pEven);
    complex_vector yOdd = recFFT(pOdd);

    // Build up the value representation of p using pEven and pOdd
    complex_vector y(n);
    for (int j = 0; j < n / 2; j++) {
        // w is jth root of unity
        complex_number w = {cos(2 * M_PI * j / n), sin(2 * M_PI * j / n)};

        y[j] = yEven[j] + w * yOdd[j];
        y[j + n / 2] = yEven[j] - w * yOdd[j];
    }

    return y;
}