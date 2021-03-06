/**
 * @file    MessageQueue.cpp
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2013/05/11
 * @version 1.0
 *
 * @brief
 */

#include "llbc/common/Export.h"
#include "llbc/common/BeforeIncl.h"

#include "llbc/core/thread/MessageBlock.h"
#include "llbc/core/thread/MessageQueue.h"

__LLBC_NS_BEGIN

LLBC_MessageQueue::LLBC_MessageQueue()
{
    _head = NULL;
    _tail = NULL;

    _size = 0;
}

LLBC_MessageQueue::~LLBC_MessageQueue()
{
    Cleanup();
}

ulong LLBC_MessageQueue::GetSize() const
{
    LLBC_MessageQueue *nonConstThis = 
        const_cast<LLBC_MessageQueue *>(this);

    nonConstThis->_lock.Lock();
    ulong ret = _size;
    nonConstThis->_lock.Unlock();

    return ret;
}

void LLBC_MessageQueue::Cleanup()
{
    _lock.Lock();

    if (!_head)
    {
        _lock.Unlock();
        return;
    }

    while (_head)
    {
        LLBC_MessageBlock *block = _head;
        _head = _head->GetNext();

        delete block;
    }

#if LLBC_TARGET_PLATFORM_NON_WIN32
    _cond.Broadcast();
#else // LLBC_TARGET_PLATFORM_WIN32
    _sem.Post(_size);
#endif // LLBC_TARGET_PLATFORM_NON_WIN32

    _size = 0;
    _tail = NULL;

    _lock.Unlock();
}

void LLBC_MessageQueue::Push(LLBC_MessageBlock *block, bool front)
{
    _lock.Lock();
    PushNonLock(block, front);
    _lock.Unlock();

#if LLBC_TARGET_PLATFORM_NON_WIN32
    _cond.Notify();
#else // LLBC_TARGET_PLATFORM_WIN32
    _sem.Post();
#endif // LLBC_TARGET_PLATFORM_NON_WIN32
}

void LLBC_MessageQueue::PushFrontNonLock(LLBC_MessageBlock *block)
{
    block->SetPrev(NULL);
    if (!_head)
    {
        block->SetNext(NULL);
        _head = _tail = block;
    }
    else
    {
        _head->SetPrev(block);
        block->SetNext(_head);
        _head = block;
    }

    _size += 1;
}

void LLBC_MessageQueue::PushBackNonLock(LLBC_MessageBlock *block)
{
    block->SetNext(NULL);
    if (!_tail)
    {
        block->SetPrev(NULL);
        _tail = _head = block;
    }
    else
    {
        _tail->SetNext(block);
        block->SetPrev(_tail);
        _tail = block;
    }

    _size += 1;
}

bool LLBC_MessageQueue::Pop(LLBC_MessageBlock *&block, int interval, bool front)
{
#if LLBC_TARGET_PLATFORM_NON_WIN32
    if (interval != 0)
    {
        block = NULL;
        while (!block)
        {
            _lock.Lock();
            if (_size > 0)
            {
                PopNonLock(block, front);
                _lock.Unlock();

                return true;
            }

            _cond.TimedWait(_lock, interval);
            if (_size > 0)
            {
                PopNonLock(block, front);
                _lock.Unlock();

                return true;
            }

            _lock.Unlock();
            if (interval != LLBC_INFINITE)
                break;
        }
    }
    else
    {
        _lock.Lock();
        if (_size > 0)
        {
            PopNonLock(block, front);
            _lock.Unlock();

            return true;
        }

        _lock.Unlock();
    }
#else // LLBC_TARGET_PLATFORM_WIN32
    if (interval == LLBC_INFINITE)
    {
        _sem.Wait();

        _lock.Lock();
        PopNonLock(block, front);
        _lock.Unlock();

        return true;
    }
    else if (interval == 0)
    {
        if (_sem.TryWait())
        {
            _lock.Lock();
            PopNonLock(block, front);
            _lock.Unlock();

            return true;
        }
    }
    else
    {
        if (_sem.TimedWait(interval))
        {
            _lock.Lock();
            PopNonLock(block, front);
            _lock.Unlock();

            return true;
        }
    }
#endif // LLBC_TARGET_PLATFORM_NON_WIN32

    return false;
}

void LLBC_MessageQueue::PopFrontNonLock(LLBC_MessageBlock *&block)
{
    block = _head;
    if (!(_head = _head->GetNext()))
        _tail = NULL;
    else
        _head->SetPrev(NULL);

    _size -= 1;
}

void LLBC_MessageQueue::PopBackNonLock(LLBC_MessageBlock *&block)
{
    block = _tail;
    if (!(_tail = _tail->GetPrev()))
        _head = NULL;
    else
        _tail->SetNext(NULL);

    _size -= 1;
}

__LLBC_NS_END

#include "llbc/common/AfterIncl.h"
