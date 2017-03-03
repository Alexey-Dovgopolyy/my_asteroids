#include "command.h"
#include <QDebug>

Command::Command()
    : action()
    , category(Category::None)
{
    //qDebug() << "yo";
}

