// Copying and distribution of this file, with or without modification,
// are permitted in any medium without royalty provided the copyright
// notice and this notice are preserved.  This file is offered as-is,
// without any warranty.

#ifndef _DECODE_IR_H
#define _DECODE_IR_H

#ifndef ARDUINO
//#define USE_JNI
#endif

#ifndef _MSC_VER
#define DecodeIR_API
#define _stdcall
#else
#define DecodeIR_API _stdcall
#endif

#ifdef _MSC_VER
#define int64_t __int64
#pragma warning(disable:4244) // Suppress security warnings conversion from double to float
#pragma warning(disable:4996) // Suppress warnings of deprecated functions
#endif

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifdef USE_JNI
typedef unsigned frequency_t;
typedef unsigned microseconds_t;
#else
typedef uint32_t frequency_t;
typedef uint16_t microseconds_t;
#endif

#define require(x) if (!(x)) { return; }

/*
const CString m_Notes[] = {
        "One frame.",
        "%i identical frames.",
        "%i frames with lead-in on only the first frame.",
        "Main frame with %i ditto frames.",
        "%i frames including distinctive start frame.",
        "%i frames including distinctive end frame.",
        "%i frames including distinctive start and end frames.",
        "%i main frames interleaved with auxiliary check frames.",
        "A generic decode of an unknown protocol",
        "One frame with lead-in.",
        "Main frame with one ditto frame.",
        "%i frames with extended lead-in on the first frame.",
        "Unknown protocol."};

const CString m_AuxNotes[] = {
        "  Incomplete signal: expected end frame missing.",
        "  Incomplete signal: expected start frame missing.",
        "  Incomplete signal: expected start and end frames missing.",
        "  Incomplete signal: main frame without check frame.",
        "  Incomplete signal: check frame without preceding main frame.",
        "  Incomplete signal: start frame with no following frames.",
        "  Incomplete signal: end frame with no preceding frames."};
 */

void DecodeIR_API DecodeIR
(uint32_t* Context,
        const microseconds_t * TpaiBursts,
        frequency_t TiFreq,
        uint16_t TiSingleBurstCount,
        uint16_t TiRepeatBurstCount,
        char* TsProtocol,
        int32_t* TiDevice, // Not working with unsigned (tested)
        int32_t* TiSubDevice, // Not working with unsigned (tested)
        int32_t* TiOBC, // Not working with unsigned (tested)
        int32_t TaiHex[4],
        char* TsMisc,
        char* TsError);

int32_t _stdcall ProtocolSupportLevel(char * TsProtocol);

void _stdcall EnumerateProtocols(int32_t iProtocolNumber, char* TsProtocol);

void _stdcall Version(char *Result);

typedef float* burst;

struct onLess {

    int16_t operator()(burst a, burst b) {
        if (a[0] != b[0])
            return ( a[0] < b[0]);
        if (a[1] != b[1])
            return ( a[1] < b[1]);
        return a < b;
    }
};

struct offLess {

    int16_t operator()(burst a, const burst b) {
        if (a[1] != b[1])
            return ( a[1] < b[1]);
        if (a[0] != b[0])
            return ( a[0] < b[0]);
        return a < b;
    }
};

struct totLess {

    int16_t operator()(burst a, burst b) {
        float d = a[0] + a[1] - (b[0] + b[1]);
        if (d > 0.0)
            return 0;
        if (d < 0.0)
            return 1;
        if (a[0] != b[0])
            return ( a[0] < b[0]);
        return a < b;
    }
};

class SetFloat {
private:
    static const unsigned int initialSize = 16U;
    float* data;
    unsigned int length;
    unsigned int size;
    void grow() {
        float* newData = new float[2*size];
        for (unsigned int i = 0; i < size; i++)
            newData[i] = data[i];
        delete [] data;
        data = newData;
        size *= 2;
    }
    
    unsigned int findInsert(float x) {
        for (unsigned int i = 0; i < length; i++) {
            if (data[i] > x)
                return i;
        }
        return length;
    }
    
