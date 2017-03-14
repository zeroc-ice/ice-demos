#include <EvictorBase.h>

using namespace std;

EvictorBase::EvictorBase(int size) : 
    _size(size)
{
    if (_size < 0)
    {
        _size = 1000;
    }
}

Ice::ObjectPtr
EvictorBase::locate(const Ice::Current& c, shared_ptr<void>& cookie)
{
    lock_guard<mutex> lk(_mutex);

    //
    // Check if we have a servant in the map already.
    //
    shared_ptr<EvictorEntry> entry;
    auto i = _map.find(c.id);
    if(i != _map.end())
    {
        //
        // Got an entry already, dequeue the entry from its current position.
        //
        entry = i->second;
        _queue.erase(entry->queuePos);
    }
    else
    {
        //
        // We do not have an entry. Ask the derived class to
        // instantiate a servant and add a new entry to the map.
        //
        entry = make_shared<EvictorEntry>();
        entry->servant = add(c, entry->userCookie); // Down-call
        if(!entry->servant)
        {
            return 0;
        }
        entry->useCount = 0;
        i = _map.insert(make_pair(c.id, entry)).first;
    }

    //
    // Increment the use count of the servant and enqueue
    // the entry at the front, so we get LRU order.
    //
    ++(entry->useCount);
    entry->queuePos = _queue.insert(_queue.begin(), i);

    cookie = entry;

    return entry->servant;
}

void
EvictorBase::finished(const Ice::Current&, const shared_ptr<Ice::Object>&, const shared_ptr<void>& cookie)
{
    lock_guard<mutex> lk(_mutex);

    auto entry = static_pointer_cast<EvictorEntry>(cookie);

    //
    // Decrement use count and check if there is something to evict.
    //
    --(entry->useCount);
    evictServants();
}

void
EvictorBase::deactivate(const string&)
{
    lock_guard<mutex> lk(_mutex);

    _size = 0;
    evictServants();
}

void
EvictorBase::evictServants()
{
    //
    // If the evictor queue has grown larger than the limit,
    // look at the excess elements to see whether any of them
    // can be evicted.
    //
    auto p = _queue.rbegin();
    auto excessEntries = static_cast<int>(_map.size() - _size);

    for(int i = 0; i < excessEntries; ++i)
    {
        auto mapPos = *p;
        if(mapPos->second->useCount == 0)
        {
            evict(mapPos->second->servant, mapPos->second->userCookie); // Down-call
            p = EvictorQueue::reverse_iterator(_queue.erase(mapPos->second->queuePos));
            _map.erase(mapPos);
        }
        else
        {
            ++p;
        }
    }
}
