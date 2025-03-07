#pragma once
#include "CObject.h"
class CLine :
    public CObject
{
public:
    CLine();
    ~CLine();
    DECLARE_SERIAL(CLine)
    void Serialize(CArchive& archive) {
        if (archive.IsStoring()) {
            archive.Write(&x, sizeof(x));
            archive.Write(&y, sizeof(y));
        }
        else {
            archive.Read(&x, sizeof(x));
            archive.Read(&y, sizeof(y));
        }
    }

    int x;
    int y;

};

