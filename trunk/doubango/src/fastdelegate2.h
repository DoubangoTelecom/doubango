/**
* @file
* @author  xxxyyyzzz <imsframework(at)gmail.com>
* @version 1.0
*
* @section LICENSE
*
*	
* This file is part of Open Source Doubango IMS Client Framework project.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
* @section DESCRIPTION
*
*
*/
/* 
	This code comes from http://www.rsdn.ru/forum/src/1712949.1.aspx
	
	* I have made some changes for MinGW (GCC 4.4.0)
*/

#ifndef MULTICATDELEGATE_H
#define MULTICATDELEGATE_H

#include "FastDelegate.h"

#include <vector>
#include <algorithm>

#define DEFAULT_CRITICAL_SECTION fastdelegate2::dummy_section

using namespace fastdelegate;

namespace fastdelegate2
{   
  namespace detail
  {
    typedef fastdelegate::detail::DefaultVoid DefaultVoid;
  }
  
  class dummy_section
  {
  public:
    void lock() const {}
    void unlock() const {}
  };

  template<typename T>
  class auto_cs_t
  {
  public:
    auto_cs_t(const T& refcs) : m_refcs(refcs) { m_refcs.lock(); }
    ~auto_cs_t() { m_refcs.unlock(); }
  private:
    const T& m_refcs;
  };

  template<class RetType>
  class default_slot
  {
  public:
    template <typename F>
    void operator() (const F& func)     
    {
      vec_results.push_back(func());
    }  
    const std::vector<RetType>& get_results() const
    {
      return vec_results;
    }
  private:
    std::vector<RetType> vec_results;
  };

  template <>
  class default_slot<detail::DefaultVoid>
  {    
  public:    
    template <typename F>
    void operator() (const F& func) 
    {
      func();
    }    
  };

  namespace detail
  {
    template<class CritSect, class DelegateT>
    class delegate_base
    {
    public:    
      typedef DelegateT holder_type; 

      delegate_base() {}

    protected:
      void connect_delegate(const holder_type& dlghld)
      {
		  auto_cs_t<CritSect> lock(cs);

        if (std::find(vec_subscribers.begin(), vec_subscribers.end(), dlghld) == vec_subscribers.end())
          vec_subscribers.push_back(dlghld);
      }
      bool disconnect_delegate(const holder_type& dlghld)
      {
        auto_cs_t<CritSect> lock(cs);

        typename std::vector<holder_type>::iterator iter = std::find(vec_subscribers.begin(), vec_subscribers.end(), dlghld);
        if (iter != vec_subscribers.end())
        {
          vec_subscribers.erase(iter);
          return true;
        }
        return false;
      }

      CritSect cs;
      std::vector<holder_type> vec_subscribers;
    };

    template<class Param1, class RetType, class DelegateT>
    struct caller1
    {
      caller1(const DelegateT& holder, Param1 param1) : dlgh(holder), p1(param1) {}
      RetType operator() () const { return dlgh(p1); }
      Param1 p1; 
      const DelegateT& dlgh;
    };
    template<class Param1, class Param2, class RetType, class DelegateT>
    struct caller2
    {
      caller2(const DelegateT& holder, Param1 param1, Param2 param2) : dlgh(holder), p1(param1), p2(param2) {}
      RetType operator() () const { return dlgh(p1, p2); }
      Param1 p1; Param2 p2; 
      const DelegateT& dlgh;
    };
    template<class Param1, class Param2, class Param3, class RetType, class DelegateT>
    struct caller3
    {
      caller3(const DelegateT& holder, Param1 param1, Param2 param2, Param3 param3) : dlgh(holder), p1(param1), p2(param2), p3(param3) {}
      RetType operator() () const { return dlgh(p1, p2, p3); }
      Param1 p1; Param2 p2; Param3 p3; 
      const DelegateT& dlgh;
    };
    template<class Param1, class Param2, class Param3, class Param4, class RetType, class DelegateT>
    struct caller4
    {
      caller4(const DelegateT& holder, Param1 param1, Param2 param2, Param3 param3, Param4 param4) : dlgh(holder), p1(param1), p2(param2), p3(param3), p4(param4) {}
      RetType operator() () const { return dlgh(p1, p2, p3, p4); }
      Param1 p1; Param2 p2; Param3 p3; Param4 p4; 
      const DelegateT& dlgh;
    };
    template<class Param1, class Param2, class Param3, class Param4, class Param5, class RetType, class DelegateT>
    struct caller5
    {
      caller5(const DelegateT& holder, Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5) : dlgh(holder), p1(param1), p2(param2), p3(param3), p4(param4), p5(param5) {}
      RetType operator() () const { return dlgh(p1, p2, p3, p4, p5); }
      Param1 p1; Param2 p2; Param3 p3; Param4 p4; Param5 p5; 
      const DelegateT& dlgh;
    };
    template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class RetType, class DelegateT>
    struct caller6
    {
      caller6(const DelegateT& holder, Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5, Param6 param6) : dlgh(holder), p1(param1), p2(param2), p3(param3), p4(param4), p5(param5), p6(param6) {}
      RetType operator() () const { return dlgh(p1, p2, p3, p4, p5, p6); }
      Param1 p1; Param2 p2; Param3 p3; Param4 p4; Param5 p5; Param6 p6; 
      const DelegateT& dlgh;
    };
    template<class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7, class RetType, class DelegateT>
    struct caller7
    {
      caller7(const DelegateT& holder, Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5, Param6 param6, Param7 param7) : dlgh(holder), p1(param1), p2(param2), p3(param3), p4(param4), p5(param5), p6(param6), p7(param7) {}
      RetType operator() () const { return dlgh(p1, p2, p3, p4, p5, p6, p7); }
      Param1 p1; Param2 p2; Param3 p3; Param4 p4; Param5 p5; Param6 p6; Param7 p7; 
      const DelegateT& dlgh;
    };
  } //detail namespace 

