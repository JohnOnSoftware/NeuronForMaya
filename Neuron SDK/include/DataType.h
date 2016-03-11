#pragma once

#include <stdint.h>

/**********************************************************
 *               Customer data types definition           *
 **********************************************************/
#ifdef __OS_XUN__            // Mac OS X, Linux or Unix like OS data type definition
#define __stdcall            // Empty
#endif

#ifndef NBOOL
#define NBOOL int
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Reference to a TCP/IP socket. It is only used as an ID to identify what connector is used.
#define SOCKET_REF void*

// Socket status
typedef enum _SocketStatus
{
    CS_Running,              // Socket is working correctly
    CS_Starting,             // Is trying to start service
    CS_OffWork,              // Not working
}SocketStatus;


#pragma pack(push, 1)

// Data version
typedef union DataVersion
{
    uint32_t _VersionMask;
    struct
    {
        uint8_t BuildNumb;      // Build number
        uint8_t Revision;       // Revision number
        uint8_t Minor;          // Subversion number
        uint8_t Major;          // Major version number
    };
}DATA_VER;

// Header format of BVH data
typedef struct _BvhDataHeader
{
    uint16_t   Token1;           // Package start token: 0xDDFF
    DATA_VER   DataVersion;      // Version of community data format. e.g.: 1.0.0.2
    uint16_t   DataCount;        // Values count
    uint8_t    WithDisp;         // With/out displacement
    uint8_t    WithReference;    // With/out reference bone data at first
    uint32_t   AvatarIndex;      // Avatar index
    uint8_t    AvatarName[32];   // Avatar name
    uint32_t   FrameIndex;       // Frame data index
    uint32_t   Reserved;         // Reserved, only enable this package has 64bytes length
    uint32_t   Reserved1;        // Reserved, only enable this package has 64bytes length
    uint32_t   Reserved2;        // Reserved, only enable this package has 64bytes length
    uint16_t   Token2;           // Package end token: 0xEEFF
}BvhDataHeader;

// Header format of BVH data
typedef struct _CalcDataHeader
{
    uint16_t   Token1;          // Package start token: 0x88FF
    DATA_VER   DataVersion;     // Version of community data format. e.g.: 1.0.0.3
    uint32_t   DataCount;       // Values count
    uint32_t   AvatarIndex;     // Avatar index
    uint8_t    AvatarName[32];  // Avatar name
    uint32_t   FrameIndex;      // Frame data index
    uint32_t   Reserved1;       // Reserved, only enable this package has 64bytes length
    uint32_t   Reserved2;       // Reserved, only enable this package has 64bytes length
    uint32_t   Reserved3;       // Reserved, only enable this package has 64bytes length
    uint16_t   Token2;          // Package end token: 0x99FF
}CalcDataHeader;

#pragma pack(pop)
