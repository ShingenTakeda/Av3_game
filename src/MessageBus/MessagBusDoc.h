/*!*****************************************************************************
* @mainpage Message Bus Documentation
*
* @section intro_sec Introduction
*
* SW Message Bus represents message flow abstraction mechanism in a complex,
* multithreaded SW system.
*
* @subsection API
*
* For API please refer to
* MessageBus.h
* MessageBusDefs.h
*
* @subsection Example
*
* using namespace std;
*
* struct MSG_TYPE_1
* {
*     int i;
* };
*
* void RegularFuncionSubscriber( MSG_TYPE_1 msg )
* {
*    cout<< "FuncionSubscriber " << msg.i << endl;
* }
*
* class FunctorSubscriber
* {
* public:
*     void operator()( MSG_TYPE_1 msg ) { cout<< "FunctorSubscriber "
*                                                            << msg.i << endl; }
* };
*
* MSG_TYPE_1 msg1 = { 10 };
*
* FunctorSubscriber functorSubscriber;
*
* // Regular Function Subscriber
*
* SubscriberHandle handle1 = MsgBus<>::Subscribe< MSG_TYPE_1 >(
*                                                    RegularFuncionSubscriber );
*
* // Functor Subscriber
*
* SubscriberHandle handle2 = MsgBus<>::Subscribe< MSG_TYPE_1 >(
*                                                           functorSubscriber );
*
* // Lambda Function Subscriber
*
* SubscriberHandle handle3 = MsgBus<>::Subscribe< MSG_TYPE_1 >(
*        [](MSG_TYPE_1 msg) { cout<< "Lambda Subscriber " << msg.i << endl; } );
*
* MsgBus<>::PublishBlocking( msg1 );
*
* MsgBus<>::PublishAsync( msg1 );
*
* MsgBus<>::UnSubscribe( handle1 );
*
* MsgBus<>::UnSubscribe( handle2 );
*
* MsgBus<>::UnSubscribe( handle3 );
*******************************************************************************/
