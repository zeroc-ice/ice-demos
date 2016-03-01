// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CASINO_BET_I_H
#define CASINO_BET_I_H

#include <Freeze/Freeze.h>
#include <Casino.h>
#include <CasinoStore.h>

class BetI : public CasinoStore::PersistentBet
{
public:

    virtual int getAmount(const Ice::Current&) const;

    virtual void accept(const Casino::PlayerPrx&, const Ice::Current&);

    virtual int getChipsInPlay(const Ice::Current&) const;

    virtual Ice::Long getCloseTime(const Ice::Current&) const;

    virtual void complete(int, const Ice::Current&);

    BetI();

    BetI(int, Ice::Long, const CasinoStore::PersistentBankPrx&,
         const Freeze::TransactionalEvictorPtr& evictor, int);

    void init(const Freeze::TransactionalEvictorPtr&, int);

private:

    Freeze::TransactionalEvictorPtr _evictor;
    int _bankEdge;
};

#endif
