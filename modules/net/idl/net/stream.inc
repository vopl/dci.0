
scope stream
{
    /////////////////////////////////////////////////////////
    iface Server
    {
        in bind:        (Address v) -> ();
        in listen:      ()          -> ();
        in accept:      ()          -> Channel;
        out accepted:   (Channel v) -> ();
        in close:       ()          -> ();
        out closed:     ();
    }

    /////////////////////////////////////////////////////////
    iface Client
    {
        in bind:        (Address v) -> ();
        in connect:     (Address v) -> Channel;
        in abort:       ()          -> ();
    }

    /////////////////////////////////////////////////////////
    iface Channel
    {
        in self:        ()          -> Address;
        in peer:        ()          -> Address;
        
        in read:        ()          -> Data;
        out readed:     (Data v)    -> ();
        
        in write:       (Data v)    -> ();
        
        in close:       ()          -> ();
        out closed:     ();
    }
}
