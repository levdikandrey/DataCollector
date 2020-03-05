#include "worker.h"
#include "aclient.h"

void Worker::addClient(qintptr socketDescriptor,ThreadReadQueue* threadReadQueue)
{
    new AClient(socketDescriptor, threadReadQueue, this);
}
