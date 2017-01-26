package Evictor;

public abstract class EvictorBase implements com.zeroc.Ice.ServantLocator
{
    public EvictorBase()
    {
        _size = 1000;
    }

    public EvictorBase(int size)
    {
        _size = size < 0 ? 1000 : size;
    }

    static public class AddResult
    {
        public AddResult()
        {
        }

        public AddResult(com.zeroc.Ice.Object r, java.lang.Object c)
        {
            returnValue = r;
            cookie = c;
        }

        public com.zeroc.Ice.Object returnValue;
        public java.lang.Object cookie;
    }

    public abstract AddResult add(com.zeroc.Ice.Current c);

    public abstract void evict(com.zeroc.Ice.Object servant, java.lang.Object cookie);

    @Override
    synchronized public final com.zeroc.Ice.ServantLocator.LocateResult locate(com.zeroc.Ice.Current c)
    {
        com.zeroc.Ice.ServantLocator.LocateResult r = new com.zeroc.Ice.ServantLocator.LocateResult();

        //
        // Check if we have a servant in the map already.
        //
        EvictorEntry entry = _map.get(c.id);
        if(entry != null)
        {
            //
            // Got an entry already, dequeue the entry from its current position.
            //
            entry.queuePos.remove();
        }
        else
        {
            //
            // We do not have entry. Ask the derived class to
            // instantiate a servant and add a new entry to the map.
            //
            entry = new EvictorEntry();
            AddResult ar = add(c); // Down-call
            if(ar.returnValue == null)
            {
                return r;
            }
            entry.servant = ar.returnValue;
            entry.userCookie = ar.cookie;
            entry.useCount = 0;
            _map.put(c.id, entry);
        }

        //
        // Increment the use count of the servant and enqueue
        // the entry at the front, so we get LRU order.
        //
        ++(entry.useCount);
        _queue.addFirst(c.id);
        entry.queuePos = _queue.iterator();
        entry.queuePos.next(); // Position the iterator on the element.

        r.cookie = entry;
        r.returnValue = entry.servant;

        return r;
    }

    @Override
    synchronized public final void finished(com.zeroc.Ice.Current c, com.zeroc.Ice.Object o, java.lang.Object cookie)
    {
        EvictorEntry entry = (EvictorEntry)cookie;

        //
        // Decrement use count and check if
        // there is something to evict.
        //
        --(entry.useCount);
        evictServants();
    }

    @Override
    synchronized public final void deactivate(String category)
    {
        _size = 0;
        evictServants();
    }

    private class EvictorEntry
    {
        com.zeroc.Ice.Object servant;
        java.lang.Object userCookie;
        java.util.Iterator<com.zeroc.Ice.Identity> queuePos;
        int useCount;
    }

    private void evictServants()
    {
        //
        // If the evictor queue has grown larger than the limit,
        // look at the excess elements to see whether any of them
        // can be evicted.
        //
        java.util.Iterator<com.zeroc.Ice.Identity> p = _queue.riterator();
        int excessEntries = _map.size() - _size;
        for(int i = 0; i < excessEntries; ++i)
        {
            com.zeroc.Ice.Identity id = p.next();
            EvictorEntry e = _map.get(id);
            if(e.useCount == 0)
            {
                evict(e.servant, e.userCookie); // Down-call
                e.queuePos.remove();
                _map.remove(id);
            }
        }
    }

    private java.util.Map<com.zeroc.Ice.Identity, EvictorEntry> _map = new java.util.HashMap<>();
    private Evictor.LinkedList<com.zeroc.Ice.Identity> _queue = new Evictor.LinkedList<>();
    private int _size;
}
