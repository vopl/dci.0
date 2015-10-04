
scope stream
{
    /////////////////////////////////////////////////////////
    interface Server
    {
        in bind:        (Address v) -> ();
        in listen:      ()          -> ();
        in accept:      ()          -> Channel;
        in close:       ()          -> ();
        out closed:     ();
    }

    /////////////////////////////////////////////////////////
    interface Client
    {
        in bind:        (Address v) -> ();
        in connect:     (Address v) -> Channel;
        in abort:       ()          -> ();
    }

    /////////////////////////////////////////////////////////
    interface Channel
    {
        in self:        ()          -> Address;
        in peer:        ()          -> Address;
        
        in read:        ()          -> bytes;
        
        in write:       (bytes v)   -> ();
        
        in close:       ()          -> ();
        out closed:     ();
    }
}
