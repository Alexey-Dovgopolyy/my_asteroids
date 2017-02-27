#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "command.h"

#include <queue>

class CommandQueue
{
public:
    CommandQueue();
    
    void        push        (const Command& command);
    Command     pop         ();
    bool        isEmpty     ();
    
private:
    std::queue<Command> mQueue;
};

#endif // COMMANDQUEUE_H
