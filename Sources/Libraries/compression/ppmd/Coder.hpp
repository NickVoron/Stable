


static struct SUBRANGE {
    DWORD LowCount, HighCount, scale;
} SubRange;
enum { TOP=1 << 24, BOT=1 << 15 };
static DWORD low, code, range;

inline void ariInitEncoder()
{
    low=0;                                  range=DWORD(-1);
}
#define ARI_ENC_NORMALIZE(stream) {                                         \
    while ((low ^ (low+range)) < TOP || range < BOT &&                      \
            ((range= -low & (BOT-1)),1)) {                                  \
        _PPMD_E_PUTC(low >> 24,stream);                                     \
        range <<= 8;                        low <<= 8;                      \
    }                                                                       \
}
inline void ariEncodeSymbol()
{
    low += SubRange.LowCount*(range /= SubRange.scale);
    range *= SubRange.HighCount-SubRange.LowCount;
}
inline void ariShiftEncodeSymbol(UINT SHIFT)
{
    low += SubRange.LowCount*(range >>= SHIFT);
    range *= SubRange.HighCount-SubRange.LowCount;
}
#define ARI_FLUSH_ENCODER(stream) {                                         \
    for (UINT i=0;i < 4;i++) {                                              \
        _PPMD_E_PUTC(low >> 24,stream);                 low <<= 8;          \
    }                                                                       \
}
#define ARI_INIT_DECODER(stream) {                                          \
    low=code=0;                             range=DWORD(-1);                \
    for (UINT i=0;i < 4;i++)                                                \
            code=(code << 8) | _PPMD_D_GETC(stream);                        \
}
#define ARI_DEC_NORMALIZE(stream) {                                         \
    while ((low ^ (low+range)) < TOP || range < BOT &&                      \
            ((range= -low & (BOT-1)),1)) {                                  \
        code=(code << 8) | _PPMD_D_GETC(stream);                            \
        range <<= 8;                        low <<= 8;                      \
    }                                                                       \
}
inline UINT ariGetCurrentCount() {
    return (code-low)/(range /= SubRange.scale);
}
inline UINT ariGetCurrentShiftCount(UINT SHIFT) {
    return (code-low)/(range >>= SHIFT);
}
inline void ariRemoveSubrange()
{
    low += range*SubRange.LowCount;
    range *= SubRange.HighCount-SubRange.LowCount;
}
