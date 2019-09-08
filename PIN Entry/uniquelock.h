#ifndef UNIQUELOCK_H
#define UNIQUELOCK_H


#include <string>

enum TState
{
    KState_Close = 0,
    KState_Open = 1,
};

class CUniqueLock
{
public:
    CUniqueLock(void);
    ~CUniqueLock(void);

    // return 0 if successful
    // return -1 if another lock is already active
    // return -2 if not enough permission
    // otherwise other errors
    int Open(const char* id);

    void Close(void);

    // 0 if closed, otherwise open
    TState m_state;
    int m_file; //file descriptor
    std::string m_path;
};

#endif // UNIQUELOCK_H
