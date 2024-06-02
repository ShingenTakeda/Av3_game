#include <Base/Game.hpp>
#include <MessageBus/MessageBus.h>

struct MSG_TYPE_1
{
  int i;
};

void RegularFunctionSubscriber( MSG_TYPE_1 msg )
{
  std::cout<< "RegularFunctionSubscriber " << msg.i << std::endl;
}

class FunctorSubscriber
{
public:
  void operator()( MSG_TYPE_1 msg ) { std::cout<< "FunctorSubscriber " << msg.i << std::endl; }
};

int main()
{
  MSG_TYPE_1 msg1 = { 10 };

  FunctorSubscriber functorSubscriber;

  // Regular Function Subscriber
  SubscriberHandle handle1 = MsgBus<>::Subscribe< MSG_TYPE_1 >(RegularFunctionSubscriber);

  //Functor Subscriber
  SubscriberHandle handle2 = MsgBus<>::Subscribe< MSG_TYPE_1 >( functorSubscriber );

  // Lambda Function Subscriber
  SubscriberHandle handle3 = MsgBus<>::Subscribe< MSG_TYPE_1 >( [](MSG_TYPE_1 msg) 
                                                               { std::cout<< "Lambda Subscriber " << msg.i << std::endl; } );

  MsgBus<>::PublishBlocking( msg1 );
  std::cout<< std::endl;

  MsgBus<>::PublishAsync( msg1 );
  std::cout<< std::endl;

  MsgBus<>::UnSubscribe( handle2 );
  std::cout<< std::endl;

  MsgBus<>::PublishBlocking( msg1 );
  std::cout<< std::endl;

  MsgBus<>::PublishAsync( msg1 );
  std::cout<< std::endl;

  Game game;
  game.init();
  game.run();
  return 0;
}
