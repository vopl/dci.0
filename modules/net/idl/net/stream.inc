
scope stream
{
    /////////////////////////////////////////////////////////
    iface Host
    {
        in Server mkServer();
        in Client mkClient();
    }

    /////////////////////////////////////////////////////////
    iface Server
    {
        in void bind(Address v);
        in void listen();
        in Stream accept();
        out void accepted(Stream v);
        in void close();
    }

    /////////////////////////////////////////////////////////
    iface Client
    {
        in void bind(Address v);
        in Stream connect(Address v);
        in void abort();
    }

    /////////////////////////////////////////////////////////
    iface Stream
    {
        in Address ownAddress();
        in Address peerAddress();
        
        in Data read();
        out void readed(Data v);
        
        in void write(Data v);
        
        in void close();
    }
}