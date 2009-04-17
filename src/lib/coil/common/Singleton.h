// -*- C++ -*-
/*!
 * @file Singleton.h
 * @brief Singleton template class
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2009
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#ifndef COIL_SINGLETON_H
#define COIL_SINGLETON_H

#include <coil/Mutex.h>
#include <coil/Guard.h>

namespace coil
{
  /*!
   * @if jp
   * @class Singleton クラステンプレート
   *
   * このテンプレートは、任意のクラスを Singleton にするテンプレートである。
   * 以下のようにして使用する。
   *
   * class A { // };
   * typedef coil::Singleton<A> A_;
   *
   * 任意の場所で
   *
   * A& a(A_:instance()); // a は A の唯一のインスタンスが入る
   *
   * ただし、A自体のコンストラクタは使用できるので、同一のソースで、
   *
   * A* a = new A();
   *
   * のようにすることもできるため、注意が必要である。
   * 対象とするクラスを new することを禁止するためには、以下のように、
   * 対象クラスで Singelton を継承 (CRTP) し friend 宣言する必要がある。
   *
   * class A
   *  : public coil::Singleton<A>
   * {
   * public:
   * private:
   *   A(){}
   * 
   *   friend class coil::Singelton<A>;
   * };
   *
   * こうすることで、
   *
   * A* a = new A(); // は禁止される
   * A& a(A::instance()); // が唯一のインスタンスを得る唯一の方法
   *
   * @else
   * @class Singleton class template
   *
   * This class template makes any classed into Singleton classes.
   * Usage is as follows.
   *
   * class A { // };
   * typedef coil::Singleton<A> A_;
   *
   * In the any places,
   * A& a(A_:instance()); // a has singular instance of A
   *
   * Since the constructor of A is still public, however, user can
   * create other instance of A as follows.
   *
   * A* a = new A();
   *
   * If you want to prohibit user from creating new instance, please
   * inherit Singleton class (CRTP) and declare it as a friend class
   * in the target class.
   *
   * class A
   *  : public coil::Singleton<A>
   * {
   * public:
   * private:
   *   A(){}
   * 
   *   friend class coil::Singelton<A>;
   * };
   *
   * A* a = new A(); // compile error
   * A& a(A::instance()); // This is the only method to get unique instance
   *
   * @endif
   */
  template <class SingletonClass>
  class Singleton
  {
  public:
    static SingletonClass& instance()
    {
      static coil::Mutex mutex;
      static SingletonClass* instance_(0);

      // DLC pattern
      if (!instance_)
      {
        coil::Guard<coil::Mutex> guard(mutex);
	if (!instance_)
	  {
	    instance_ = new SingletonClass();
	  }
      }
      return *instance_;
    }

  protected:
    Singleton(){};
    ~Singleton(){};
    Singleton(const Singleton& x);
    Singleton& operator=(const Singleton& x);
  };
}; // namepsace coil

#endif // COIL_SINGLETON_H