    void move(unsigned int index) {
        if (length > 0)
            for (int i = length - 1; i >= (int) index; i--)
                data[i+1] = data[i];
    }
public:

    SetFloat() {
        data = new float[initialSize];
        length = 0;
        size = initialSize;
    };
    
    virtual ~SetFloat() {
        delete [] data; 
    };
    
    typedef float* iterator;

    void clear() {
        length = 0;
    };
    
    void insert(float x) {
        if (length == size)
            grow();
        unsigned int point = findInsert(x);
        move(point);
        data[point] = x;
        length++;
    }

    iterator begin() {
        return data;
    };

    iterator end() {
        return data + length;
    };
};

struct setBurst : public SetFloat {
    float min3;
    float min2;
    float min1;
    float mid1;
    float mid2;
    float max1;
    float max2;
    float max3;

    setBurst() {
        clearF();
    }

    void clear() {
        SetFloat::clear();
        clearF();
    }

    void clearF() {
        min3 = 1e9;
        min2 = 1e9;
        min1 = 1e9;
        max1 = 0;
        max2 = 0;
        max3 = 0;
    }

    void insert1(burst b) {
        double x = b[0] + b[1];
        SetFloat::insert( x );
        if (x < min3) {
            if (x >= min2) {
                min3 = x;
            } else {
                min3 = min2;
                if (x >= min1) {
                    min2 = x;
                } else {
                    min2 = min1;
                    min1 = x;
                }
            }
        }
        if (x > max3) {
            if (x <= max2) {
                max3 = x;
            } else {
                max3 = max2;
                if (x <= max1) {
                    max2 = x;
                } else {
                    max2 = max1;
                    max1 = x;
                }
            }
        }
    }

    void insert(burst b) {
        double x = b[0] + b[1];
        SetFloat::insert( x );
        bool redo = (x > mid1 && x < mid2);
        if (x < min3) {
            if (x >= min2) {
                min3 = x;
            } else {
                min3 = min2;
                double g = min1 - x;
                if (g <= 0) {
                    min2 = x;
                } else {
                    min2 = min1;
                    min1 = x;
                    if (g > mid2 - mid1) redo = true;
                }
            }
        }
        if (x > max3) {
            if (x <= max2) {
                max3 = x;
            } else {
                max3 = max2;
                double g = x - max1;
                if (g <= 0) {
                    max2 = x;
                } else {
                    max2 = max1;
                    max1 = x;
                    if (g > mid2 - mid1) redo = true;
                }
            }
        }
        if (redo) {
            findMid();
        }
    }

    void findMid() {
        iterator it = begin();
        iterator limit = end();
        float x = (*it);
        mid1 = x;
        mid2 = x;
        float m = 0;
        while (++it != limit) {
            float y = (*it);
            float z = y - x;
            if (z > m) {
                m = z;
                mid1 = x;
                mid2 = y;
            }
            x = y;
        }
    }

};

struct setOn {
    float min1;
    float max1;
    float max2;

    setOn() {
        clear();
    }

    void clear() {
        min1 = 1e9;
        max1 = 0;
        max2 = 0;
    }

    void insert(burst b) {
        double x = b[0];
        if (x < min1) min1 = x;
        if (x > max2) {
            if (x > max1) {
                max2 = max1;
                max1 = x;
                return;
            }
            max2 = x;
        }
    }
};

struct setOff {
    float min1;
    float max1;

    setOff() {
        clear();
    }

    void clear() {
        min1 = 1e9;
        max1 = 0;
    }

    void insert(burst b) {
        double x = b[0];
        if (x < min1) min1 = x;
        if (x > max1) max1 = x;
    }
};

enum {
    MinFrame = 2, MaxFrame = 100
};

struct Signal {
    Signal(
            uint32_t* p_Context,
            const microseconds_t* p_Bursts,
            frequency_t n_Freq,
            uint16_t n_Single,
            uint16_t n_Repeat,
            char* p_Protocol,
            int32_t* p_Device,
            int32_t* p_SubDevice,
            int32_t* p_OBC,
            int32_t* p_Hex,
            char* p_Misc,
            char* p_Error);
    ~Signal();

