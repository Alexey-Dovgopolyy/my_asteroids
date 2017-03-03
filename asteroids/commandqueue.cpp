#include "commandqueue.h"
#include <QDebug>

CommandQueue::CommandQueue()
{

}

void CommandQueue::push(const Command& command)
{
    mQueue.push(command);
    //qDebug() << mQueue.size();
}

Command CommandQueue::pop()
{
    Command command = mQueue.front();
    mQueue.pop();
    return command;
}

bool CommandQueue::isEmpty()
{
    return mQueue.empty();
}
