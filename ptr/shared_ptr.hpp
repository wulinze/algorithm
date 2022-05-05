#pragma once

#include <iostream>
#include <mutex>

namespace algorithm{
    namespace ptr{
        template<typename _Tp>
        class sharedPtr{
        private:
            _Tp* _ptr;
            int* _pRefCount;
            std::mutex* _pMutex;
        public:
            sharedPtr() = default;
            sharedPtr(_Tp* ptr = nullptr)
            : _ptr(ptr)
            , _pRefCount(new int(1))
            , _pMutex(new std::mutex)
            {}
            ~sharedPtr(){
                Release();
            }
            sharedPtr(const sharedPtr<_Tp>& p)
            : _ptr(p._ptr)
            , _pRefCount(p._pRefCount)
            , _pMutex(p._pMutex)
            {
                AddCount();
            }

            sharedPtr<_Tp>& operator=(sharedPtr<_Tp>&& p){
                if(_ptr != p._ptr){
                    Release();
                    this->_ptr = p->_ptr;
                    this->_pRefCount = p->_pRefCount;
                    this->_pMutex = p->_pMutex;
                    AddCount();
                }

                return *this;
            }
            _Tp& operator*(){
                return *_ptr;
            }
            _Tp* operator->(){
                return _ptr;
            }
            int UseCount() {
                return *_pRefCount;
            }
            _Tp* Get(){
                return _ptr;
            }
            void AddCount(){
                _pMutex->lock();
                (*_pRefCount)++;
                _pMutex->unlock();
            }
            friend std::ostream& operator <<(std::ostream& out, sharedPtr<_Tp>& p){
                out << *(p._ptr) << ',' << *(p._pRefCount) << std::endl;

                return out;
            }
        private:
            void Release(){
                auto flag = false;
                _pMutex->lock();
                if(--(*_pRefCount) == 0){
                    delete _ptr;
                    delete _pRefCount;
                    flag = true;
                }
                _pMutex->unlock();
                if(flag){
                    delete this->_pMutex;
                }
            }
        };
    }
}