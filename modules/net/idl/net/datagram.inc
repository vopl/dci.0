
scope datagram
{
    /////////////////////////////////////////////////////////
    interface Channel
    {
        in bind:            (Address v)         -> ();
        
        in receiveData:     ()                  -> (bytes);
        in receive:         ()                  -> (bytes, Address);
        out received:       (bytes, Address)    -> ();
        
        in send:            (bytes, Address)    -> ();
        
        in close:           ()                  -> ();
        out closed:         ();
    }
}
