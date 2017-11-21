/****************************************************************************
 *  Contents: huge (12MB) model for bi-level images, 3rd revision           *
 *  (C) 2002 by Dmitry Shkarin, e-mail: dmitry.shkarin@mtu-net.ru           *
 *  Results for CCITT images:                                               *
 *                    PWC    gPPM   JBIG1 ³ JBIG2     JB2³   bCdr           *
 *        F01_200   12683   12216   12728 ³ 12858    9606³  11228           *
 *        F02_200    7734    7203    7910 ³  7882    8697³   6774           *
 *        F03_200   19502   18375   19896 ³ 19404   16820³  16741           *
 *        F04_200   48469   45970   48854 ³ 46321   29480³  39015           *
 *        F05_200   22655   21425   23115 ³ 22569   18230³  18829           *
 *        F06_200   11501   10633   11646 ³ 11680   13011³   9918           *
 *        F07_200   51093   50057   52214 ³ 50958   38295³  45697           *
 *        F08_200   12940   11998   13158 ³ 12850   14504³  11261           *
 *        F10_200   59786   48198   63501 ³ 51531   58624³  43539           *
 *                 246363  226075  253022 ³236053  207267³ 203002           *
 ****************************************************************************/
#include <string.h>
//#include "model.h"
#pragma hdrstop
#include "coder.hpp"

#pragma pack(1)
struct BIN_CODER {
enum { STEP=10, INI_M_F=72*STEP, MAX_FREQ=16384-10*STEP };
    void initParent() { s[0]=s[1]=  STEP/2; MaxFreq=INI_M_F/4; }
    void initOCdr()   { s[0]=s[1]=3*STEP/2; MaxFreq=4*INI_M_F; }
    void initChild(BIN_CODER& bc) {         // Ja-ja, inheritance!
        UINT indx=s[1]-1, sf=bc.s[0]+bc.s[1];
        s[0]=(32*(bc.s[0]-1)+sf)/sf;        s[1]=(32*(bc.s[1]-1)+sf)/sf;
        s[indx] += STEP;                    MaxFreq=INI_M_F;
    }
    void scaleRare();
    void encode(UINT indx) {
        rcEncodeSymbol(s[0],s[1],indx);
        if (SummFreq > MaxFreq)             scaleRare();
        s[indx] += STEP;
    }
    void encodeYoung(BIN_CODER& bc,BIN_CODER& other,UINT indx);
    void encode(BIN_CODER& bc,BIN_CODER& other,UINT indx) {
        if ( MaxFreq ) {
            encode(indx);
            if (SummFreq < 12*STEP)         bc.s[indx] += 2*STEP/5;
        } else                              encodeYoung(bc,other,indx);
    }
    WORD s[2], MaxFreq;
};
#pragma pack()
void BIN_CODER::scaleRare()
{
    s[0] -= s[0] >> 1;                      s[1] -= s[1] >> 1;
    if (MaxFreq < MAX_FREQ)                 MaxFreq += 8*STEP;
}
void BIN_CODER::encodeYoung(BIN_CODER& bc,BIN_CODER& other,UINT indx)
{
    if ( s[1] ) {
        initChild(bc);                      encode(indx);
        bc.s[indx] += STEP;
    } else { bc.encode(indx);               other.s[1]=s[1]=1+indx; }
}