    void decode2(); // GD 2009
    void decode();
    void setPreempt(int32_t prValue); // GD 2009
    void setzContext(); // GD 2009
    uint32_t getFreq(int32_t start, int32_t end); // GD 2009
    void cleanup();
    int16_t phaseBit();
    int16_t phaseBit2();
    bool framed(float nGap);
    bool framedLeft(float nGap);

    bool unmodulated(float maxOn) {
        return nFreq * maxOn < 500000.;
    }
    uint32_t getMsb(uint32_t first, uint32_t bits);
    uint32_t getLsb(uint32_t first, uint32_t bits);

    uint8_t getBit(uint32_t n) {
        return cBits[n >> 3]&(1 << (n & 7));
    }
    uint32_t msb(uint32_t val, uint32_t bits);
    void makeMsb();
    void decodeX(int32_t nCount);
    void decodeX2(int32_t nCount);
    bool checkDecodeX(int32_t start, int32_t count, float minShort, float maxLong, float maxFront);
    bool decodeRaw(int32_t bitsRequested);
    bool decodeAsync(float *pData, int32_t bits, int32_t sizes, double dBitMin, double dBitMax, int32_t bitsPerByte, int32_t minBits);

    bool lead_in(float minTot, float maxTot, float maxOn, float minOff, float minFrameInc) {
        if (pLead) {
            if (pLead[1] >= minOff && pLead[0] <= maxOn) {
                float t = pLead[0] + pLead[1];
                if (t >= minTot && t <= maxTot && nFrame1 >= t + minFrameInc)
                    return 1;
            }
            if (pLead2) {
                if (pLead2[1] >= minOff && pLead2[0] <= maxOn) {
                    float t = pLead2[0] + pLead2[1];
                    if (t >= minTot && t <= maxTot && nFrame2 >= t + minFrameInc)
                        return 1;
                }
            }
        }
        return 0;
    }
    bool processHumaxAtoms(int32_t bitStart, float* pFr, int32_t maxBursts);
    bool processManchesterAtoms(int32_t burstStart, int32_t numBits, float used, float bitDur, float* pFr);
    void tryRC6();
    void tryRC5();
    void tryNokia();
    void tryGap();
    void tryZenith();
    void tryXMP();
    void trySomfy();
    void tryX10();
    void tryPCTV();
    void tryPid13();
    void tryF12();
    void trySingleBurstSize();
    void trySony();
    void tryGXB();
    void tryAsync();
    void tryAirboard();
    void tryAirAsync();
    void tryAK();
    void tryXX();
    void tryQ1();
    void tryQ2();
    void tryDirecTV();
    void trySunfire();
    void tryGrundig16();
    void tryTDC(); // GD 2009
    void tryCanalSat(); // GD 2009
    void trySejin(); // GD 2009
    void tryLutron(); // GD 2010
    void tryElan(); // DAR 2012
    void tryBryston(); // DAR 2012
    void tryHumax(); // DAR 2012
    void tryAdNotam(); // DAR 2012

    int16_t moreBlaupunkt(int32_t bits);

    uint32_t* pContext;
    int32_t nFreq_in; // GD 2009 Input freq (actual freq may be calculated)
    int32_t nSingle; // Bursts in single part
    int32_t nRepeat; // Bursts in repeat part
    char* pProtocol;
    int32_t* pDevice;
    int32_t* pSubDevice;
    int32_t* pOBC;
    int32_t* pHex;
    char* pMisc;
    char* pError;
    float* pDuration;
    float* pMainLimit;
    float* pFullLimit;



