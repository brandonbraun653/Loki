/********************************************************************************
 *  File Name:
 *    generic_threading_implementation.cpp
 *
 *  Description:
 *    Implements all classes used in Chimera::Threading for the Generic system.
 *
 *  Notes:
 *    1. This will require the underlying system to have some idea of how to
 *       enable/disable system level interrupts to simulate blocking access to
 *       resources.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <chrono>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/system.hpp>
#include <Chimera/threading/extensions.hpp>
#include <Chimera/threading/mutex.hpp>
#include <Chimera/threading/thread.hpp>


#if defined( USING_WINDOWS ) || defined( USING_LINUX ) || defined( USING_STL_THREADS )

namespace Chimera::Threading
{
  /************************************************************************/
  /*                             EXTENSIONS                               */
  /************************************************************************/
  Lockable::Lockable()
  {
  }

  Lockable::~Lockable()
  {
  }

  void Lockable::lock()
  {
    mutex.lock();
  }

  void Lockable::lockFromISR()
  {
    mutex.lock();
  }

  bool Lockable::try_lock_for( const size_t timeout )
  {
    return mutex.try_lock_for( timeout );
  }

  void Lockable::unlock()
  {
    mutex.unlock();
  }

  void Lockable::unlockFromISR()
  {
    mutex.unlock();
  }


  /************************************************************************/
  /*                                MUTEX                                 */
  /************************************************************************/

  /*------------------------------------------------
  Mutex Implementation
  ------------------------------------------------*/
  void Mutex::lock()
  {
    _mtx.lock();
  }

  bool Mutex::try_lock()
  {
    return _mtx.try_lock();
  }

  void Mutex::unlock()
  {
    _mtx.unlock();
  }

  /*------------------------------------------------
  Recursive Mutex Implementation
  ------------------------------------------------*/
  void RecursiveMutex::lock()
  {
    _mtx.lock();
  }

  bool RecursiveMutex::try_lock()
  {
    return _mtx.try_lock();
  }

  void RecursiveMutex::unlock()
  {
    _mtx.unlock();
  }

  /*------------------------------------------------
  Timed Mutex Implementation
  ------------------------------------------------*/
  void TimedMutex::lock()
  {
    _mtx.lock();
  }

  bool TimedMutex::try_lock()
  {
    return _mtx.try_lock();
  }

  bool TimedMutex::try_lock_for( const size_t timeout )
  {
    return _mtx.try_lock_for( std::chrono::milliseconds( timeout ) );
  }

  bool TimedMutex::try_lock_until( const size_t timeout )
  {
    auto now = std::chrono::steady_clock::now();
    return _mtx.try_lock_until( now + std::chrono::milliseconds( timeout ) );
  }

  void TimedMutex::unlock()
  {
    _mtx.unlock();
  }

  /*------------------------------------------------
  Recursive Timed Mutex Implementation
  ------------------------------------------------*/
  void RecursiveTimedMutex::lock()
  {
    _mtx.lock();
  }

  bool RecursiveTimedMutex::try_lock()
  {
    return _mtx.try_lock();
  }

  bool RecursiveTimedMutex::try_lock_for( const size_t timeout )
  {
    return _mtx.try_lock_for( std::chrono::milliseconds( timeout ) );
  }

  bool RecursiveTimedMutex::try_lock_until( const size_t timeout )
  {
    auto now = std::chrono::steady_clock::now();
    return _mtx.try_lock_until( now + std::chrono::milliseconds( timeout ) );
  }

  void RecursiveTimedMutex::unlock()
  {
    _mtx.unlock();
  }

  /************************************************************************/
  /*                               THREAD                                 */
  /************************************************************************/
  void startScheduler()
  {
    while ( 1 )
    {
      Chimera::Threading::this_thread::sleep_for( 100 );
    }
  }

  Thread::Thread() : mFunc( nullptr ), mFuncArg( nullptr )
  {
    mThreadName.fill( 0 );
  }

  Thread::Thread( Thread &&other ) : mFunc( other.mFunc ), mFuncArg( other.mFuncArg )
  {
    mThreadName.fill( 0 );
  }

  Thread::Thread( ThreadFunctPtr func, ThreadArg arg ) : mFunc( func ), mFuncArg( arg )
  {
    mThreadName.fill( 0 );
  }

  Thread::~Thread()
  {
  }

  void Thread::start( const Priority priority, const size_t stackDepth, const std::string_view name )
  {
    ( void )priority;
    ( void )stackDepth;

    size_t copyLen = name.length();
    if ( copyLen > MAX_NAME_LEN )
    {
      copyLen = MAX_NAME_LEN;
    }

    mThreadName.fill( 0 );
    memcpy( mThreadName.data(), name.data(), copyLen );

    mThread = std::thread( mFunc, mFuncArg );
  }

  void Thread::join()
  {
    mThread.join();
  }

  bool Thread::joinable()
  {
    return mThread.joinable();
  }

  Id Thread::get_id()
  {
    auto stl_id = mThread.get_id();

    return Id();
  }

  detail::native_thread_handle_type Thread::native_handle()
  {
    return mThread.native_handle();
  }

  int Thread::hardware_concurrency()
  {
    return Chimera::System::maxConcurrentThreads();
  }

  Id this_thread::get_id()
  {
    auto stl_id = std::this_thread::get_id();
    
    return Id();
  }

  void this_thread::sleep_for( const size_t timeout )
  {
    std::this_thread::sleep_for( std::chrono::milliseconds( timeout ) );
  }

  void this_thread::sleep_until( const size_t timeout )
  {
    auto now = std::chrono::steady_clock::now();
    std::this_thread::sleep_until( now + std::chrono::milliseconds( timeout ) );
  }

  void this_thread::yield()
  {
    std::this_thread::yield();
  }

}  // namespace Chimera::Threading

#endif /* USING_GENERIC_SYSTEM */