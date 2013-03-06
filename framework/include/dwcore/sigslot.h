//
// Written by Sarah Thompson (sarah@telergy.com) 2002.
//
// License: Public domain. You are free to use this code however you like, with the proviso that
//          the author takes on no responsibility or liability for any use.
//
// QUICK DOCUMENTATION
//
//				(see also the full documentation at http://sigslot.sourceforge.net/)
//
//		#define switches
//			SIGSLOT_PURE_ISO			- Define this to force ISO C++ compliance. This also disables
//										  all of the thread safety support on platforms where it is
//										  available.
//
//			SIGSLOT_USE_POSIX_THREADS	- Force use of Posix threads when using a C++ compiler other than
//										  gcc on a platform that supports Posix threads. (When using gcc,
//										  this is the default - use SIGSLOT_PURE_ISO to disable this if
//										  necessary)
//
//			SIGSLOT_DEFAULT_MT_POLICY	- Where thread support is enabled, this defaults to multi_threaded_global.
//										  Otherwise, the default is single_threaded. #define this yourself to
//										  override the default. In pure ISO mode, anything other than
//										  single_threaded will cause a compiler error.
//
//		PLATFORM NOTES
//
//			Win32						- On Win32, the WIN32 symbol must be #defined. Most mainstream
//										  compilers do this by default, but you may need to define it
//										  yourself if your build environment is less standard. This causes
//										  the Win32 thread support to be compiled in and used automatically.
//
//			Unix/Linux/BSD, etc.		- If you're using gcc, it is assumed that you have Posix threads
//										  available, so they are used automatically. You can override this
//										  (as under Windows) with the SIGSLOT_PURE_ISO switch. If you're using
//										  something other than gcc but still want to use Posix threads, you
//										  need to #define SIGSLOT_USE_POSIX_THREADS.
//
//			ISO C++						- If none of the supported platforms are detected, or if
//										  SIGSLOT_PURE_ISO is defined, all multithreading support is turned off,
//										  along with any code that might cause a pure ISO C++ environment to
//										  complain. Before you ask, gcc -ansi -pedantic won't compile this
//										  library, but gcc -ansi is fine. Pedantic mode seems to throw a lot of
//										  errors that aren't really there. If you feel like investigating this,
//										  please contact the author.
//
//
//		THREADING MODES
//
//			single_threaded				- Your program is assumed to be single threaded from the point of view
//										  of signal/slot usage (i.e. all objects using signals and slots are
//										  created and destroyed from a single thread). Behaviour if objects are
//										  destroyed concurrently is undefined (i.e. you'll get the occasional
//										  segmentation fault/memory exception).
//
//			multi_threaded_global		- Your program is assumed to be multi threaded. Objects using signals and
//										  slots can be safely created and destroyed from any thread, even when
//										  connections exist. In multi_threaded_global mode, this is achieved by a
//										  single global mutex (actually a critical section on Windows because they
//										  are faster). This option uses less OS resources, but results in more
//										  opportunities for contention, possibly resulting in more context switches
//										  than are strictly necessary.
//
//			multi_threaded_local		- Behaviour in this mode is essentially the same as multi_threaded_global,
//										  except that each signal, and each object that inherits has_slots, all
//										  have their own mutex/critical section. In practice, this means that
//										  mutex collisions (and hence context switches) only happen if they are
//										  absolutely essential. However, on some platforms, creating a lot of
//										  mutexes can slow down the whole OS, so use this option with care.
//
//		USING THE LIBRARY
//
//			See the full documentation at http://sigslot.sourceforge.net/
//
//

#ifndef SIGSLOT_H__
#define SIGSLOT_H__

//do not use stl list
//#define _SIGSLOT_USING_STL

#ifdef _SIGSLOT_USING_STL
    #include <list>
    #define SigslotList std::list
#else
    #include <dwcore/dwlinkedlist.h>
    #define SigslotList DwLinkedList
#endif

#define ASSERT(x) assert(x)

#define _SIGSLOT_SINGLE_THREADED

