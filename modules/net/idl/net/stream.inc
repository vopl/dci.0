
scope stream
{
    /////////////////////////////////////////////////////////
    interface Server
    {
        in bind:        (Address v) -> ();
        in listen:      ()          -> ();
        in accept:      ()          -> Channel;
        out accepted:   (Channel v) -> ();
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
        out readed:     (bytes v)   -> ();
        
        in write:       (bytes v)   -> ();
        
        in close:       ()          -> ();
        out closed:     ();
    }
}
