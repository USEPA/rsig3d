#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

/******************************************************************************
PURPOSE: ResourceHandler.h - Calls a handler on a resource in destructor.
HISTORY: 2013-10-17 plessel.todd@epa.gov
STATUS:  unreviewed tested
NOTES:   RAII Idiom.
         http://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization
         Example usage:
         {
           const char* name = "output.bin";
           emvl::ResourceHandler<FILE*> closer( fopen( name, "wb" ), fclose );
           FILE* const file = closer.get();

           if ( file ) {
             closer.handler2( name, unlink ); // File is removed upon failure.
             // ... write to file ...
             call_function_that_might_throw_exception(); // Close/remove file.
             closer.handler2( 0, 0 ); // Successful so don't remove.
           }
         } // File is closed.

******************************************************************************/

//================================== TYPES ====================================

namespace emvl {

template <typename T> class ResourceHandler {
public:
  typedef int   (*IntHandler)( T ); // Type of handler, e.g., fclose().
  typedef void (*VoidHandler)( T ); // Type of handler, e.g., free().
  typedef int   (*Handler2)( const char* ); // Type of 2nd handler. unlink().
private:
  T resource;               // Resource to be handled in dtor. E.g., FILE*.
  IntHandler  intHandler;   // Primary handler called on resource. E.g., fclose
  VoidHandler voidHandler;  // Alternative primary handler called on resource.
  const char* resource2;    // Optional 2nd resource for handler2.
  Handler2    handler2_;    // Optional secondary handler, e.g., unlink().

  ResourceHandler(); // Not allowed.
  ResourceHandler( const ResourceHandler& other ); // Not allowed.
  ResourceHandler& operator=( const ResourceHandler& other ); // Not allowed.

public:

  /*
   * Constructors take a resource and a handler (returning either int or void)
   * to be called on resource when this object is destructed.
   */

  ResourceHandler( T resource, IntHandler handler )
    : resource( resource ), intHandler( handler ), voidHandler( 0 ),
    resource2( 0 ), handler2_( 0 ) {}
 
  ResourceHandler( T resource, VoidHandler handler )
    : resource( resource ), intHandler( 0 ), voidHandler( handler ),
    resource2( 0 ), handler2_( 0 ) {}

  // Destructor calls primary and optionally secondary handlers on resource:

  ~ResourceHandler() {
            
    if ( resource ) { // Doesn't call if resource is 0 (null).

      if ( intHandler ) {
        intHandler( resource );
      } else if ( voidHandler ) {
        voidHandler( resource );        
      }

      resource = 0;
      intHandler = 0;
      voidHandler = 0;

      if ( resource2 && handler2_ ) {
        handler2_( resource2 );
      }

      resource2 = 0;
      handler2_ = 0;
    }
  }

  // Set optional secondary handler:

  void handler2( const char* resource, Handler2 handler ) {
    resource2 = resource;
    handler2_ = handler;
  }

  T get() const { return resource; }
};

} // Namespace emvl.

#endif /* RESOURCEHANDLER_H */