#ifndef SIGSLOT_DEFAULT_MT_POLICY
#	ifdef _SIGSLOT_SINGLE_THREADED
#		define SIGSLOT_DEFAULT_MT_POLICY single_threaded
#	else
#		define SIGSLOT_DEFAULT_MT_POLICY multi_threaded_local
#	endif
#endif

// This is a very common pattern for us
#pragma warning(disable: 4355) // 'this' : used in base member initializer list

namespace sigslot {

	class single_threaded
	{
		public:
			single_threaded()
			{
				;
			}

			virtual ~single_threaded()
			{
				;
			}

			virtual void lock()
			{
				;
			}

			virtual void unlock()
			{
				;
			}
	};

	class has_slots
	{
		private:
			class slot_wrapper
			{
				public:
					slot_wrapper(has_slots *pObject)
						:m_object(pObject), m_refcount(0)
					{
					}

					~slot_wrapper()
					{
					}

					has_slots* get() const
					{
						return m_object;
					}

					long addref()
					{
						return ::InterlockedIncrement(&m_refcount);
					}

					long release()
					{
						long val = ::InterlockedDecrement(&m_refcount);
						if(0 == val)
						{
							delete this;
						}
						return val;
					}

				private:
					friend class has_slots;
					has_slots *m_object;
					mutable long m_refcount;
			};
		public:
			class slot_holder
			{
				public:
					slot_holder(slot_wrapper *target = NULL)
						:m_target(target)
					{
						if(m_target)
						{
							m_target->addref();
						}
					}

					slot_holder(const slot_holder &rhs)
						:m_target(rhs.m_target)
					{
						if(m_target)
						{
							m_target->addref();
						}
					}

					~slot_holder()
					{
						if(m_target)
						{
							m_target->release();
						}
					}

					slot_holder& operator=(slot_wrapper *target)
					{
						if(target != m_target)
						{
							if(m_target)
							{
								m_target->release();
							}
							m_target = target;
							if(m_target)
							{
								m_target->addref();
							}
						}
						return *this;
					}

					slot_holder& operator=(const slot_holder &rhs)
					{
						return (*this) = rhs.m_target;
					}

					operator slot_wrapper*()
					{
						return m_target;
					}

					slot_wrapper* operator->() const
					{
						return m_target;
					}


					operator const slot_wrapper*() const
					{
						return m_target;
					}

				private:
					slot_wrapper *m_target;
			};

		public:

			has_slots()
				:receiever(new slot_wrapper(this))
			{
			}

			virtual ~has_slots()
			{
				receiever->m_object = NULL;
			}

			const slot_holder& get_receiver()
			{
				return receiever;
			}

			void disconnect_all() {
				receiever->m_object = NULL;
				receiever = new slot_wrapper(this);
			}

		private:
			slot_holder receiever;
	};

	typedef has_slots::slot_holder slot_holder;

	class _connection_base
	{
		public:
			_connection_base()
				:m_pobject(), m_emitter(NULL)
			{
			}

			_connection_base(const slot_holder &slot, void *emitter)
				:m_pobject(slot), m_emitter(emitter)
			{
			}

			_connection_base(const _connection_base &rhs)
				:m_pobject(rhs.m_pobject), m_emitter(rhs.m_emitter)
			{
			}

			virtual ~_connection_base(){}
			virtual _connection_base* clone() = 0;
			virtual _connection_base* duplicate(has_slots* pnewdest) = 0;
			virtual bool equal(const _connection_base *rhs) const = 0;

			has_slots* getdest() const
			{
				return m_pobject->get();
			}

			void* get_emitter() const
			{
				return m_emitter;
			}

			class connection_holder
			{
				public:
					connection_holder(const _connection_base *conn = NULL)
						:m_conn(const_cast<_connection_base*>(conn)) {}
					connection_holder(const connection_holder &rhs)
						:m_conn(rhs.m_conn->clone()) {}
					~connection_holder()
					{
						delete m_conn;
					}
					_connection_base* operator->()
					{
						return m_conn;
					}

					const _connection_base* operator->() const
					{
						return m_conn;
					}

					operator _connection_base*()
					{
						return m_conn;
					}

					operator const _connection_base*() const
					{
						return m_conn;
					}
				private:
					_connection_base *m_conn;
			};
		protected:
			slot_holder m_pobject;
			void *m_emitter;
	};

