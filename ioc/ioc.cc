namespace CppUtils {
template <typename Root>
Root *IOC<Root>::create(const String &str) {
     Lock lock;
     typename HashMap::iterator iter = _map.find(str);
     if (iter == _map.end()) {
          return NULL;
     }
     try {
          Root * root = static_cast<Root*>(iter->second.ct());
          return root;
     } catch (...) {
          // add warning
     }
     return NULL;
}

template <typename Root>
int IOC<Root>::construct(const String &str, void *p)
{
     Lock lock;
     typename HashMap::iterator iter = _map.find(str);
     if (iter == _map.end()) {
          return -1;
     }
     iter->second.cs(p);
     return 0;
}

template <typename Root>
size_t IOC<Root>::getSize(const String &str)
{
     Lock lock;
     typename HashMap::iterator iter = _map.find(str);
     if (iter == _map.end()) {
          return 0;
     }
     return iter->second.gs();
}

template <typename Root>
void IOC<Root>::destroy(Root *ptr)
{
     delete ptr;
}

template <typename Root>
void IOC<Root>::destruct(Root *ptr)
{
     ptr->~Root();
}

template <typename Root>
int IOC<Root>::add(const String &str, ct_t ct, cs_t cs, gs_t gs)
{
     Lock lock;
     try {
          node_t cb;
          cb.ct = ct;
          cb.cs = cs;
          cb.gs = gs;
          _map.insert(std::make_pair(str, cb));
     } catch (...) {
          // add warning
          return -1;
     }
     return 0;
}
}
