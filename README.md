# fftw3cxx

[FFTW](http://www.fftw.org), the *Fastest Fourier Transform in the West*,
is a widely used C library for computing the discrete Fourier transform
(DFT) in multiple sizes and dimensions. The current major API version is 3.

fftw3cxx is a thin C++ interface wrapper to make FFTW3 a little more user
friendly in C++. It's goals are to:

 - provide a C++ `plan` template class for holding FFTW3 plans with:
    - the [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) idiom,
    - the [named constructor idiom](https://isocpp.org/wiki/faq/ctors#named-ctor-idiom) for plan creation methods, and
    - [reference counting](https://en.wikipedia.org/wiki/Reference_counting) of created plans
 - provide direct use of the C++ `std::complex<>` template types
   (in addition to the FFTW3 complex types)
 - consist of only a thin wrapper around the existing FFTW3 API,
   with only small inline forwarding functions
 - be wrapped in a single C++ namespace, `fftw3cxx`
 - be implemented in a one-file header-only library, `fftw3cxx.h`,
   that can be cherry-picked into projects using FFTW3 (or not)

FFTW3 uses a "huge second-order macro" to implement templates in the C
preprocessor, expanding this macro once for each supported type.
fftw3cxx wraps and forwards the complete FFTW3 API with C++ template types,
using a large macro in the style of `fftw3.h`.
C++ template specialization is used to implement each of the FFTW3 types.

| FFTW3 prefix |    C type     |    fftw3cxx prefix    |
|--------------|---------------|-----------------------|
|   `fftw_`    | `double`      | `fftw<double>::`      |
|   `fftwf_`   | `float`       | `fftw<float>::`       |
|   `fftwl_`   | `long double` | `fftw<long double>::` |

This re-prefixed API isn't necessarily intended for direct use, but is a
helpful step toward the desired fftw3cxx API: the `plan` class.
With this re-prefixed API, the desired `plan` class is a straightforward
template class with inline forwarder methods. All of the FFTW3 functions
that want to be plan methods are included in `class fftw3cxx::plan<T>`:
 - plan creation named constructors: `plan_*`, e.g. `plan_dft_1d`
 - plan execution: `execute_*`, e.g. `execute_dft`
 - plan utilities: `print`, `flops`, etc.

The remaining functions in the FFTW3 API are template functions directly
in the fftw3cxx namespace, e.g. `fftw3cxx::cleanup<T>()`,
`fftw3cxx::import_wisdom_from_filename<T>(...)`, etc.

Because fftw3cxx is only a thin wrapper around the FFTW3 API, no documentation
is provided or needed. Refer directly to the FFTW3 function with the same name.
