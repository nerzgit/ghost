//
//  delegate.hpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/09.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#ifndef delegate_hpp
#define delegate_hpp


#include <glm/glm.hpp>
#include <string>

struct AABB;
struct OBB;
struct RayOBB;

class Delegator {
    
public:
    Delegator(){};
    virtual ~Delegator(){};
    
    virtual void        operator()(                   ) = 0;
    virtual int         operator()( int         value ) = 0;
    virtual float       operator()( float       value ) = 0;
    virtual glm::vec2   operator()( glm::vec2   value ) = 0;
    virtual glm::vec3   operator()( glm::vec3   value ) = 0;
    virtual glm::mat4   operator()( glm::mat4   value ) = 0;
    virtual std::string operator()( std::string value ) = 0;
    virtual AABB*       operator()( AABB*       value ) = 0;
    virtual OBB*        operator()( OBB*        value ) = 0;
    virtual RayOBB*     operator()( RayOBB*     value ) = 0;
    
};


template < class T >
class Delegate : public Delegator {
    
public:
    Delegate(){};
    virtual ~Delegate(){};
    
    
    
    // void
    virtual void operator()() {
        (fobject->*fvoid)();
    }
    
    typedef void (T::*FVoid)();
    
    void set( T* obj, FVoid func ) {
        fobject = obj;
        fvoid = func;
    }
    
    static Delegator* createDelegator( T* obj, void (T::*func)() ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
    // int
    virtual int operator()( int value ) {
        return (fobject->*fint)( value );
    }
    
    typedef int (T::*FInt)( int );
    
    void set( T* obj, FInt func ) {
        fobject = obj;
        fint = func;
    }
    
    static Delegator* createDelegator( T* obj, int (T::*func)( int ) ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
    // float
    virtual float operator()( float value ) {
        return (fobject->*ffloat)( value );
    }
    
    typedef float (T::*FFloat)( float );
    
    void set( T* obj, FFloat func ) {
        fobject = obj;
        ffloat = func;
    }
    
    static Delegator* createDelegator( T* obj, float (T::*func)( float ) ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
    // glm::vec2
    virtual glm::vec2 operator()( glm::vec2 value ) {
        return (fobject->*fvec2)( value );
    }
    
    typedef glm::vec2 (T::*FVec2)( glm::vec2 );
    
    void set( T* obj, FVec2 func ) {
        fobject = obj;
        fvec2 = func;
    }
    
    static Delegator* createDelegator( T* obj, glm::vec2 (T::*func)( glm::vec2 ) ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
    // glm::vec3
    virtual glm::vec3 operator()( glm::vec3 value ) {
        return (fobject->*fvec3)( value );
    }
    
    typedef glm::vec3 (T::*FVec3)( glm::vec3 );
    
    void set( T* obj, FVec3 func ) {
        fobject = obj;
        fvec3 = func;
    }
    
    static Delegator* createDelegator( T* obj, glm::vec3 (T::*func)( glm::vec3 ) ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
    // glm::mat4
    virtual glm::mat4 operator()( glm::mat4 value ) {
        return (fobject->*fmat4)( value );
    }
    
    typedef glm::mat4 (T::*FMat4)( glm::mat4 );
    
    void set( T* obj, FMat4 func ) {
        fobject = obj;
        fmat4 = func;
    }
    
    static Delegator* createDelegator( T* obj, glm::mat4 (T::*func)( glm::mat4 ) ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
    // std::string
    virtual std::string operator()( std::string value ) {
        return (fobject->*fstring)( value );
    }
    
    typedef std::string (T::*FString)( std::string );
    
    void set( T* obj, FString func ) {
        fobject = obj;
        fstring = func;
    }
    
    static Delegator* createDelegator( T* obj, std::string (T::*func)( std::string ) ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
    // AABB
    virtual AABB* operator()( AABB* value ) {
        return (fobject->*faabb)( value );
    }
    
    typedef AABB* (T::*FAABB)( AABB* );
    
    void set( T* obj, FAABB func ) {
        fobject = obj;
        faabb = func;
    }
    
    static Delegator* createDelegator( T* obj, AABB* (T::*func)( AABB* ) ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
    // OBB
    virtual OBB* operator()( OBB* value ) {
        return (fobject->*fobb)( value );
    }
    
    typedef OBB* (T::*FOBB)( OBB* );
    
    void set( T* obj, FOBB func ) {
        fobject = obj;
        fobb = func;
    }
    
    static Delegator* createDelegator( T* obj, OBB* (T::*func)( OBB* ) ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
    // RayOBB
    virtual RayOBB* operator()( RayOBB* value ) {
        return (fobject->*frayobb)( value );
    }
    
    typedef RayOBB* (T::*FRayOBB)( RayOBB* );
    
    void set( T* obj, FRayOBB func ) {
        fobject = obj;
        frayobb = func;
    }
    
    static Delegator* createDelegator( T* obj, RayOBB* (T::*func)( RayOBB* ) ) {
        Delegate* dg = new Delegate;
        dg->set( obj, func );
        return dg;
    }
    
    
    
protected:
    T*       fobject;         // Object
    FVoid    fvoid;           // Method pointer
    FInt     fint;            // Method pointer
    FFloat   ffloat;          // Method pointer
    FVec2    fvec2;           // Method pointer
    FVec3    fvec3;           // Method pointer
    FMat4    fmat4;           // Method pointer
    FString  fstring;         // Method pointer
    FAABB    faabb;           // Method pointer
    FOBB     fobb;            // Method pointer
    FRayOBB  frayobb;         // Method pointer
    
};


#endif /* delegate_hpp */
