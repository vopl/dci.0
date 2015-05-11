
scope datagram
{
    /////////////////////////////////////////////////////////
    iface Host
    {
        in mkChannel:   () -> Channel;
    }

    /////////////////////////////////////////////////////////
    iface Channel
    {
        in bind:            (Address v)         -> ();
        
        in receiveData:     ()                  -> Data;
        in receive:         ()                  -> (Data, Address);
        out received:       (Data, Address)     -> ();
        
        in send:            (Data, Address)     -> ();
        
        in close:           ()                  -> ();
    }
}
