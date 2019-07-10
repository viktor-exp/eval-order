#include <cstdint>
#include <cassert>
#include <cstring>

typedef int32_t int32_t;

#define MAXREFS      20
#define OK 0

struct Reference_t
{
    uint32_t refs[MAXREFS];
};

struct Look_t
{
    Reference_t* refs;
    uint16_t count;
};

static uint32_t CountOffsets(uint32_t* offsets)
{
    uint32_t count = (uint32_t)0;
    assert(offsets != NULL);

    while (*offsets)
    {
        offsets++;
        count++;
    }

    if (count)
    {
        count--;
    }

    return count;
}

static int32_t GetFields( Look_t* ptL, uint32_t** offsets, uint32_t* offsetCount)
{
    int32_t status = OK;
    uint32_t count = 0;
    uint32_t* offs = NULL;

    assert(ptL != NULL);

    uint16_t wI = 0;

    // This condition is evaluated partially incorrectly.
    // Although the pointer is *tested* first, the dereference happens before that!
    // 00007FF75A891020  mov         ecx,dword ptr [rax]   ; Dereference *offs
    // 00007FF75A891022  test        rax,rax               ; Test the pointer offs
    // 00007FF75A891025  je          main+2Bh (07FF75A89102Bh)
    // 00007FF75A891027  test        ecx,ecx               ; Test the dereferenced value
    // 00007FF75A891029  jne         main+44h (07FF75A891044h)  

    while ((offs == NULL || *offs == 0) && wI < ptL->count)
    {
        offs = ptL->refs[wI].refs;
        wI++;
    }

    // Uncommenting the following if produces correct code.
    //if (offs != NULL)
    {
        count = CountOffsets(offs);
    }
    status = OK;
    *offsetCount = count;
    *offsets = offs;
    return status;
}

int main()
{
    Look_t look;
    Reference_t refs[] = { { 1, 2, 3 }, 3 };
    uint32_t* offs = NULL;
    uint32_t count = 0;
    memset(&look, 0, sizeof(look));
    look.count = 1;
    look.refs = refs;
    return GetFields(&look, &offs, &count);
}
