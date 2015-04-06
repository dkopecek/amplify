#ifndef AMPUSH_TYPEDEFS_HPP
#define AMPUSH_TYPEDEFS_HPP

#include <build-config.h>
#include <memory>
#include <map>
#include <vector>

namespace Amplify
{
  template<typename T>
  using Pointer = std::shared_ptr<T>;

  template<typename pointer_type>
  using PointerVector = std::vector<Pointer<pointer_type> >;

  template<typename key_type, typename pointer_type>
  using PointerMap = std::map<key_type, Pointer<pointer_type> >;
  
  template<typename pointer_type, typename... Args>
  static inline Pointer<pointer_type> makePointer(Args&&... args)
  {
    return std::move(std::make_shared<pointer_type>(std::forward<Args>(args)...));
  }

  template<typename pointer_type, typename deleter_type = std::default_delete<pointer_type> >
  using UniquePointer = std::unique_ptr<pointer_type, deleter_type>;

  template<typename pointer_type, typename... Args>
  static inline UniquePointer<pointer_type> makeUniquePointer(Args&&... args)
  {
    return std::move(std::unique_ptr<pointer_type>(new pointer_type(std::forward<Args>(args)...)));
  }

}; /* namespace Amplify */

#endif /* AMPUSH_TYPEDEFS_HPP */
