/********************************************************************************
*  File Name:
*    extensions.hpp
*
*  Description:
*    Threading extensions that implement some kind of helper objects
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#ifndef CHIMERA_THREADING_EXTENSIONS_HPP
#define CHIMERA_THREADING_EXTENSIONS_HPP

/* Chimera Includes */
#include <Chimera/threading/mutex.hpp>

namespace Chimera::Threading
{
  template<class mutex_type>
  class LockGuard
  {
  public:
    explicit LockGuard( mutex_type &mutex ) :
        mtx( mutex )
    {
      mtx.lock();
    }

    ~LockGuard()
    {
      mtx.unlock();
    }

    LockGuard( const LockGuard & ) = delete;

  private:
    mutex_type &mtx;
  };

  template<class mutex_type>
  class TimedLockGuard
  {
  public:
    explicit TimedLockGuard( mutex_type &mutex ) : mtx( mutex ), is_locked( false )
    {
    }

    ~TimedLockGuard()
    {
      if ( is_locked )
      {
        mtx.unlock();
      }
    }

    bool try_lock_for( const size_t timeout = 100 )
    {
      is_locked = mtx.try_lock_for( timeout );
      return is_locked;
    }

  private:
    mutex_type &mtx;
    bool is_locked;
  };
  

  class Lockable
  {
  public:
    void lock();

    void lockFromISR();

    bool try_lock_for( const size_t timeout );

    void unlock();

    void unlockFromISR();

    Lockable();
    ~Lockable();

  private:
    RecursiveTimedMutex mutex;
  };
}

#endif	/* !CHIMERA_THREADING_EXTENSIONS_HPP */