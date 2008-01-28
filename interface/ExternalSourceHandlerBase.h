#ifndef CondCore_PopCon_ExternalSourceHandlerBase_h
#define CondCore_PopCon_ExternalSourceHandlerBase_h
namespace cond{
  /** 
      Abstract base functor
  */
  template <typename T>
    class ExternalSourceHandlerBase{
  public:
    /// returns a newed pointer
    virtual T* operator()()=0;
    virtual ~ExternalSourceHandlerBase(){}
  };
}//ns cond
#endif
