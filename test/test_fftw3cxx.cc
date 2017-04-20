/*
 *  Copyright (c) 2017 Gregory E. Allen
 *  
 *  This is free and unencumbered software released into the public domain.
 *  
 *  Anyone is free to copy, modify, publish, use, compile, sell, or
 *  distribute this software, either in source code form or as a compiled
 *  binary, for any purpose, commercial or non-commercial, and by any
 *  means.
 *  
 *  In jurisdictions that recognize copyright laws, the author or authors
 *  of this software dedicate any and all copyright interest in the
 *  software to the public domain. We make this dedication for the benefit
 *  of the public at large and to the detriment of our heirs and
 *  successors. We intend this dedication to be an overt act of
 *  relinquishment in perpetuity of all present and future rights to this
 *  software under copyright law.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "fftw3cxx.hh"

template <typename T>
int test_fftw3cxx(const int N)
{
    fftw3cxx::plan<T> p;
    if (!p.empty()) return -1;

    try {
        p.execute();
        return -1;
    } catch (const std::exception &ex) {
        std::cout << "expected exception: " << ex.what() << std::endl;
    }

    typedef typename fftw3cxx::fftw<T>::complex complex;
#if 0
    complex *inc = fftw3cxx::alloc_complex<T>(N*N*N);   // new in fftw-3.3
    complex *outc = fftw3cxx::alloc_complex<T>(N*N*N);
#else
    complex *inc = (complex*)fftw3cxx::malloc<T>(N*N*N*sizeof(T)*2);
    complex *outc = (complex*)fftw3cxx::malloc<T>(N*N*N*sizeof(T)*2);
#endif
    std::complex<T> *in = (std::complex<T>*)inc;
    std::complex<T> *out = (std::complex<T>*)outc;

    const int rank = 2;
    int Ns[rank] = { N, N };
    p = fftw3cxx::plan<T>::plan_dft(rank, Ns, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    p.execute();
    p = fftw3cxx::plan<T>::plan_dft(rank, Ns, inc, outc, FFTW_FORWARD, FFTW_ESTIMATE);
    p.execute_dft(in, out);
    p.execute_dft(inc, outc);

    if (p.empty()) return -1;

    fftw3cxx::plan<T> p2 = p;

    p.clear();
    if (!p.empty()) return -1;

    if (p2.empty()) return -1;

    p = fftw3cxx::plan<T>::plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    p.execute_dft(inc, outc);
    p = fftw3cxx::plan<T>::plan_dft_1d(N, inc, outc, FFTW_FORWARD, FFTW_ESTIMATE);
    p.execute_dft(in, out);

    printf("plan: ");
    p.print();
    printf("\n");
    printf("estimate_cost: %f\n", p.estimate_cost());

    p = fftw3cxx::plan<T>::plan_dft_2d(N, N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    p.execute_dft(inc, outc);
    p = fftw3cxx::plan<T>::plan_dft_2d(N, N, inc, outc, FFTW_FORWARD, FFTW_ESTIMATE);
    p.execute_dft(in, out);

    p = fftw3cxx::plan<T>::plan_dft_3d(N, N, N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    p.execute_dft(inc, outc);
    p = fftw3cxx::plan<T>::plan_dft_3d(N, N, N, inc, outc, FFTW_FORWARD, FFTW_ESTIMATE);
    p.execute_dft(in, out);

    std::vector<T> r1v(N*N*(N+2));
    T* r1 = &r1v.front();

    fftw3cxx::plan<T> r2c;
    r2c = fftw3cxx::plan<T>::plan_dft_r2c(rank, Ns, r1, out, FFTW_ESTIMATE);
    r2c.execute();
    r2c = fftw3cxx::plan<T>::plan_dft_r2c(rank, Ns, r1, outc, FFTW_ESTIMATE);
    r2c.execute_dft_r2c(r1, out);
    r2c.execute_dft_r2c(r1, outc);

    r2c = fftw3cxx::plan<T>::plan_dft_r2c_1d(N, r1, out, FFTW_ESTIMATE);
    r2c.execute();
    r2c.execute_dft_r2c(r1, outc);

    r2c = fftw3cxx::plan<T>::plan_dft_r2c_2d(N, N, r1, out, FFTW_ESTIMATE);
    r2c.execute();
    r2c.execute_dft_r2c(r1, outc);

    r2c = fftw3cxx::plan<T>::plan_dft_r2c_3d(N, N, N, r1, out, FFTW_ESTIMATE);
    r2c.execute();
    r2c.execute_dft_r2c(r1, outc);

    fftw3cxx::plan<T> c2r;
    c2r = fftw3cxx::plan<T>::plan_dft_c2r(rank, Ns, in, r1, FFTW_ESTIMATE);
    c2r.execute();
    c2r = fftw3cxx::plan<T>::plan_dft_c2r(rank, Ns, inc, r1, FFTW_ESTIMATE);
    c2r.execute_dft_c2r(in, r1);
    c2r.execute_dft_c2r(inc, r1);

    c2r = fftw3cxx::plan<T>::plan_dft_c2r_1d(N, in, r1, FFTW_ESTIMATE);
    c2r.execute();
    c2r.execute_dft_c2r(inc, r1);

    c2r = fftw3cxx::plan<T>::plan_dft_c2r_2d(N, N, in, r1, FFTW_ESTIMATE);
    c2r.execute();
    c2r.execute_dft_c2r(inc, r1);

    c2r = fftw3cxx::plan<T>::plan_dft_c2r_3d(N, N, N, in, r1, FFTW_ESTIMATE);
    c2r.execute();
    c2r.execute_dft_c2r(inc, r1);

    std::vector<T> r2v(N*N*(N+2));
    T* r2 = &r2v.front();

    typename fftw3cxx::fftw<T>::r2r_kind kinds[rank] = { FFTW_R2HC, FFTW_R2HC };
    typename fftw3cxx::fftw<T>::r2r_kind kind = kinds[0];

    fftw3cxx::plan<T> r2r;
    r2r = fftw3cxx::plan<T>::plan_r2r(rank, Ns, r1, r2, kinds, FFTW_ESTIMATE);
    r2r.execute();
    r2r.execute_r2r(r1, r2);

    r2r = fftw3cxx::plan<T>::plan_r2r_1d(N, r1, r2, kind, FFTW_ESTIMATE);
    r2r.execute();
    r2r.execute_r2r(r1, r2);

    r2r = fftw3cxx::plan<T>::plan_r2r_2d(N, N, r1, r2, kind, kind, FFTW_ESTIMATE);
    r2r.execute();
    r2r.execute_r2r(r1, r2);

    r2r = fftw3cxx::plan<T>::plan_r2r_3d(N, N, N, r1, r2, kind, kind, kind, FFTW_ESTIMATE);
    r2r.execute();
    r2r.execute_r2r(r1, r2);

    fftw3cxx::free<T>(inc);
    fftw3cxx::free<T>(outc);

    return 0;
}

int main(int argc, const char* argv[]) {
    int res = 0;
    long nfft = 128;
    if (argc>1) {
        nfft = atol(argv[1]);
    }

#ifdef FFTW3CXX_TEST_FFTW3F
    printf("- test_fftw3cxx<float>(%ld);\n", nfft);
    res = test_fftw3cxx<float>(nfft);
    if (res) return res;
#endif

#ifdef FFTW3CXX_TEST_FFTW3
    printf("- test_fftw3cxx<double>(%ld);\n", nfft);
    res = test_fftw3cxx<double>(nfft);
    if (res) return res;
#endif

#ifdef FFTW3CXX_TEST_FFTW3L
    printf("- test_fftw3cxx<long double>(%ld);\n", nfft);
    res = test_fftw3cxx<long double>(nfft);
#endif

    return res;
}

