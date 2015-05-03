
scope datagram
{
    /////////////////////////////////////////////////////////
    iface Host
    {
        out Socket mkSocket();
    }

    /////////////////////////////////////////////////////////
    struct Datagram
    {
        Address peer;
        Data data;
    }

    /////////////////////////////////////////////////////////
    iface Socket
    {
        out void bind(Address v);
        
        out Data receiveData();
        out Datagram receive();
        in void received(Datagram v);
        
        out void send(Data d, Address a);
        
        out void close();
    }
}