	template<class mt_policy>
		class _signal_base : public mt_policy
	{
		public:
			typedef SigslotList<_connection_base::connection_holder>  connections_list;

			_signal_base()
			{
			}

			_signal_base(const _signal_base& s)
				:mt_policy(s)
			{
				s.compact();
				typename connections_list::const_iterator it = s.m_connected_slots.begin();
				typename connections_list::const_iterator itEnd = s.m_connected_slots.end();
				while(it != itEnd)
				{
					m_connected_slots.push_back((*it)->clone());
					++it;
				}
			}

			~_signal_base()
			{
				disconnect_all();
			}

			void disconnect_all()
			{
				typename connections_list::const_iterator it = m_connected_slots.begin();
				typename connections_list::const_iterator itEnd = m_connected_slots.end();
				m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
			}

			void disconnect(has_slots* pclass)
			{
				typename connections_list::iterator it = m_connected_slots.begin();
				typename connections_list::iterator itEnd = m_connected_slots.end();

				while(it != itEnd)
				{
					has_slots *dest = (*it)->getdest();
					if(dest == NULL || dest == pclass)
					{
						it = m_connected_slots.erase(it);
						continue;
					}
					++it;
				}
			}

			void slot_duplicate(const has_slots* oldtarget, has_slots* newtarget)
			{
				compact();
				typename connections_list::iterator it = m_connected_slots.begin();
				typename connections_list::iterator itEnd = m_connected_slots.end();

				while(it != itEnd)
				{
					if((*it)->getdest() == oldtarget)
					{
						m_connected_slots.push_back((*it)->duplicate(newtarget));
					}

					++it;
				}
			}

			//compact out dead slots
			//NOTICE: this method is unsafe, lock before calling it
			void compact()
			{
				typename connections_list::iterator itNext;
				typename connections_list::iterator itEnd = m_connected_slots.end();
                typename connections_list::iterator it = m_connected_slots.begin();

				while(it != itEnd)
				{
					itNext = it;
					++itNext;

					if((*it)->getdest() == NULL)
					{
						m_connected_slots.erase(it);
					}

					it = itNext;
				}
			}

			//debug routine, check if the slot is connected twice
			//NOTICE: this method is unsafe, lock before calling it
			bool contains(_connection_base *new_bie)
			{
				typename connections_list::const_iterator it = m_connected_slots.begin();
				typename connections_list::const_iterator itEnd = m_connected_slots.end();

				while(it != itEnd)
				{
					if((*it)->equal(new_bie))
					{
						return true;
					}
					
					++it;
				}
				return false;
			}
			//return by value, force it to be copied
			connections_list connection_copy()
			{
				compact();
				return m_connected_slots;
			}


		protected:
			connections_list m_connected_slots;
	};

	template<class dest_type, class functype>
		class _connection : public _connection_base
	{
		public:
			typedef _connection<dest_type, functype> my_type;
		public:
			_connection()
				:_connection_base()
			{
				m_pmemfun = NULL;
			}

			_connection(dest_type* pobject, functype pmemfun, void *emitter)
				:_connection_base(pobject->get_receiver(), emitter)
			{
				m_pmemfun = pmemfun;
			}

			_connection(const _connection &rhs)
				:_connection_base(rhs)
			{
				m_pmemfun = rhs.m_pmemfun;
			}

			virtual _connection_base* clone()
			{
				return new _connection(*this);
			}

			virtual _connection_base* duplicate(has_slots* pnewdest)
			{
				return new _connection(static_cast<dest_type*>(pnewdest), m_pmemfun, this->get_emitter());
			}

			dest_type* get_dest() const
			{
				return static_cast<dest_type*>(this->getdest());
			}

			functype get_func() const
			{
				return m_pmemfun;
			}

			virtual bool equal(const _connection_base *rhs) const
			{
				return this->getdest() == rhs->getdest()
					&& this->m_pmemfun == ((my_type*)rhs)->m_pmemfun;
			}

		private:
			functype m_pmemfun;
	};

	template<class Ty_>
	struct emit_helper;