    int32_t nExtra; // GD 2009 Bursts in extra part
    int32_t nFreq;
    float* pFrame;
    float* pFrameEnd;
    int32_t nFrameL; // Burst count of frame 
    int32_t nProtocol;
    int32_t preemptLength;
    int32_t preemptValue;
    int32_t newPreemptLength;
    int32_t newPreemptValue;
    // GD 2009 start
    int32_t nFrameCount;
    int32_t bSuppressCount;
    int32_t nDittos;
    int32_t nNonSpurious; // Count of assuredly non-spurious bursts 
    int32_t bInitLeadIn; // Set when a single lead-in precedes a repeat sequence, eg JVC protocol
    int32_t nContextLength; // Length of Context[], default 2
    int32_t nGapProtocol; // Index of tryGap in Funcs[] (only protocol to use pLead)
    int32_t bXMPHalfValid; // Boolean set when an incomplete XMP decode has first half frame with valid checksum
    uint32_t zContext[2]; // Saved context value from start of repeat series

    // The values nNote_out and nAuxNote_out are indexes (base 0) into arrays m_Notes and m_AuxNotes 
    // of notes defined inIRScope.  A copy of those definitions is given above.  A value -1 means
    // no note is assigned.

    int32_t nNote_out; // Ref number of note for return to IRScope
    int32_t nAuxNote_out; // Ref number of aux note for return to IRScope
    int16_t* pCounts; // Provided in pContext[4] by IRScope for calc of freq for individual decodes
    // GD 2009 end
    float nTotDur; // Excluding Off half of leadOut
    float nMaxDur; // Excluding Off half of leadOut

    unsigned char cBits[16];
    char* pSuffix; //DAR Dec 2010 String which may be appended to pProtocol
    bool AminoToggleDetected; // DAR July 2011  Set when a correct toggle sequence is found
    bool ZaptorToggleDetected;
    int32_t nBit;
    int32_t nState;
    float* pBit;
    float* pLead;
    float* pLead2;

    float minGlitch;
    float nMinShort;
    float nMaxShort;
    float nMinLong;
    float nMaxLong;
    float nMinShort2;
    float nMaxShort2;

    union {

        struct {
            float m_minShortPulse;
            float m_nominalShortPulse;
            float m_maxShortPulse;
            float m_nominalLongPulse;
            float m_maxLongPulse;

            float m_minShortGap;
            float m_maxShortGap;
            float m_minLongGap;
            float m_maxLongGap;

            float m_nominalShortGap;
            float m_nominalLongGap;
            float m_negDelta; // vs. nominal full bit time
            float m_posDelta;
        };

        struct {
            float m_rawUnit;
            float m_rawPulseAdjust;
            float m_rawGapAdjust;
            float m_rawErrorLimit;
        };
    };

    float m_extra;

    float nFrame1;
    float nFrame2;

    float frameLeft;
    float frame;

    setOn sortOn;
    setOff sortOff;
    setBurst sortBurst;
}; // struct Signal

inline uint32_t parity(uint32_t p) {
    uint32_t x = p ^ (p >> 16);
    x = x ^ (x >> 8);
    x = x ^ (x >> 4);
    x = x ^ (x >> 2);
    return ( x ^ (x >> 1)) & 1;
}

enum {
    prGap = 1,
    prAsync = 1,
    prByXMP = 1,
    prByZenith = 1,
    prByPid_0003 = 1,
    prByDenon = 1, // GD 2009 Denon protocol 2nd half
    prBySamsung36 = 1,
    prByBlaupunkt = 1,
    prBlaupunkt = prAsync + 1,
    prByPid_0001 = prBlaupunkt,
    prBySony = prBlaupunkt,
    prJerrold = prBlaupunkt,
    prByGI_RG = prBlaupunkt, // DAR 2012
    prBySIM2 = prBlaupunkt, // DAR 2012
    prQ2 = prBlaupunkt + 1, // GD 2009
    prRCx = prQ2, // GD 2009
    prByOrtekMCE = prQ2, // GD 2009
    prBySolidtek = prQ2 // GD 2009

};

inline bool Signal::framedLeft(float nGap) {
    return nGap <= frameLeft;
}

inline bool Signal::framed(float nGap) {
    return nGap <= frame;
}

#endif // ! _DECODE_IR_H
