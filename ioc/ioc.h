/**
 * @file ioc.h
 * @author yueyingcup
 * @date 2015/7/20
 * @brief implementation of ioc in cpp
 * 
 **/


#ifndef  __IOC_H_
#define  __IOC_H_

#ifdef __i386
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <string>

namespace CppUtils {
typedef std::string String;

class StrHashFunc {
public:
     size_t operator()(const String & str) const {
          register size_t hash_value = 0;
          uint32_t len = str.length();
          for (int i = 0; i < len; ++i) {
               hash_value = hash_value * 131 + str[len];
          }
          return hash_value;
     }
};

class Lock {
public:
     Lock() {
          pthread_mutex_init(&_mutex, NULL);
          thread_mutex_lock(&_mutex);
     }

     ~Lock() {
          pthread_mutex_unlock(&_mutex);
          pthread_mutex_destory(&_mutex);
     }
};

template <typename Root>
class IOC {
     typedef void * (*ct_t)();     //memory allocation and construction
     typedef void (*cs_t)(void *); //construction only
     typedef size_t (*gs_t)();     //get the size of the class

     //反转控制的结构体回调
     struct node_t {
          ct_t ct;
          cs_t cs;
          gs_t gs;
     };

     //class metadata map
#ifdef __i386
     typedef std::hash_map<String, node_t, StrHashFunc> HashMap;
#else
     typedef __gnu_cxx::hash_map<String, node_t, StrHashFunc> HashMap;
#endif
     public:
          /**
           * @brief create the instance according to the class name
           *
           * @param [in] name   : const String&     the class name
           * @return Root*   : the instance pointer of the base class
           * @retval NULL for failure
           * @author yueyingcup
           **/
          Root* create(const String &name);

          /**
           * @brief create the instance using the space pointed by the ptr
           *
           * @param [in] name   : const String&     the class name
           * @param [in/out] ptr   : void*     the allocated space
           * @retval 0 for success, failure otherwise
           * @author yueyingcup
           **/
          int construct(const String &name, void *ptr);

          /**
           * @brief get the size of the class
           *
           * @param [in] name   : const String&     the class name
           * @return size_t      the class size
           * @retval 0 for failure
           * @author yueyingcup
           **/
          size_t getSize(const String &name);

          /**
           * @brief destroy the instance pointed by the ptr
           *
           * @param [in/out] ptr   : Root*      the instance of the base class
           * @author yueyingcup
           **/
          void destroy(Root *ptr);

          /**
           * @brief destruct the instance pointed by the ptr
           *
           * @param [in/out] ptr   : Root*     the instance of the base class
           * @author yueyingcup
           **/
          void destruct(Root *ptr);

          /**
           * @brief add the class and the ct, cs, gs (please refer to typedefs above) functions into the ioc
           *
           * @param [in/out] name   : const String&     the class name
           * @param [in/out] ct   : ct_t     memory allocation and construction function
           * @param [in/out] cs   : cs_t     construction only
           * @param [in/out] gs   : gs_t     get the size of the class
           * @retval 0 for success, failure otherwise
           * @author yueyingcup
           **/
          int add(const String &name, ct_t ct, cs_t cs, gs_t gs);

     private:
          HashMap _map;
};
}

//using the 2 macros to activate the ioc function
//generate the ioc functions
#define UB_GEN_IOC_CREATE(ClassName)\
     void * ClassName##_ioc_create_func() { \
          return static_cast<void *> (new ClassName); \
     }\
     void ClassName##_ioc_construct_func(void *p) {\
          ::new(p) ClassName;\
     }\
     size_t ClassName##_ioc_sizeof() {\
          return sizeof(ClassName);\
     }

//register the ioc functions
#define UB_ADD_IOC_CLASS(ioc, ClassName)\
     ioc.add(#ClassName, ClassName##_ioc_create_func, ClassName##_ioc_construct_func, ClassName##_ioc_sizeof);

#include "ioc.cc"

#endif  //__IOC_H_
