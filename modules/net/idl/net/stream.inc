
scope stream
{
    /////////////////////////////////////////////////////////
    iface Host
    {
        in mkServer:    () -> Server;
        in mkClient:    () -> Client;
    }

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
        in ownAddress:  ()          -> Address;
        in peerAddress: ()          -> Address;
        
        in read:        ()          -> Data;
        out readed:     (Data v)    -> ();
        
        in write:       (Data v)    -> ();
        
        in close:       ()          -> ();
        out closed:     ();
    }
}
