#ifndef SINGLETON_H
#define SINGLETON_H

#include "shared/typedef.h"

#define MAKE_SINGLETON(CLASS) friend class jRylServer::common::shared::iSingleton<CLASS>

namespace jRylServer {
namespace common {
namespace shared {

template<class T> class iSingleton {
public: 
    static T& GetInstance();
    static T* GetInstance_ptr();
    static void DestroyInstance();
protected:
    inline iSingleton() {
        iSingleton::ms_Instance = static_cast<T*>(this); 
    }

    inline ~iSingleton() {
        iSingleton::ms_Instance = NULL; 
    }
private:
    static T* ms_Instance; 
    inline iSingleton(iSingleton const&) {}
    inline iSingleton& operator=(iSingleton const&) { return *this; }
};

template<class T> T& iSingleton<T>::GetInstance() {
    if ( iSingleton::ms_Instance == NULL ) {
        iSingleton::ms_Instance = new T;
    }
    return *(iSingleton::ms_Instance);
}

template<class T> T* iSingleton<T>::GetInstance_ptr() {
    if ( iSingleton::ms_Instance == NULL ) {
        iSingleton::ms_Instance = new T;
    }
    return iSingleton::ms_Instance;
}

template<class T> void iSingleton<T>::DestroyInstance() {
    if ( iSingleton::ms_Instance != NULL ) {
        delete iSingleton::ms_Instance;
        iSingleton::ms_Instance = NULL;
    }
}

template<class T> T* iSingleton<T>::ms_Instance = NULL;

} //namespace shared
} //namespace common
} //namespace jRylServer

#endif