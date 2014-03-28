#pragma once




class CHosts
{
public:
    CHosts(void);
    ~CHosts(void);

    BOOL Apply(DWORD dwModeId);
    BOOL Restore();
};

