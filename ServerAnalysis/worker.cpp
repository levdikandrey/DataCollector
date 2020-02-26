#include "worker.h"
#include "aclient.h"

void Worker::addClient(qintptr socketDescriptor)
{
    new AClient(socketDescriptor, this);
}
