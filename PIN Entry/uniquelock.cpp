#include "uniquelock.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream> //for debugging
#include <string.h>

CUniqueLock::CUniqueLock()
{
    m_state = KState_Close;
    m_file = -1; //invalid descriptor
}

CUniqueLock::~CUniqueLock()
{
    Close();
}

int CUniqueLock::Open(const char * id)
{
    Close(); //to make sure it is not already open
    m_state = KState_Open;

    m_path = std::string("/var/tmp/");
    m_path.append(id);
    m_file = open(m_path.data(), O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
    if (m_file == -1)
    {
        if (errno == EACCES)
        {
            return -2;
        }
        else
        {
            std::cout <<  strerror(errno) << std::endl;
            return -10;
        }
    }
    if (flock(m_file, LOCK_EX | LOCK_NB) == -1)
    {
        if (errno == EWOULDBLOCK)\
        {
            //another process already running
            close(m_file);
            m_file = -1;
            return -1;
        }
    }

    return 0;
}

void CUniqueLock::Close()
{
    if (m_state == KState_Close)
    {
        return;
    }

    if (m_file >= 0)
    {
        flock(m_file, LOCK_UN);
        close(m_file);
        unlink(m_path.data());
    }
    m_state = KState_Close;
}
