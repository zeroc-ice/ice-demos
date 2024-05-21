#ifndef EVICTORBASE_H
#define EVICTORBASE_H

#include <Ice/Ice.h>
#include <list>
#include <map>
#include <mutex>

class EvictorBase : public Ice::ServantLocator
{
public:
    EvictorBase(int size = 1000);

    virtual std::shared_ptr<Ice::Object> locate(const Ice::Current&, std::shared_ptr<void>&) override;
    virtual void
    finished(const Ice::Current&, const std::shared_ptr<Ice::Object>&, const std::shared_ptr<void>&) override;
    virtual void deactivate(const std::string&) override;

protected:
    virtual std::shared_ptr<Ice::Object> add(const Ice::Current&, std::shared_ptr<void>&) = 0;
    virtual void evict(const std::shared_ptr<Ice::Object>&, const std::shared_ptr<void>&) = 0;

private:
    struct EvictorEntry;

    using EvictorMap = std::map<Ice::Identity, std::shared_ptr<EvictorEntry>>;
    using EvictorQueue = std::list<EvictorMap::iterator>;

    struct EvictorEntry
    {
        std::shared_ptr<Ice::Object> servant;
        std::shared_ptr<void> userCookie;
        EvictorQueue::iterator queuePos;
        int useCount;
    };

    EvictorMap _map;
    EvictorQueue _queue;
    int _size;

    std::mutex _mutex;

    void evictServants();
};
#endif