	template<class dest_type>
	struct emit_helper<void (dest_type::*)()>
	{
		static void emit(const _connection_base *conn_base)
		{
			typedef void (dest_type::* functype)();
			typedef const _connection<dest_type, functype> conn_type;
			conn_type *conn = static_cast<conn_type*>(conn_base);
			dest_type *dest = conn->get_dest();
			if(dest)
			{
				(dest->*(conn->get_func()))();
			}
		}
	};

	template<class dest_type, class arg1_type>
	struct emit_helper<void (dest_type::*)(arg1_type)>
	{
		static void emit(const _connection_base *conn_base, arg1_type arg1)
		{
			typedef void (dest_type::* functype)(arg1_type);
			typedef const _connection<dest_type, functype> conn_type;
			conn_type *conn = static_cast<conn_type*>(conn_base);
			dest_type *dest = conn->get_dest();
			if(dest)
			{
				(dest->*(conn->get_func()))(arg1);
			}
		}
	};

	template<class dest_type, class arg1_type, class arg2_type>
	struct emit_helper<void (dest_type::*)(arg1_type, arg2_type)>
	{
		static void emit(const _connection_base *conn_base, arg1_type arg1, arg2_type arg2)
		{
			typedef void (dest_type::* functype)(arg1_type, arg2_type);
			typedef const _connection<dest_type, functype> conn_type;
			conn_type *conn = static_cast<conn_type*>(conn_base);
			dest_type *dest = conn->get_dest();
			if(dest)
			{
				(dest->*(conn->get_func()))(arg1, arg2);
			}
		}
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type>
	struct emit_helper<void (dest_type::*)(arg1_type, arg2_type, arg3_type)>
	{
		static void emit(const _connection_base *conn_base, arg1_type arg1, arg2_type arg2, arg3_type arg3)
		{
			typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type);
			typedef const _connection<dest_type, functype> conn_type;
			conn_type *conn = static_cast<conn_type*>(conn_base);
			dest_type *dest = conn->get_dest();
			if(dest)
			{
				(dest->*(conn->get_func()))(arg1, arg2, arg3);
			}
		}
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type, class arg4_type>
	struct emit_helper<void (dest_type::*)(arg1_type, arg2_type, arg3_type, arg4_type)>
	{
		static void emit(const _connection_base *conn_base, arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4)
		{
			typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type);
			typedef const _connection<dest_type, functype> conn_type;
			conn_type *conn = static_cast<conn_type*>(conn_base);
			dest_type *dest = conn->get_dest();
			if(dest)
			{
				(dest->*(conn->get_func()))(arg1, arg2, arg3, arg4);
			}
		}
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type,
		class arg4_type, class arg5_type>
	struct emit_helper<void (dest_type::*)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type)>
	{
		static void emit(const _connection_base *conn_base, arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5)
		{
			typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type);
			typedef const _connection<dest_type, functype> conn_type;
			conn_type *conn = static_cast<conn_type*>(conn_base);
			dest_type *dest = conn->get_dest();
			if(dest)
			{
				(dest->*(conn->get_func()))(arg1, arg2, arg3, arg4, arg5);
			}
		}
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type,
		class arg4_type, class arg5_type, class arg6_type>
	struct emit_helper<void (dest_type::*)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type)>
	{
		static void emit(const _connection_base *conn_base, arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5, arg6_type arg6)
		{
			typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type);
			typedef const _connection<dest_type, functype> conn_type;
			conn_type *conn = static_cast<conn_type*>(conn_base);
			dest_type *dest = conn->get_dest();
			if(dest)
			{
				(dest->*(conn->get_func()))(arg1, arg2, arg3, arg4, arg5, arg6);
			}
		}
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type,
		class arg4_type, class arg5_type, class arg6_type, class arg7_type>
	struct emit_helper<void (dest_type::*)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type)>
	{
		static void emit(const _connection_base *conn_base, arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5, arg6_type arg6, arg7_type arg7)
		{
			typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type);
			typedef const _connection<dest_type, functype> conn_type;
			conn_type *conn = static_cast<conn_type*>(conn_base);
			dest_type *dest = conn->get_dest();
			if(dest)
			{
				(dest->*(conn->get_func()))(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			}
		}
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type,
		class arg4_type, class arg5_type, class arg6_type, class arg7_type, class arg8_type>
	struct emit_helper<void (dest_type::*)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type)>
	{
		static void emit(const _connection_base *conn_base, arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8)
		{
			typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type);
			typedef const _connection<dest_type, functype> conn_type;
			//由于参数为8个时，提示"conn":未声明的标识符
			conn_type *conn = static_cast<conn_type*>(conn_base);
			dest_type *dest = conn->get_dest();
			if(dest)
			{
				(dest->*(conn->get_func()))(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			}
		}
	};

	template<class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
		class signal0 : public _signal_base<mt_policy>
	{
		typedef _signal_base<mt_policy>	_mybase;
		typedef typename _mybase::connections_list	connections_list;
		typedef bool(*emitter_type)(const _connection_base*);

		public:
		signal0()
		{
		}

		signal0(const signal0 &s)
			: _signal_base<mt_policy>(s)
		{
		}

		template<class dest_type>
			void connect(dest_type* pclass, void (dest_type::*pmemfun)())
			{
				typedef void (dest_type::* functype)();
				typedef _connection<dest_type, functype> conn_type;
				compact();
				conn_type *conn = new conn_type(pclass, pmemfun, (void*)&emit_helper<functype>::emit);
				ASSERT(!this->contains(conn));
				m_connected_slots.push_back(conn);
			}

		void operator()()
		{
			const connections_list &connections = connection_copy();
			typename connections_list::const_iterator itNext, it = connections.begin();
			typename connections_list::const_iterator itEnd = connections.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;
				emitter_type emitter = static_cast<emitter_type>((*it)->get_emitter());
				emitter(*it);
				it = itNext;
			}
		}

		void emit()
		{
			(*this)();
		}
	};

	template<class arg1_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
		class signal1 : public _signal_base<mt_policy>
	{
		typedef _signal_base<mt_policy>	_mybase;
		typedef typename _mybase::connections_list	connections_list;
		typedef bool(*emitter_type)(const _connection_base*, arg1_type);

		public:
		signal1()
		{
		}

		signal1(const signal1 &s)
			: _signal_base<mt_policy>(s)
		{
		}

		template<class dest_type>
			void connect(dest_type* pclass, void (dest_type::*pmemfun)(arg1_type))
			{
				typedef void (dest_type::* functype)(arg1_type);
				typedef _connection<dest_type, functype> conn_type;
				compact();
				conn_type *conn = new conn_type(pclass, pmemfun, (void*)&emit_helper<functype>::emit);
				ASSERT(!this->contains(conn));
				m_connected_slots.push_back(conn);
			}

		void operator()(arg1_type arg1)
		{
			const connections_list &connections = connection_copy();
			typename connections_list::const_iterator itNext, it = connections.begin();
			typename connections_list::const_iterator itEnd = connections.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;
				emitter_type emitter = static_cast<emitter_type>((*it)->get_emitter());
				emitter(*it, arg1);
				it = itNext;
			}
		}

		void emit(arg1_type arg1)
		{
			(*this)(arg1);
		}
	};

	template<class arg1_type, class arg2_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
		class signal2 : public _signal_base<mt_policy>
	{
		typedef _signal_base<mt_policy>	_mybase;
		typedef typename _mybase::connections_list	connections_list;
		typedef bool(*emitter_type)(const _connection_base*, arg1_type, arg2_type);

		public:
		signal2()
		{
		}

		signal2(const signal2& s)
			: _signal_base<mt_policy>(s)
		{
		}

		template<class dest_type>
			void connect(dest_type* pclass, void (dest_type::*pmemfun)(arg1_type, arg2_type))
			{
				typedef void (dest_type::* functype)(arg1_type, arg2_type);
				typedef _connection<dest_type, functype> conn_type;
				compact();
				conn_type *conn = new conn_type(pclass, pmemfun, (void*)&emit_helper<functype>::emit);
				ASSERT(!this->contains(conn));
				m_connected_slots.push_back(conn);
			}

		void operator()(arg1_type arg1, arg2_type arg2)
		{
			const connections_list &connections = connection_copy();
			typename connections_list::const_iterator itNext, it = connections.begin();
			typename connections_list::const_iterator itEnd = connections.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;
				emitter_type emitter = static_cast<emitter_type>((*it)->get_emitter());
				emitter(*it, arg1, arg2);
				it = itNext;
			}
		}

		void emit(arg1_type arg1, arg2_type arg2)
		{
			(*this)(arg1, arg2);
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
		class signal3 : public _signal_base<mt_policy>
	{
		typedef _signal_base<mt_policy>	_mybase;
		typedef typename _mybase::connections_list	connections_list;
		typedef bool(*emitter_type)(const _connection_base*, arg1_type, arg2_type, arg3_type);

		public:
		signal3()
		{
		}

		signal3(const signal3 &s)
			: _signal_base<mt_policy>(s)
		{
		}

		template<class dest_type>
			void connect(dest_type* pclass, void (dest_type::*pmemfun)(arg1_type, arg2_type, arg3_type))
			{
				typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type);
				typedef _connection<dest_type, functype> conn_type;
				compact();
				conn_type *conn = new conn_type(pclass, pmemfun, (void*)&emit_helper<functype>::emit);
				ASSERT(!this->contains(conn));
				m_connected_slots.push_back(conn);
			}

		void operator()(arg1_type arg1, arg2_type arg2, arg3_type arg3)
		{
			const connections_list &connections = connection_copy();
			typename connections_list::const_iterator itNext, it = connections.begin();
			typename connections_list::const_iterator itEnd = connections.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;
				emitter_type emitter = static_cast<emitter_type>((*it)->get_emitter());
				emitter(*it, arg1, arg2, arg3);
				it = itNext;
			}
		}

		void emit(arg1_type arg1, arg2_type arg2, arg3_type arg3)
		{
			(*this)(arg1, arg2, arg3);
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
		class signal4 : public _signal_base<mt_policy>
	{
		typedef _signal_base<mt_policy>	_mybase;
		typedef typename _mybase::connections_list	connections_list;
		typedef bool(*emitter_type)(const _connection_base*, arg1_type, arg2_type, arg3_type, arg4_type);

		public:
		signal4()
		{
		}

		signal4(const signal4 &s)
			: _signal_base<mt_policy>(s)
		{
		}

		template<class dest_type>
			void connect(dest_type* pclass, void (dest_type::*pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type))
			{
				typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type);
				typedef _connection<dest_type, functype> conn_type;
				compact();
				conn_type *conn = new conn_type(pclass, pmemfun, (void*)&emit_helper<functype>::emit);
				ASSERT(!this->contains(conn));
				m_connected_slots.push_back(conn);
			}

		void operator()(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4)
		{
			const connections_list &connections = connection_copy();
			typename connections_list::const_iterator itNext, it = connections.begin();
			typename connections_list::const_iterator itEnd = connections.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;
				emitter_type emitter = static_cast<emitter_type>((*it)->get_emitter());
				emitter(*it, arg1, arg2, arg3, arg4);
				it = itNext;
			}
		}

		void emit(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4)
		{
			(*this)(arg1, arg2, arg3, arg4);
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
		class arg5_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
		class signal5 : public _signal_base<mt_policy>
	{
		typedef _signal_base<mt_policy>	_mybase;
		typedef typename _mybase::connections_list	connections_list;
		typedef bool(*emitter_type)(const _connection_base*, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type);

		public:
		signal5()
		{
		}

		signal5(const signal5 &s)
			: _signal_base<mt_policy>(s)
		{
		}

		template<class dest_type>
			void connect(dest_type* pclass, void (dest_type::*pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type))
			{
				typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type);
				typedef _connection<dest_type, functype> conn_type;
				compact();
				conn_type *conn = new conn_type(pclass, pmemfun, (void*)&emit_helper<functype>::emit);
				ASSERT(!this->contains(conn));
				m_connected_slots.push_back(conn);
			}

		void operator()(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5)
		{
			const connections_list &connections = connection_copy();
			typename connections_list::const_iterator itNext, it = connections.begin();
			typename connections_list::const_iterator itEnd = connections.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;
				emitter_type emitter = static_cast<emitter_type>((*it)->get_emitter());
				emitter(*it, arg1, arg2, arg3, arg4, arg5);
				it = itNext;
			}
		}

		void emit(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5)
		{
			(*this)(arg1, arg2, arg3, arg4, arg5);
		}
	};


	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
		class arg5_type, class arg6_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
		class signal6 : public _signal_base<mt_policy>
	{
		typedef _signal_base<mt_policy>	_mybase;
		typedef typename _mybase::connections_list	connections_list;
		typedef bool(*emitter_type)(const _connection_base*, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type);

		public:
		signal6()
		{
		}

		signal6(const signal6 &s)
			: _signal_base<mt_policy>(s)
		{
		}

		template<class dest_type>
			void connect(dest_type* pclass, void (dest_type::*pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type))
			{
				typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type);
				typedef _connection<dest_type, functype> conn_type;
				compact();
				conn_type *conn = new conn_type(pclass, pmemfun, (void*)&emit_helper<functype>::emit);
				ASSERT(!this->contains(conn));
				m_connected_slots.push_back(conn);
			}

		void operator()(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5, arg6_type arg6)
		{
			const connections_list &connections = connection_copy();
			typename connections_list::const_iterator itNext, it = connections.begin();
			typename connections_list::const_iterator itEnd = connections.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;
				emitter_type emitter = static_cast<emitter_type>((*it)->get_emitter());
				emitter(*it, arg1, arg2, arg3, arg4, arg5, arg6);
				it = itNext;
			}
		}

		void emit(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5, arg6_type arg6)
		{
			(*this)(arg1, arg2, arg3, arg4, arg5, arg6);
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
		class arg5_type, class arg6_type, class arg7_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
		class signal7 : public _signal_base<mt_policy>
	{
		typedef _signal_base<mt_policy>	_mybase;
		typedef typename _mybase::connections_list	connections_list;
		typedef bool(*emitter_type)(const _connection_base*, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type);

		public:
		signal7()
		{
		}

		signal7(const signal7 &s)
			: _signal_base<mt_policy>(s)
		{
		}

		template<class dest_type>
			void connect(dest_type* pclass, void (dest_type::*pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type))
			{
				typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type);
				typedef _connection<dest_type, functype> conn_type;
				compact();
				conn_type *conn = new conn_type(pclass, pmemfun, (void*)&emit_helper<functype>::emit);
				ASSERT(!this->contains(conn));
				m_connected_slots.push_back(conn);
			}

		void operator()(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5, arg6_type arg6, arg7_type arg7)
		{
			const connections_list &connections = connection_copy();
			typename connections_list::const_iterator itNext, it = connections.begin();
			typename connections_list::const_iterator itEnd = connections.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;
				emitter_type emitter = static_cast<emitter_type>((*it)->get_emitter());
				emitter(*it, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
				it = itNext;
			}
		}

		void emit(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5, arg6_type arg6, arg7_type arg7)
		{
			(*this)(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
		class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
		class signal8 : public _signal_base<mt_policy>
	{
		typedef _signal_base<mt_policy>	_mybase;
		typedef typename _mybase::connections_list	connections_list;
		typedef bool(*emitter_type)(const _connection_base*, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type);

		public:
		signal8()
		{
		}

		signal8(const signal8 &s)
			: _signal_base<mt_policy>(s)
		{
		}

		template<class dest_type>
			void connect(dest_type* pclass, void (dest_type::*pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type))
			{
				typedef void (dest_type::* functype)(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type);
				typedef _connection<dest_type, functype> conn_type;
				compact();
				conn_type *conn = new conn_type(pclass, pmemfun, (void*)&emit_helper<functype>::emit);
				ASSERT(!this->contains(conn));
				m_connected_slots.push_back(conn);
			}

		void operator()(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8)
		{
			const connections_list &connections = connection_copy();
			typename connections_list::const_iterator itNext, it = connections.begin();
			typename connections_list::const_iterator itEnd = connections.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;
				emitter_type emitter = static_cast<emitter_type>((*it)->get_emitter());
				emitter(*it, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
				it = itNext;
			}
		}

		void emit(arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8)
		{
			(*this)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
		}
	};

}; // namespace sigslot

#endif // SIGSLOT_H__

