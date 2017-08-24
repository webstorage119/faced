#include "logger.h"

Logger* Logger::instance = NULL;

QString Logger::out = CONSOLE;
QFile* Logger::file = NULL;

Logger::Logger(QObject *parent) :
    QObject(parent)
{
}

Logger *Logger::getInstance()
{
    if(!instance)
    {
        instance = new Logger();
        LOG_INF( "Logger: successfully initialized" );
    }
    return instance;
}

QString Logger::getOut() const
{
    return out;
}

void Logger::setOut( const QString &value )
{
    out = value;
    LOG_INF( QString( "Logger: logging to " + this->getOut() ) );
}

QFile *Logger::getFile() const
{
    return file;
}

void Logger::setFile(QFile *value)
{
    file = value;
}

void Logger::log( QString text )
{
    if( getOut() == CONSOLE )
    {
        qDebug() << text;
        return;
    }

    if( getFile() == NULL )
    {
        QString currentPath = QDir::currentPath();
        QString outFilePath = currentPath + LOG_DIR;
        file = new QFile(
                    outFilePath + QDateTime::currentDateTime().toString() + LOG_FILE_EXT );
    }

    if(!getFile()->isOpen())
    {
        if( getFile()->open(QIODevice::ReadWrite) == false )
        {
            setOut(CONSOLE);
            log("Error open log file");
        }
    }

    if(getFile()->isOpen())
    {
        QTextStream stream(getFile());
        stream << text << endl;
    }
    else
    {
        setOut( CONSOLE );
        log(text);
    }
}

void Logger::inf( QString text )
{
    log( QString( PREF_INF + text ) );
}

void Logger::war( QString text )
{
    log( QString( PREF_WAR + text ) );
}

void Logger::err( QString text )
{
    log( QString( PREF_ERR + text ) );
    throw new runtime_error(text.toStdString());
}

Logger::~Logger()
{
    if(getFile()->isOpen())
    {
        getFile()->close();
    }
    delete file;
    delete instance;
}