  template< class RetType=detail::DefaultVoid, 
  class CritSect = DEFAULT_CRITICAL_SECTION, 
  class SlotT=default_slot<RetType> >
  class delegate0 : 
    detail::delegate_base< CritSect, FastDelegate0<RetType> >
  {
  public:
    delegate0() {}
    template < class X, class Y >
    delegate0(const Y *pthis, RetType (X::* fn)() const) 
    { connect_delegate(holder_type(pthis, fn)); }
    template < class X, class Y >
    delegate0(Y *pthis, RetType (X::* fn)()) 
    { connect_delegate(holder_type(pthis, fn)); }
    delegate0(RetType (*fn)()) 
    { connect_delegate(holder_type(fn)); }
    template < class X, class Y >
    void connect(const Y *pthis, RetType (X::* fn)() const) 
    { connect_delegate(holder_type(pthis, fn)); }
    template < class X, class Y >
    void connect(Y *pthis, RetType (X::* fn)()) 
    { connect_delegate(holder_type(pthis, fn)); }
    void connect(RetType (*fn)()) 
    { connect_delegate(holder_type(fn)); }
    template < class X, class Y >
    bool disconnect(const Y *pthis, RetType (X::* fn)() const)
    { return disconnect_delegate(holder_type(pthis, fn)); }
    template < class X, class Y >
    bool disconnect(Y *pthis, RetType (X::* fn)())
    { return disconnect_delegate(holder_type(pthis, fn)); }
    bool disconnect(RetType (*fn)()) 
    { return disconnect_delegate(holder_type(fn)); }
    void operator() () const
    {
      auto_cs_t<CritSect> lock(this->cs);
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate0<RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) (*iter)();
    }
    SlotT invoke() const
    {
      auto_cs_t<CritSect> lock(this->cs);
      SlotT slot;
      typename std::vector<typename detail::delegate_base< CritSect, FastDelegate0<RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) slot(*iter);
      return slot;
    }
  };
  template< class Param1, 
  class RetType=detail::DefaultVoid,
  class CritSect = DEFAULT_CRITICAL_SECTION,
  class SlotT=default_slot<RetType> >
  class delegate1 :
    detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >
  {
  public:
    delegate1() {}
    template < class X, class Y >
    delegate1(const Y *pthis, RetType (X::* fn)(Param1 p1) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    delegate1(Y *pthis, RetType (X::* fn)(Param1 p1))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type(pthis, fn)); }
    delegate1(RetType (*fn)(Param1 p1))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type(fn)); }
    template < class X, class Y >
    void connect(const Y *pthis, RetType (X::* fn)(Param1 p1) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    void connect(Y *pthis, RetType (X::* fn)(Param1 p1))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type(pthis, fn)); }
    void connect(RetType (*fn)(Param1 p1))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type(fn)); }
    template < class X, class Y >
    bool disconnect(const Y *pthis, RetType (X::* fn)(Param1 p1) const)
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    bool disconnect(Y *pthis, RetType (X::* fn)(Param1 p1))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type(pthis, fn)); }
    bool disconnect(RetType (*fn)(Param1 p1))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type(fn)); }
    void operator() (Param1 p1) const
    {
      auto_cs_t<CritSect> lock(this->cs);
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) (*iter)(p1);
    }
    SlotT invoke(Param1 p1) const
    {
      auto_cs_t<CritSect> lock(this->cs);
      SlotT slot;
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) slot(detail::caller1<Param1, RetType, typename detail::delegate_base< CritSect, FastDelegate1<Param1, RetType> >::holder_type>((*iter), p1));
      return slot;
    }
  };
  template< class Param1, class Param2, 
  class RetType=detail::DefaultVoid,
  class CritSect = DEFAULT_CRITICAL_SECTION,
  class SlotT=default_slot<RetType> >
  class delegate2 :
    detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >
  {
  public:
    delegate2() {}
    template < class X, class Y >
    delegate2(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    delegate2(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type(pthis, fn)); }
    delegate2(RetType (*fn)(Param1 p1, Param2 p2))
    { connect_delegate(holder_type(fn)); }
    template < class X, class Y >
    void connect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    void connect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type(pthis, fn)); }
    void connect(RetType (*fn)(Param1 p1, Param2 p2))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type(fn)); }
    template < class X, class Y >
    bool disconnect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2) const)
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    bool disconnect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type(pthis, fn)); }
    bool disconnect(RetType (*fn)(Param1 p1, Param2 p2))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type(fn)); }
    void operator() (Param1 p1, Param2 p2) const
    {
      auto_cs_t<CritSect> lock(this->cs);
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) (*iter)(p1, p2);
    }
    SlotT invoke(Param1 p1, Param2 p2) const
    {
      auto_cs_t<CritSect> lock(this->cs);
      SlotT slot;
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) slot(detail::caller2<Param1, Param2, RetType, typename detail::delegate_base< CritSect, FastDelegate2<Param1, Param2, RetType> >::holder_type>((*iter), p1, p2));
      return slot;
    }
  };
  template< class Param1, class Param2, class Param3, 
  class RetType=detail::DefaultVoid,
  class CritSect = DEFAULT_CRITICAL_SECTION,
  class SlotT=default_slot<RetType> >
  class delegate3 :
    detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >
  {
  public:
    delegate3() {}
    template < class X, class Y >
    delegate3(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    delegate3(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type(pthis, fn)); }
    delegate3(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type(fn)); }
    template < class X, class Y >
    void connect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    void connect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type(pthis, fn)); }
    void connect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type(fn)); }
    template < class X, class Y >
    bool disconnect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3) const)
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    bool disconnect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type(pthis, fn)); }
    bool disconnect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type(fn)); }
    void operator() (Param1 p1, Param2 p2, Param3 p3) const
    {
      auto_cs_t<CritSect> lock(this->cs);
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) (*iter)(p1, p2, p3);
    }
    SlotT invoke(Param1 p1, Param2 p2, Param3 p3) const
    {
      auto_cs_t<CritSect> lock(this->cs);
      SlotT slot;
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) slot(detail::caller3<Param1, Param2, Param3, RetType, typename detail::delegate_base< CritSect, FastDelegate3<Param1, Param2, Param3, RetType> >::holder_type>((*iter), p1, p2, p3));
      return slot;
    }
  };
  template< class Param1, class Param2, class Param3, class Param4, 
  class RetType=detail::DefaultVoid,
  class CritSect = DEFAULT_CRITICAL_SECTION,
  class SlotT=default_slot<RetType> >
  class delegate4 :
    detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >
  {
  public:
    delegate4() {}
    template < class X, class Y >
    delegate4(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    delegate4(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type(pthis, fn)); }
    delegate4(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type(fn)); }
    template < class X, class Y >
    void connect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    void connect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type(pthis, fn)); }
    void connect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4))
    { connect_delegate(holder_type(fn)); }
    template < class X, class Y >
    bool disconnect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const)
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    bool disconnect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type(pthis, fn)); }
    bool disconnect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type(fn)); }
    void operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4) const
    {
      auto_cs_t<CritSect> lock(this->cs);
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) (*iter)(p1, p2, p3, p4);
    }
    SlotT invoke(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const
    {
      auto_cs_t<CritSect> lock(this->cs);
      SlotT slot;
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) slot(detail::caller4<Param1, Param2, Param3, Param4, RetType, typename detail::delegate_base< CritSect, FastDelegate4<Param1, Param2, Param3, Param4, RetType> >::holder_type>((*iter), p1, p2, p3, p4));
      return slot;
    }
  };
  template< class Param1, class Param2, class Param3, class Param4, class Param5, 
  class RetType=detail::DefaultVoid,
  class CritSect = DEFAULT_CRITICAL_SECTION,
  class SlotT=default_slot<RetType> >
  class delegate5 :
    detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >
  {
  public:
    delegate5() {}
    template < class X, class Y >
    delegate5(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    delegate5(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type(pthis, fn)); }
    delegate5(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5))
    { connect_delegate(holder_type(fn)); }
    template < class X, class Y >
    void connect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    void connect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type(pthis, fn)); }
    void connect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5))
    { connect_delegate(holder_type(fn)); }
    template < class X, class Y >
    bool disconnect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const)
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    bool disconnect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type(pthis, fn)); }
    bool disconnect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type(fn)); }
    void operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const
    {
      auto_cs_t<CritSect> lock(this->cs);
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) (*iter)(p1, p2, p3, p4, p5);
    }
    SlotT invoke(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const
    {
      auto_cs_t<CritSect> lock(this->cs);
      SlotT slot;
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) slot(detail::caller5<Param1, Param2, Param3, Param4, Param5, RetType, typename detail::delegate_base< CritSect, FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType> >::holder_type>((*iter), p1, p2, p3, p4, p5));
      return slot;
    }
  };
  template< class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, 
  class RetType=detail::DefaultVoid,
  class CritSect = DEFAULT_CRITICAL_SECTION,
  class SlotT=default_slot<RetType> >
  class delegate6 :
    detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >
  {
  public:
    delegate6() {}
    template < class X, class Y >
    delegate6(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    delegate6(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type(pthis, fn)); }
    delegate6(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type(fn)); }
    template < class X, class Y >
    void connect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    void connect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type(pthis, fn)); }
    void connect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type(fn)); }
    template < class X, class Y >
    bool disconnect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const)
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    bool disconnect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type(pthis, fn)); }
    bool disconnect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type(fn)); }
    void operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const
    {
      auto_cs_t<CritSect> lock(this->cs);
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) (*iter)(p1, p2, p3, p4, p5, p6);
    }
    SlotT invoke(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const
    {
      auto_cs_t<CritSect> lock(this->cs);
      SlotT slot;
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) slot(detail::caller6<Param1, Param2, Param3, Param4, Param5, Param6, RetType, typename detail::delegate_base< CritSect, FastDelegate6<Param1, Param2, Param3, Param4, Param5, Param6, RetType> >::holder_type>((*iter), p1, p2, p3, p4, p5, p6));
      return slot;
    }
  };
  template< class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7, 
  class RetType=detail::DefaultVoid,
  class CritSect = DEFAULT_CRITICAL_SECTION,
  class SlotT=default_slot<RetType> >
  class delegate7 :
    detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >
  {
  public:
    delegate7() {}
    template < class X, class Y >
    delegate7(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    delegate7(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type(pthis, fn)); }
    delegate7(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7))
    { connect_delegate(holder_type(fn)); }
    template < class X, class Y >
    void connect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const)
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    void connect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type(pthis, fn)); }
    void connect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7))
    { connect_delegate(typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type(fn)); }
    template < class X, class Y >
    bool disconnect(const Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const)
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type(pthis, fn)); }
    template < class X, class Y >
    bool disconnect(Y *pthis, RetType (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type(pthis, fn)); }
    bool disconnect(RetType (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7))
    { return disconnect_delegate(typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type(fn)); }
    void operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const
    {
      auto_cs_t<CritSect> lock(this->cs);
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) (*iter)(p1, p2, p3, p4, p5, p6, p7);
    }
    SlotT invoke(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const
    {
      auto_cs_t<CritSect> lock(this->cs);
      SlotT slot;
	  typename std::vector<typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type>::const_iterator iter = this->vec_subscribers.begin();
      for (;iter != this->vec_subscribers.end();++iter) slot(detail::caller7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType, typename detail::delegate_base< CritSect, FastDelegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType> >::holder_type>((*iter), p1, p2, p3, p4, p5, p6, p7));
      return slot;
    }
  };

  template <typename Function> class delegate;

  template <typename R>
  class delegate< R () > 
    : public delegate0<R>
  {    
  public:
    typedef delegate0<R> base_type;

    delegate() : base_type() {}
    template < class X, class Y >
    delegate(const Y *pthis, R (X::* fn)() const) : base_type(pthis, fn) {}
    template < class X, class Y >
    delegate(Y *pthis, R (X::* fn)()) : base_type(pthis, fn) {}
    delegate(R (*fn)()) : base_type(fn) {}
  };
  template <typename R, class Param1>
  class delegate< R (Param1) >
    : public delegate1<Param1, R>
  {
  public:
    typedef delegate1<Param1, R> base_type;
    delegate() : base_type() {}
    template < class X, class Y >
    delegate(const Y *pthis, R (X::* fn)(Param1 p1) const) : base_type(pthis, fn) {}
    template < class X, class Y >
    delegate(Y *pthis, R (X::* fn)(Param1 p1)) : base_type(pthis, fn) {}
    delegate(R (*fn)(Param1 p1)) : base_type(fn) {}
  };
  template <typename R, class Param1, class Param2>
  class delegate< R (Param1, Param2) >
    : public delegate2<Param1, Param2, R>
  {
  public:
    typedef delegate2<Param1, Param2, R> base_type;
    delegate() : base_type() {}
    template < class X, class Y >
    delegate(const Y *pthis, R (X::* fn)(Param1 p1, Param2 p2) const) : base_type(pthis, fn) {}
    template < class X, class Y >
    delegate(Y *pthis, R (X::* fn)(Param1 p1, Param2 p2)) : base_type(pthis, fn) {}
    delegate(R (*fn)(Param1 p1, Param2 p2)) : base_type(fn) {}
  };
  template <typename R, class Param1, class Param2, class Param3>
  class delegate< R (Param1, Param2, Param3) >
    : public delegate3<Param1, Param2, Param3, R>
  {
  public:
    typedef delegate3<Param1, Param2, Param3, R> base_type;
    delegate() : base_type() {}
    template < class X, class Y >
    delegate(const Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3) const) : base_type(pthis, fn) {}
    template < class X, class Y >
    delegate(Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3)) : base_type(pthis, fn) {}
    delegate(R (*fn)(Param1 p1, Param2 p2, Param3 p3)) : base_type(fn) {}
  };
  template <typename R, class Param1, class Param2, class Param3, class Param4>
  class delegate< R (Param1, Param2, Param3, Param4) >
    : public delegate4<Param1, Param2, Param3, Param4, R>
  {
  public:
    typedef delegate4<Param1, Param2, Param3, Param4, R> base_type;
    delegate() : base_type() {}
    template < class X, class Y >
    delegate(const Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const) : base_type(pthis, fn) {}
    template < class X, class Y >
    delegate(Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) : base_type(pthis, fn) {}
    delegate(R (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) : base_type(fn) {}
  };
  template <typename R, class Param1, class Param2, class Param3, class Param4, class Param5>
  class delegate< R (Param1, Param2, Param3, Param4, Param5) >
    : public delegate5<Param1, Param2, Param3, Param4, Param5, R>
  {
  public:
    typedef delegate5<Param1, Param2, Param3, Param4, Param5, R> base_type;
    delegate() : base_type() {}
    template < class X, class Y >
    delegate(const Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const) : base_type(pthis, fn) {}
    template < class X, class Y >
    delegate(Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) : base_type(pthis, fn) {}
    delegate(R (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) : base_type(fn) {}
  };
  template <typename R, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6>
  class delegate< R (Param1, Param2, Param3, Param4, Param5, Param6) >
    : public delegate6<Param1, Param2, Param3, Param4, Param5, Param6, R>
  {
  public:
    typedef delegate6<Param1, Param2, Param3, Param4, Param5, Param6, R> base_type;
    delegate() : base_type() {}
    template < class X, class Y >
    delegate(const Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const) : base_type(pthis, fn) {}
    template < class X, class Y >
    delegate(Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6)) : base_type(pthis, fn) {}
    delegate(R (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6)) : base_type(fn) {}
  };
  template <typename R, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7>
  class delegate< R (Param1, Param2, Param3, Param4, Param5, Param6, Param7) >
    : public delegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, R>
  {
  public:
    typedef delegate7<Param1, Param2, Param3, Param4, Param5, Param6, Param7, R> base_type;
    delegate() : base_type() {}
    template < class X, class Y >
    delegate(const Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const) : base_type(pthis, fn) {}
    template < class X, class Y >
    delegate(Y *pthis, R (X::* fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7)) : base_type(pthis, fn) {}
    delegate(R (*fn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7)) : base_type(fn) {}
  };
}

#endif /* MULTICATDELEGATE_H */