class MONO_MODELLER {
public:
    MONO_MODELLER(UINT XSize,UINT YSize);
    ~MONO_MODELLER();
    void          bmEncode(BYTE* Buf,FILE* fp);
protected:
    UINT         encodeRun(UINT ix);
    inline void     update();
    void         updateRun(UINT RLen);
    inline UINT getContext();
    void         startLine();
    UINT xs, ys, Cont0, Cont1, OldLR[3], I4Mask[16];
    BYTE OldVal[1 << 12], ns0, ns1, ns2, EOLMask;
    BYTE* LineVal[2], * pv0, * pvv, * RLBits;
    BIN_CODER BCdr1[1 << 8], BCdr2[1 << 21], RunCdr1[2], RunCdr2[1024], OCdr[320];
};
MONO_MODELLER::~MONO_MODELLER()
{
    delete LineVal[0];                      delete LineVal[1];
    delete RLBits;
}
MONO_MODELLER::MONO_MODELLER(UINT XSize,UINT YSize): xs(XSize), ys(YSize)
{
    UINT i, k;
    LineVal[0] = new BYTE[xs+16];           memset(LineVal[0],0,xs+16);
    LineVal[1] = new BYTE[xs+16];           memset(LineVal[1],0,xs+16);
    OldLR[0]=OldLR[1]=OldLR[2]=0;           RLBits = new BYTE[xs+1];
    for (RLBits[0]=i=k=0;i < xs;i++)        RLBits[i+1]=(k += (i == (2U << k)));
    RunCdr1[0].initParent();                RunCdr1[1].initParent();
    memset(RunCdr2,0,sizeof(RunCdr2));
    for (i=0;i < 320;i++)                   OCdr[i].initOCdr();
    memset(OldVal,1,sizeof(OldVal));        memset(BCdr2,0,sizeof(BCdr2));
    for (i=0;i < 256;i++)                   BCdr1[i].initParent();
    for (i=0;i < 16;i++)
        I4Mask[i]=((i & 0x08) >> 3)+((i & 0x04) << 6)+((i & 0x02) << 15)+
                    ((i & 0x01) << 24);
    EOLMask=((xs & 0x07) == 0)?(0xFF):(0xFF00 >> (xs & 0x07));
}
void MONO_MODELLER::startLine()
{
    pv0=LineVal[0]+8;                       pvv=LineVal[1]+8;
    for (UINT i=ns0=ns1=ns2=0;i < 6;i++) {
        ns1 += ns1+(pvv[i] & 1);            ns2 += ns2+((pvv[i] >> 1) & 1);
    }
}
inline void MONO_MODELLER::update()
{
    ns0 += ns0+pv0[ 0];                     pvv[0] += pvv[0]+pv0[ 0];
    OldVal[Cont0]=pv0[ 0];                  pv0++;  pvv++;
    ns1 += ns1+(pvv[5] & 1);                ns2 += ns2+((pvv[5] >> 1) & 1);
}
void MONO_MODELLER::updateRun(UINT RLen)
{
    UINT i=RLen;
    BYTE * p1=pv0, * p2=pvv;
    pv0 += RLen;                            pvv += RLen;
    do { *p2++ <<= 1;   *p1++ = 0; } while ( --i );
    ns0=64*p1[-7]+32*p1[-6]+16*p1[-5]+8*p1[-4];
    OldVal[Cont0]=0;
    OldVal[Cont0=(p2[0] & 0x0B)+256*(p2[1] & 0x07)]=0;
    ns1=4*(p2[3] & 1)+2*(p2[4] & 1)+(p2[5] & 1);
    ns2=4*(p2[2] & 2)+2*(p2[3] & 2)+(p2[4] & 2)+((p2[5] >> 1) & 1);
}
static UINT GetNZ(BYTE* Start,UINT MaxNZ)   // counts number of zeroed bytes
{
    BYTE tmp=Start[MaxNZ];                  Start[MaxNZ]=1;
    const BYTE* p=Start;
    if ((UINT(p) & 3) != 0 && !*p && (UINT(++p) & 3) != 0 && !*p)
            p++;
    while (*(DWORD*) p == 0)                p += sizeof(DWORD);
    if (!*p && !*++p && !*++p)              p++;
    Start[MaxNZ]=tmp;                       return p-Start;
}
static UINT GetNZ(BYTE* Start,UINT MaxNZ,BYTE m) // counts number of zeroed bytes under mask m
{
    BYTE tmp=Start[MaxNZ];                  Start[MaxNZ]=m;
    const BYTE* p=Start;
    if ((UINT(p) & 3) != 0 && !(*p & m) && (UINT(++p) & 3) != 0 && !(*p & m))
            p++;
    DWORD dwm=(m << 0)+(m << 8)+(m << 16)+(m << 24);
    while ( !((*(DWORD*) p) & dwm) )        p += sizeof(DWORD);
    if (!(*p & m) && !(*++p & m) && !(*++p & m))
            p++;
    Start[MaxNZ]=tmp;                       return p-Start;
}
UINT MONO_MODELLER::encodeRun(UINT ix)      // for speed mainly
{
    UINT i, k, m1, m2, n, Cont, MaxRLen, RLen, LongRun;
    UINT j=16*(ix == 0);
    if (xs-ix > 3) {
        MaxRLen=GetNZ(pvv+5,xs-ix-3,0x03);  MaxRLen += 3-(pvv[5+MaxRLen] & 1);
    } else                                  MaxRLen=xs-ix;
    k=(GetNZ(pvv+2,MaxRLen-3,0x3C) == MaxRLen-3);
    i=RLBits[MaxRLen];                      n=(MaxRLen > 6)+(MaxRLen > 24);
    m1=pvv[MaxRLen+2] | pvv[MaxRLen+3] | pvv[MaxRLen+4] | pvv[MaxRLen+5];
    Cont=OldLR[n]+2*OldVal[Cont0]+4*(m1 & 0x03)+j+32*k+64*i;
    OldLR[n]=LongRun=((RLen=GetNZ(pv0,MaxRLen)) == MaxRLen);
    RunCdr2[Cont].encode(RunCdr1[Cont & 1],RunCdr2[Cont ^ 1],LongRun);
    if ( RLen )                             updateRun(RLen);
    if ( !LongRun )                         update();
    RC_ENC_NORMALIZE();
    if (LongRun || MaxRLen == 1)            return MaxRLen;
    k=1 << i;                               n=0;
    m1=~(2*k);                              m2=~(2*k+4*k);
    j += 64+32*(Cont & 1);
    do {
        if ((n | k) < MaxRLen) {
            OCdr[i+j+64*((n == 0)+((n & m1) == 0)+((n & m2) == 0))].encode((RLen & k) != 0);
            RC_ENC_NORMALIZE();
            n |= (RLen & k);                j &= 16+32;
        }
        i--;                                k >>= 1;
        m1 >>= 1;                           m2 >>= 1;
    } while ( k );
    return RLen+1;
}
inline UINT TestVal(UINT Val,UINT Val0) { return (Val-1U >= Val0-1U); }
inline UINT MONO_MODELLER::getContext()
{
    Cont1=(ns0 & 0x03)+(ns1 & 0xF0)+(pvv[-1] & 0x04)+8*OldVal[Cont0];
    UINT c=(ns0 & 0x0F)+16*(ns1 & 0xF8)+(ns2 & 0x70)+1024*(pvv[0] & 0x04)+
            8192*OldVal[Cont0];
    c += TestVal((ns1 & 0x10)+(ns2 & 0x08)+(pvv[ 3] & 0x04),0x10+0x08+0x04) << 14;
    c += TestVal((ns1 & 0x40)+(ns2 & 0x80)+(pvv[-3] & 0x08),0x40+0x80+0x08) << 15;
    c += TestVal(pvv[ 0] & 0xFC,0xFC) << 16;
    c += TestVal(ns0 & 0xF8,0xF8) << 17;
    c += TestVal((ns1 & 0x0E)+(ns2 & 0x04),0x0E+0x04) << 18;
    c += TestVal(pvv[-1] & 0x3C,0x3C) << 19;
    c += TestVal(pvv[ 1] & 0x1E,0x1E) << 20;
    return c;
}
void MONO_MODELLER::bmEncode(BYTE* Buf,FILE* fp)
{
    BYTE* p=Buf;                            rcInitEncoder(fp);
    for (UINT* p1, Cont2, ix, iy=ys; iy ;iy--) {
        startLine();
        ix=(xs+7) >> 3;                     p[ix-1] &= EOLMask;
        for (p1=(UINT*) pv0; ix ;ix--, p++, p1 += 2) {
            p1[0]=I4Mask[*p >> 4];          p1[1]=I4Mask[*p & 0x0F];
        }
        for (ix=0;ix < xs; ) {
            Cont0=(pvv[0] & 0x0B)+2*(pvv[-1] & 0x0A)+32*(pvv[-2] & 0x07)+
                    256*((pvv[1] & 0x07)+(ns1 & 0x08));
            if ((ns0 & 0x03)+(ns1 & 0x7F)+(ns2 & 0x7E)+(pvv[0] & 0x04)+
                                            (pvv[1] & 0x0C) != 0) {
                Cont2=getContext();
                BCdr2[Cont2].encode(BCdr1[Cont1],BCdr2[Cont2 ^ 8192],*pv0);
                update();                   ix++;
                RC_ENC_NORMALIZE();
            } else                          ix += encodeRun(ix);
        }
    }
    rcFlushEncoder();
}

void BMEncode(_BM* bm,FILE* fpOut)
{
    MONO_MODELLER* mm = new MONO_MODELLER(bm->XSize,bm->YSize);
    mm->bmEncode(bm->buffer(),fpOut);
    delete mm;
}
