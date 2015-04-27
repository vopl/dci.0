
scope datagram
{
    /////////////////////////////////////////////////////////
    iface Host
    {
        in Socket mkSocket();
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
        in void bind(Address v);
        
        in Data receiveData();
        in Datagram receive();
        out void received(Datagram v);
        
        in void send(Data d, Address a);
        
        in void close();
    }
}
