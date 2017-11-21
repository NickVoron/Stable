#define SOFT_SKIN
#define EYE_SHARPEN
#define TEETH_WHITENING
#define EYE_WHITENING
// #define NOMASK

#define SOFT_SKIN_RADIUS 0.3

uniform float uniTexRatioXY;

uniform float uniSharpenIntensity;
uniform float uniEyeWhiteIntensity;
uniform float uniSoftIntensity;

varying vec2 maskTexCoord;
varying vec2 originalImageCoord;

varying vec2 texCoord;
varying vec4 texCoord0;
varying vec4 texCoord1;
varying vec4 texCoord2;
varying vec4 texCoord3;

#ifdef VERTEX_SHADER

attribute vec2 atbCoord;
attribute vec2 atbMaskTexCoord;

void main(void)
{
    vec2 delta = vec2(1.0 / 1280.0, 1.0 / 1280.0); 

    originalImageCoord = atbCoord * 0.5 + 0.5;
    maskTexCoord = vec2(atbMaskTexCoord.x, 1.0 - atbMaskTexCoord.y);
    
    //Soft skin coords
    texCoord = vec2(0.5 * atbCoord + 0.5);   
    texCoord0.xy = texCoord + vec2(-0.00694444, -0.00390625);
    texCoord1.xy = texCoord + vec2(-0.00694444, 0.00546875);
    texCoord2.xy = texCoord + vec2(0.00972222, -0.00390625);
    texCoord3.xy = texCoord + vec2(0.00972222, 0.00546875);

    //Sharpening coords
    texCoord0.zw = texCoord + vec2(-delta.x, -delta.y);
    texCoord1.zw = texCoord + vec2(delta.x, -delta.y);
    texCoord2.zw = texCoord + vec2(-delta.x, delta.y);
    texCoord3.zw = texCoord + vec2(delta.x, delta.y);
    gl_Position = vec4(atbCoord, 0.0, 1.0);
}

#endif


#ifdef FRAGMENT_SHADER

uniform sampler2D uniTexture;
uniform sampler2D uniOriginalImage;
uniform sampler2D uniMask;
uniform float uniAlpha;


#include "shaders/common/definedBlendMode.glsl"

vec4 mapColor(vec4 orgColor)
{
    const float EPS = 0.000001;
    const float pxSize = 512.0;
    
    float bValue = (orgColor.b * 255.0) / 4.0;
    
    vec2 mulB = clamp(floor(bValue) + vec2(0.0, 1.0), 0.0, 63.0);
    vec2 row = floor(mulB / 8.0 + EPS);
    vec4 row_col = vec4(row, mulB - row * 8.0);
    vec4 lookup = orgColor.ggrr * (63.0/pxSize) + row_col * (64.0/pxSize) + (0.5/pxSize);

    float b1w = bValue - mulB.x;
    
    vec3 sampled1 = texture2D(uniTexture, lookup.zx).rgb;
    vec3 sampled2 = texture2D(uniTexture, lookup.wy).rgb;
    
    vec3 res = mix(sampled1, sampled2, b1w);
    return vec4(res, orgColor.a);
}

vec4 whitening(vec4 originalColor, float factor) {
    vec4 color = mapColor(originalColor);
    return mix(originalColor, color, factor);
}

vec4 sharpen(vec4 originalColor, float factor) {
    vec4 accum = 5.0 * originalColor;
    accum -= texture2D(uniOriginalImage, texCoord0.zw);
    accum -= texture2D(uniOriginalImage, texCoord1.zw);
    accum -= texture2D(uniOriginalImage, texCoord2.zw);
    accum -= texture2D(uniOriginalImage, texCoord3.zw);
    vec4 result = mix(originalColor, accum, factor);
    return clamp(result, 0.0, 1.0);;
}

vec4 getLuminance4(mat4 color) {
    const vec4 rgb2y = vec4(0.299, 0.587, 0.114, 0.0);
    return rgb2y * color;
}

float getLuminance(vec4 color) {
    const vec4 rgb2y = vec4(0.299, 0.587, 0.114, 0.0);
    return dot(color, rgb2y);
}

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

vec4 getWeight(float intens, vec4 nextIntens) {
    vec4 lg = log(nextIntens/intens);
    lg *= lg;
    return exp(lg / (-2.0 * SOFT_SKIN_RADIUS * SOFT_SKIN_RADIUS));
}

vec4 softSkin(vec4 originalColor, float factor) {    
    vec4 screenColor = originalColor;
    float intens = getLuminance(screenColor);
    float sum = 1.0;
    
    mat4 nextColor;
    nextColor[0] = texture2D(uniOriginalImage, texCoord0.xy);
    nextColor[1] = texture2D(uniOriginalImage, texCoord1.xy);
    nextColor[2] = texture2D(uniOriginalImage, texCoord2.xy);
    nextColor[3] = texture2D(uniOriginalImage, texCoord3.xy);
    vec4 nextIntens = getLuminance4(nextColor);
    vec4 curr = 0.36787944 * getWeight(intens, nextIntens);
    sum += dot(curr, vec4(1.0));
    screenColor += nextColor * curr;
    
#ifdef GL_FRAGMENT_PRECISION_HIGH
    float noise = (rand(texCoord) - 0.5) / 30.0;
    screenColor = screenColor / sum + vec4(noise, noise, noise, 1.0);
#else
    screenColor = screenColor / sum;
#endif
    screenColor = mix(originalColor, screenColor, factor);
    return screenColor;
}

void main(void)
{
#ifndef NOMASK
    vec4 maskColor = texture2D(uniMask, maskTexCoord);
#else
    vec4 maskColor = vec4(1.0);
#endif
    vec4 res = texture2D(uniOriginalImage, originalImageCoord);
#ifdef SOFT_SKIN
    res = softSkin(res, maskColor.r * uniSoftIntensity);
#endif
#ifdef EYE_SHARPEN
    res = sharpen(res, maskColor.b * uniSharpenIntensity); 
#endif
    float whiteningAlpha = 0.0;
#ifdef EYE_WHITENING
    whiteningAlpha += maskColor.b * uniEyeWhiteIntensity;
#endif
#ifdef TEETH_WHITENING
    whiteningAlpha += maskColor.g * uniAlpha;
#endif
#if defined(EYE_WHITENING) || defined(TEETH_WHITENING)
    res = whitening(res, whiteningAlpha);
#endif

    gl_FragColor = res;
}

#endif
