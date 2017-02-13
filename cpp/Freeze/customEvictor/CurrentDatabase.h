// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CURRENT_DATABASE_H
#define CURRENT_DATABASE_H

#include <Ice/Ice.h>
#include <Database.h>
#include <list>

//
// Provides a Database object for each thread.
//
class CurrentDatabase
{
public:
    
    CurrentDatabase(const Ice::CommunicatorPtr&, const std::string&, const std::string&);
    ~CurrentDatabase();

    Database& get();
  
private:

    const Ice::CommunicatorPtr _communicator;
    const std::string _envName;
    const std::string _dbName;
    IceUtil::Mutex _dbListMutex;
    std::list<Database*> _dbList;
};

#endif
