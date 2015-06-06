set(PROJECT dci-async)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)





############################################################
include(CheckIncludeFiles)










############################################################
check_include_files(ucontext.h HAVE_UCONTEXT)

############################################################
if(WIN32)
    include(CheckPrototypeDefinition)
    check_prototype_definition(
        CreateFiberEx
        "LPVOID WINAPI CreateFiberEx(SIZE_T dwStackCommitSize, SIZE_T dwStackReserveSize, DWORD dwFlags, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter);"
        "NULL"
        windows.h
        HAVE_WINFIBER)
endif()

############################################################
find_package(Boost COMPONENTS context system)

if(Boost_CONTEXT_FOUND)
    set(HAVE_BOOSTCONTEXT ON)
endif()

############################################################
include(FindValgrind.cmake)
if(VALGRIND_FOUND)
    set(HAVE_VALGRIND TRUE)
    #message(STATUS "found valgrind: ${VALGRIND_PROGRAM}, ${VALGRIND_INCLUDE_DIR}")
endif()






############################################################
if(NOT ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER AND NOT ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT AND NOT ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    if(HAVE_BOOSTCONTEXT)
        set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT ON)
    elseif(HAVE_WINFIBER)
        set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER ON)
    elseif(HAVE_UCONTEXT)
        set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT ON)
    else()
        message(FATAL_ERROR "no support for context engine available")
    endif()
endif()



set(CONTEXT_ENGINE "")

if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT AND HAVE_BOOSTCONTEXT)
    set(CONTEXT_ENGINE "BOOSTCONTEXT")
elseif(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT AND HAVE_UCONTEXT)
    set(CONTEXT_ENGINE "UCONTEXT")
elseif(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER AND HAVE_WINFIBER)
    set(CONTEXT_ENGINE "WINFIBER")
else()
    message(FATAL_ERROR "context engine has no support")
endif()

message(STATUS "use ${CONTEXT_ENGINE} as context engine")
if(CONTEXT_ENGINE STREQUAL "WINFIBER")
    set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER ON)
    set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT OFF)
    set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT OFF)
elseif(CONTEXT_ENGINE STREQUAL "UCONTEXT")
    set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER OFF)
    set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT ON)
    set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT OFF)
elseif(CONTEXT_ENGINE STREQUAL "BOOSTCONTEXT")
    set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER OFF)
    set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT OFF)
    set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT ON)
else()
    message(FATAL_ERROR "unknown context engine2")
endif()


set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER ${ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER} CACHE BOOL "use windows fibers as context engine" FORCE)
set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT ${ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT} CACHE BOOL "use posix ucontext as context engine" FORCE)
set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT ${ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT} CACHE BOOL "use boost::context as context engine" FORCE)

############################################################
configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})



























############################################################
set(INC
    include/dci/async.hpp
    include/dci/async/api.hpp
    include/dci/async/functions.hpp

    include/dci/async/waitable.hpp
    include/dci/async/lockable.hpp
    include/dci/async/mutex.hpp
    include/dci/async/mutexRecursive.hpp
    include/dci/async/semaphore.hpp
    include/dci/async/condVar.hpp
    include/dci/async/event.hpp
    include/dci/async/notifierOne.hpp
    include/dci/async/notifierAll.hpp
    include/dci/async/pulserOne.hpp
    include/dci/async/pulserAll.hpp
    include/dci/async/barrier.hpp

    include/dci/async/waiter.hpp
    include/dci/async/wait.hpp
    include/dci/async/future.hpp
    include/dci/async/promise.hpp
    include/dci/async/details/futureState.hpp
    include/dci/async/details/task.hpp
    include/dci/async/details/taskInstance.hpp
    include/dci/async/details/taskPromiseProcessor.hpp
    include/dci/async/details/wait.hpp
    include/dci/async/layoutProvider.hpp
)

