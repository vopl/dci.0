
scope datagram
{
    /////////////////////////////////////////////////////////
    iface Channel
    {
        in bind:            (Address v)         -> ();
        
        in receiveData:     ()                  -> Bytes;
        in receive:         ()                  -> (Bytes, Address);
        out received:       (Bytes, Address)    -> ();
        
        in send:            (Bytes, Address)    -> ();
        
        in close:           ()                  -> ();
        out closed:         ();
    }
}
