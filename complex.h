/*
 * complex.h - lean and mean complex vector
 * uses AVX2 unit
 *
 * Optimized 2018 by Joerg Dentler
 *
 */

#ifndef COMPLEX_H
#define COMPLEX_H

#include <immintrin.h>
#include <iostream>
#include <cmath>

#if !defined ( __AVX2__ )
#error "no AVX2 configuration"
#endif

class Complex
{
public:
    Complex()
    {
        m_v = _mm256_setr_pd(0.0, 0.0, 0.0, 0.0);
    }
    Complex(const double &real)
    {
        m_v = _mm256_setr_pd(real, 0.0, 0.0, 0.0);
    }
    Complex(const double &real, const double &imag)
    {
        m_v = _mm256_setr_pd(real, imag, 0.0, 0.0);
    }
    Complex(const Complex &c)
    {
       m_v = c.m_v;
    }
    Complex(const __m256d &v)
    {
        m_v = v;
    }
    double real() const
    {
        double* res = (double*)&m_v;
        return res[0];
    }
    double imag() const
    {
        double* res = (double*)&m_v;
        return res[1];
    }

    Complex operator + (const Complex &c) const
    {
        return _mm256_add_pd(m_v, c.m_v);
    }
    Complex operator - (const Complex &c) const
    {
        return _mm256_sub_pd(m_v, c.m_v);
    }
    Complex operator * (const double &scalar) const
    {
        __m256d v = _mm256_setr_pd(scalar, scalar, 0.0, 0.0);
        return _mm256_mul_pd(m_v, v);
    }
    Complex operator * (const Complex &c) const
    {
        __m256d neg = _mm256_setr_pd(1.0, -1.0, 1.0, -1.0);

        /* Step 1: Multiply vec1 and vec2 */
        __m256d vc3 = _mm256_mul_pd(m_v, c.m_v);

        /* Step 2: Switch the real and imaginary elements of vec2 */
        __m256d vc2 = _mm256_permute_pd(c.m_v, 0x5);

        /* Step 3: Negate the imaginary elements of vec2 */
        vc2 = _mm256_mul_pd(vc2, neg);

        /* Step 4: Multiply vec1 and the modified vec2 */
        __m256d vc4 = _mm256_mul_pd(m_v, vc2);

        /* Horizontally subtract the elements in vec3 and vec4 */
        return _mm256_hsub_pd(vc3, vc4);
    }
    /**
     * complex division
     * ((ac+bd)+i(bc-ad))/(c^2+d^2)
     */
    Complex operator / (const Complex &c) const
    {
        __m256d n1 = _mm256_mul_pd(m_v, c.m_v);
                n1 = _mm256_hadd_pd(n1, n1);
        __m256d n2 = _mm256_permute_pd(m_v, 0b00000001);
                n2 = _mm256_mul_pd(c.m_v, n2);
                n2 = _mm256_hsub_pd(n2, n2);
        n1 = _mm256_shuffle_pd(n1, n2, 0b0000);

        __m256d d = _mm256_mul_pd(c.m_v, c.m_v);
                d = _mm256_hadd_pd(d, d);
                d = _mm256_permute_pd(d, 0b00000000);
        return _mm256_div_pd(n1, d);
    }
    Complex operator / (const double &scalar) const
    {
        __m256d v = _mm256_setr_pd(scalar, scalar, 1.0, 1.0);
        return _mm256_div_pd(m_v, v);
    }
    Complex& operator +=(const Complex &c)
    {
        m_v = _mm256_add_pd(m_v, c.m_v);
        return *this;
    }
    Complex& operator -=(const Complex &c)
    {
        m_v = _mm256_sub_pd(m_v, c.m_v);
        return *this;
    }
    Complex& operator *=(const double &scalar)
    {
        __m256d v = _mm256_setr_pd(scalar, scalar, 0.0, 0.0);
        m_v = _mm256_mul_pd(m_v, v);
        return *this;
    }
    Complex& operator *=(const Complex &c)
    {
        __m256d neg = _mm256_setr_pd(1.0, -1.0, 1.0, -1.0);

        /* Step 1: Multiply vec1 and vec2 */
        __m256d vc3 = _mm256_mul_pd(m_v, c.m_v);

        /* Step 2: Switch the real and imaginary elements of vec2 */
        __m256d vc2 = _mm256_permute_pd(c.m_v, 0x5);

        /* Step 3: Negate the imaginary elements of vec2 */
        vc2 = _mm256_mul_pd(vc2, neg);

        /* Step 4: Multiply vec1 and the modified vec2 */
        __m256d vc4 = _mm256_mul_pd(m_v, vc2);

        /* Horizontally subtract the elements in vec3 and vec4 */
        m_v = _mm256_hsub_pd(vc3, vc4);

        return *this;
    }
    Complex& operator /=(const Complex &c)
    {
        __m256d n1 = _mm256_mul_pd(m_v, c.m_v);
                n1 = _mm256_hadd_pd(n1, n1);
        __m256d n2 = _mm256_permute_pd(m_v, 0b00000001);
                n2 = _mm256_mul_pd(c.m_v, n2);
                n2 = _mm256_hsub_pd(n2, n2);
        n1 = _mm256_shuffle_pd(n1, n2, 0b0000);

        __m256d d = _mm256_mul_pd(c.m_v, c.m_v);
                d = _mm256_hadd_pd(d, d);
                d = _mm256_permute_pd(d, 0b00000000);
        m_v = _mm256_div_pd(n1, d);
        return *this;
    }
    Complex& operator /=(const double &scalar)
    {
        __m256d v = _mm256_setr_pd(scalar, scalar, 1.0, 1.0);
        m_v = _mm256_div_pd(m_v, v);
        return *this;
    }
    Complex conj() const
    {
        __m256d v = _mm256_setr_pd(1.0, -1.0, 0.0, 0.0);
        return _mm256_mul_pd(m_v, v);
    }
    double abs() const
    {
        __m256d p = _mm256_mul_pd(m_v, m_v);
        __m256d s = _mm256_hadd_pd(p, p);
        p = _mm256_sqrt_pd(s);
        double* res = (double*)&p;
        return res[0];
    }
    double norm() const
    {
        __m256d p = _mm256_mul_pd(m_v, m_v);
        __m256d n = _mm256_hadd_pd(p, p);
        double* res = (double*)&n;
        return res[0];
    }
    bool operator ==(const Complex &c) const
    {
        double* v1 = (double*)&m_v;
        double* v2 = (double*)&c.m_v;
        return v1[0] == v2[0] && v1[1] == v2[1];
    }

    bool operator !=(const Complex &c) const
    {
        return !(*this == c);
    }

    void write(std::ostream& os) const
    {
        os << m_v[0] << " i" << m_v[1];
    }

protected:
    __m256d m_v;
};

#endif