############################################################
set(SRC
    src/functions.cpp

    src/waiter.cpp
    src/waitable.cpp
    src/lockable.cpp
    src/mutex.cpp
    src/mutexRecursive.cpp
    src/semaphore.cpp
    src/condVar.cpp
    src/event.cpp
    src/notifierOne.cpp
    src/notifierAll.cpp
    src/pulserOne.cpp
    src/pulserAll.cpp
    src/barrier.cpp

    src/details/task.cpp
    
    src/impl/scheduler.hpp
    src/impl/scheduler.cpp
    src/impl/task.hpp
    src/impl/task.cpp
    src/impl/effortContainer.hpp

    src/impl/waiter.hpp
    src/impl/waitable.hpp
    src/impl/lockable.hpp

    src/impl/mutex.hpp
    src/impl/mutexRecursive.hpp
    src/impl/semaphore.hpp
    src/impl/condVar.hpp
    src/impl/event.hpp
    src/impl/notifierOne.hpp
    src/impl/notifierAll.hpp
    src/impl/pulserOne.hpp
    src/impl/pulserAll.hpp
    src/impl/barrier.hpp

    src/impl/waiter.cpp
    src/impl/waitable.cpp
    src/impl/lockable.cpp

    src/impl/mutex.cpp
    src/impl/mutexRecursive.cpp
    src/impl/semaphore.cpp
    src/impl/condVar.cpp
    src/impl/event.cpp
    src/impl/notifierOne.cpp
    src/impl/notifierAll.cpp
    src/impl/pulserOne.cpp
    src/impl/pulserAll.cpp
    src/impl/barrier.cpp
)







############################################################
set(INC_CTX
)

############################################################
set(SRC_CTX
    src/impl/ctx/root.hpp
    src/impl/ctx/root.cpp
    src/impl/ctx/coro.hpp
    src/impl/ctx/coro.cpp
)
if(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER)
    list(APPEND SRC_CTX src/impl/ctx/engine_winfiber.hpp)
    list(APPEND SRC_CTX src/impl/ctx/engine_winfiber.cpp)
endif()
if(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT)
    list(APPEND SRC_CTX src/impl/ctx/engine_ucontext.hpp)
    list(APPEND SRC_CTX src/impl/ctx/engine_ucontext.cpp)
endif()
if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    list(APPEND SRC_CTX src/impl/ctx/engine_boostcontext.hpp)
    list(APPEND SRC_CTX src/impl/ctx/engine_boostcontext.cpp)
endif()



















############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

if(VALGRIND_FOUND)
    include_directories(${VALGRIND_INCLUDE_DIR})
endif()







############################################################
add_library(${PROJECT} SHARED
    ${INC}             ${SRC}
    ${INC_CTX}         ${SRC_CTX}
)

target_link_libraries(${PROJECT}
    dci-himpl
    dci-mm
    dci-logger)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

include(dciApi)
dciApiExport(${PROJECT})

if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    target_link_libraries(${PROJECT} ${Boost_CONTEXT_LIBRARY})
endif()

target_link_libraries(${PROJECT} ${Boost_SYSTEM_LIBRARY})

include(dciHimpl)
dciMakeHimplLayouts(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dci/async/layoutProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/scheduler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/task.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/waitable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/lockable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/waiter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/mutex.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/mutexRecursive.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/semaphore.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/condVar.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/event.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/notifierOne.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/notifierAll.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/pulserOne.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/pulserAll.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/barrier.hpp

    CLASSES
        ::dci::async::impl::Scheduler
        ::dci::async::impl::Task
        ::dci::async::impl::Waitable
        ::dci::async::impl::Lockable
        ::dci::async::impl::Waiter

        ::dci::async::impl::Mutex
        ::dci::async::impl::MutexRecursive
        ::dci::async::impl::Semaphore
        ::dci::async::impl::CondVar
        ::dci::async::impl::Event
        ::dci::async::impl::NotifierOne
        ::dci::async::impl::NotifierAll
        ::dci::async::impl::PulserOne
        ::dci::async::impl::PulserAll
        ::dci::async::impl::Barrier
)


############################################################
include(dciTest)
dciTest(${PROJECT} noenv
    SRC test/syncronizers.cpp
    LINK ${PROJECT}
)



if(COMMAND localinstall)
    localinstall(${PROJECT} include DROP_PREFIX include/ ${INC})
endif()




