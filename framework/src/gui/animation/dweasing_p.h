#pragma once
/*
Disclaimer for Robert Penner's Easing Equations license:

TERMS OF USE - EASING EQUATIONS

Open source under the BSD License.

Copyright @ 2001 Robert Penner
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*
NB! THE SOURCE FILE IS SHIFT FROM QT 4.7.3
*/
#include <math.h>
#include "dwgraphics/dwmatrix.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 (M_PI / 2)
#endif

/**
 * Easing equation function for a simple linear tweening, with no easing.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeNone(DwReal progress)
{
    return progress;
}

/**
 * Easing equation function for a quadratic (t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInQuad(DwReal t)
{
    return t*t;
}

/**
* Easing equation function for a quadratic (t^2) easing out: decelerating to zero velocity.
*
* @param t		Current time (in frames or seconds).
* @return		The correct value.
*/
static DwReal easeOutQuad(DwReal t)
{
    return -t*(t-2);
}

/**
 * Easing equation function for a quadratic (t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInOutQuad(DwReal t)
{
    t*=2.0;
    if (t < 1)
    {
        return t*t/DwReal(2);
    }
    else
    {
        --t;
        return -0.5 * (t*(t-2) - 1);
    }
}

/**
 * Easing equation function for a quadratic (t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutInQuad(DwReal t)
{
    if (t < 0.5) return easeOutQuad (t*2)/2;
    return easeInQuad((2*t)-1)/2 + 0.5;
}

/**
 * Easing equation function for a cubic (t^3) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInCubic(DwReal t)
{
    return t*t*t;
}

/**
 * Easing equation function for a cubic (t^3) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutCubic(DwReal t)
{
    t-=1.0;
    return t*t*t + 1;
}

/**
 * Easing equation function for a cubic (t^3) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInOutCubic(DwReal t)
{
    t*=2.0;
    if(t < 1)
    {
        return 0.5*t*t*t;
    }
    else
    {
        t -= DwReal(2.0);
        return 0.5*(t*t*t + 2);
    }
}

/**
 * Easing equation function for a cubic (t^3) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutInCubic(DwReal t)
{
    if (t < 0.5) return easeOutCubic (2*t)/2;
    return easeInCubic(2*t - 1)/2 + 0.5;
}

/**
 * Easing equation function for a quartic (t^4) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInQuart(DwReal t)
{
    return t*t*t*t;
}

/**
 * Easing equation function for a quartic (t^4) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutQuart(DwReal t)
{
    t-= DwReal(1.0);
    return - (t*t*t*t- 1);
}

/**
 * Easing equation function for a quartic (t^4) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInOutQuart(DwReal t)
{
    t*=2;
    if (t < 1) return 0.5*t*t*t*t;
    else
    {
        t -= 2.0f;
        return -0.5 * (t*t*t*t- 2);
    }
}

/**
 * Easing equation function for a quartic (t^4) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutInQuart(DwReal t)
{
    if (t < 0.5) return easeOutQuart (2*t)/2;
    return easeInQuart(2*t-1)/2 + 0.5;
}

/**
 * Easing equation function for a quintic (t^5) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInQuint(DwReal t)
{
    return t*t*t*t*t;
}

/**
 * Easing equation function for a quintic (t^5) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutQuint(DwReal t)
{
    t-=1.0;
    return t*t*t*t*t + 1;
}

/**
 * Easing equation function for a quintic (t^5) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInOutQuint(DwReal t)
{
    t*=2.0;
    if (t < 1) return 0.5*t*t*t*t*t;
    else
    {
        t -= 2.0;
        return 0.5*(t*t*t*t*t + 2);
    }
}

/**
 * Easing equation function for a quintic (t^5) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutInQuint(DwReal t)
{
    if (t < 0.5) return easeOutQuint (2*t)/2;
    return easeInQuint(2*t - 1)/2 + 0.5;
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInSine(DwReal t)
{
    return (t == 1.0) ? 1.0 : -dwCos(t * M_PI_2) + 1.0;
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutSine(DwReal t)
{
    return dwSin(t* M_PI_2);
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInOutSine(DwReal t)
{
    return -0.5 * (dwCos(M_PI*t) - 1);
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutInSine(DwReal t)
{
    if (t < 0.5) return easeOutSine (2*t)/2;
    return easeInSine(2*t - 1)/2 + 0.5;
}

/**
 * Easing equation function for an exponential (2^t) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInExpo(DwReal t)
{
    return (t==0 || t == 1.0) ? t : dwPow((DwReal)2.0, 10 * (t - 1)) - DwReal(0.001);
}

/**
 * Easing equation function for an exponential (2^t) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutExpo(DwReal t)
{
    return (t==1.0) ? 1.0 : 1.001 * (-dwPow((DwReal)2.0f, (DwReal)(-10 * t)) + 1);
}

/**
 * Easing equation function for an exponential (2^t) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInOutExpo(DwReal t)
{
    if (t==0.0) return DwReal(0.0);
    if (t==1.0) return DwReal(1.0);
    t*=2.0;
    if (t < 1) return 0.5 * dwPow(DwReal(2.0), 10 * (t - 1)) - 0.0005;
    return 0.5 * 1.0005 * (-dwPow(DwReal(2.0), -10 * (t - 1)) + 2);
}

/**
 * Easing equation function for an exponential (2^t) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutInExpo(DwReal t)
{
    if (t < 0.5) return easeOutExpo (2*t)/2;
    return easeInExpo(2*t - 1)/2 + 0.5;
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInCirc(DwReal t)
{
    return -(dwSqrt(1 - t*t) - 1);
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutCirc(DwReal t)
{
    t-= DwReal(1.0);
    return dwSqrt(1 - t* t);
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeInOutCirc(DwReal t)
{
    t*=DwReal(2.0);
    if (t < 1)
    {
        return -0.5 * (dwSqrt(1 - t*t) - 1);
    }
    else
    {
        t -= DwReal(2.0);
        return 0.5 * (dwSqrt(1 - t*t) + 1);
    }
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static DwReal easeOutInCirc(DwReal t)
{
    if (t < 0.5) return easeOutCirc (2*t)/2;
    return easeInCirc(2*t - 1)/2 + 0.5;
}

static DwReal easeInElastic_helper(DwReal t, DwReal b, DwReal c, DwReal d, DwReal a, DwReal p)
{
    if (t==0) return b;
    DwReal t_adj = (DwReal)t / (DwReal)d;
    if (t_adj==1) return b+c;

    DwReal s;
    if(a < dwFabs(c))
    {
        a = c;
        s = p / 4.0f;
    }
    else
    {
        s = p / (2 * M_PI) * dwAsin(c / a);
    }

    t_adj -= 1.0f;
    return -(a*dwPow((DwReal)2.0f,10*t_adj) * dwSin( (t_adj*d-s)*(2*M_PI)/p )) + b;
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static DwReal easeInElastic(DwReal t, DwReal a, DwReal p)
{
    return easeInElastic_helper(t, 0, 1, 1, a, p);
}

static DwReal easeOutElastic_helper(DwReal t, DwReal /*b*/, DwReal c, DwReal /*d*/, DwReal a, DwReal p)
{
    if (t==0) return 0;
    if (t==1) return c;

    DwReal s;
    if(a < c)
    {
        a = c;
        s = p / 4.0f;
    }
    else
    {
        s = p / (2 * M_PI) * dwAsin(c / a);
    }

    return (a*dwPow((DwReal)2.0f,-10*t) * dwSin( (t-s)*(2*M_PI)/p ) + c);
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static DwReal easeOutElastic(DwReal t, DwReal a, DwReal p)
{
    return easeOutElastic_helper(t, 0, 1, 1, a, p);
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static DwReal easeInOutElastic(DwReal t, DwReal a, DwReal p)
{
    if (t==0) return 0.0;
    t*=2.0;
    if (t==2) return 1.0;

    DwReal s;
    if(a < 1.0)
    {
        a = 1.0;
        s = p / 4.0f;
    }
    else
    {
        s = p / (2 * M_PI) * dwAsin(1.0 / a);
    }

    if (t < 1) return -.5*(a*dwPow((DwReal)2.0f,10*(t-1)) * dwSin( (t-1-s)*(2*M_PI)/p ));
    return a*dwPow((DwReal)2.0f,-10*(t-1)) * dwSin( (t-1-s)*(2*M_PI)/p )*.5 + 1.0;
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static DwReal easeOutInElastic(DwReal t, DwReal a, DwReal p)
{
    if (t < 0.5) return easeOutElastic_helper(t*2, 0, 0.5, 1.0, a, p);
    return easeInElastic_helper(2*t - 1.0, 0.5, 0.5, 1.0, a, p);
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static DwReal easeInBack(DwReal t, DwReal s)
{
    return t*t*((s+1)*t - s);
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static DwReal easeOutBack(DwReal t, DwReal s)
{
    t-= DwReal(1.0);
    return t*t*((s+1)*t+ s) + 1;
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static DwReal easeInOutBack(DwReal t, DwReal s)
{
    t *= 2.0;
    if (t < 1)
    {
        s *= 1.525f;
        return 0.5*(t*t*((s+1)*t - s));
    }
    else
    {
        t -= 2;
        s *= 1.525f;
        return 0.5*(t*t*((s+1)*t+ s) + 2);
    }
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static DwReal easeOutInBack(DwReal t, DwReal s)
{
    if (t < 0.5) return easeOutBack (2*t, s)/2;
    return easeInBack(2*t - 1, s)/2 + 0.5;
}

static DwReal easeOutBounce_helper(DwReal t, DwReal c, DwReal a)
{
    if (t == 1.0) return c;
    if (t < (4/11.0))
    {
        return c*(7.5625*t*t);
    }
    else if (t < (8/11.0))
    {
        t -= (6/11.0);
        return -a * (1. - (7.5625*t*t + .75)) + c;
    }
    else if (t < (10/11.0))
    {
        t -= (9/11.0);
        return -a * (1. - (7.5625*t*t + .9375)) + c;
    }
    else
    {
        t -= (21/22.0);
        return -a * (1. - (7.5625*t*t + .984375)) + c;
    }
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static DwReal easeOutBounce(DwReal t, DwReal a)
{
    return easeOutBounce_helper(t, 1, a);
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static DwReal easeInBounce(DwReal t, DwReal a)
{
    return 1.0 - easeOutBounce_helper(1.0-t, 1.0, a);
}


/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static DwReal easeInOutBounce(DwReal t, DwReal a)
{
    if (t < 0.5) return easeInBounce (2*t, a)/2;
    else return (t == 1.0) ? 1.0 : easeOutBounce (2*t - 1, a)/2 + 0.5;
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static DwReal easeOutInBounce(DwReal t, DwReal a)
{
    if (t < 0.5) return easeOutBounce_helper(t*2, 0.5, a);
    return 1.0 - easeOutBounce_helper (2.0-2*t, 0.5, a);
}

static inline DwReal dw_sinProgress(DwReal value)
{
    return dwSin((value * M_PI) - M_PI_2) / 2 + DwReal(0.5);
}

static inline DwReal dw_smoothBeginEndMixFactor(DwReal value)
{
    return dwMin(dwMax(1 - value * 2 + DwReal(0.3), DwReal(0.0)), DwReal(1.0));
}

// SmoothBegin blends Smooth and Linear Interpolation.
// Progress 0 - 0.3      : Smooth only
// Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
// Progress ~ 0.5  - 1   : Linear only

/**
 * Easing function that starts growing slowly, then increases in speed. At the end of the curve the speed will be constant.
 */
static DwReal easeInCurve(DwReal t)
{
    const DwReal sinProgress = dw_sinProgress(t);
    const DwReal mix = dw_smoothBeginEndMixFactor(t);
    return sinProgress * mix + t * (1 - mix);
}

/**
 * Easing function that starts growing steadily, then ends slowly. The speed will be constant at the beginning of the curve.
 */
static DwReal easeOutCurve(DwReal t)
{
    const DwReal sinProgress = dw_sinProgress(t);
    const DwReal mix = dw_smoothBeginEndMixFactor(1 - t);
    return sinProgress * mix + t * (1 - mix);
}

/**
 * Easing function where the value grows sinusoidally. Note that the calculated  end value will be 0 rather than 1.
 */
static DwReal easeSineCurve(DwReal t)
{
    return (dwSin(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}

/**
 * Easing function where the value grows cosinusoidally. Note that the calculated start value will be 0.5 and the end value will be 0.5
 * contrary to the usual 0 to 1 easing curve.
 */
static DwReal easeCosineCurve(DwReal t)
{
    return (dwCos(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}