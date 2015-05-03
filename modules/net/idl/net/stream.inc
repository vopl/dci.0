
scope stream
{
    /////////////////////////////////////////////////////////
    iface Host
    {
        out Server mkServer();
        out Client mkClient();
    }

    /////////////////////////////////////////////////////////
    iface Server
    {
        out void bind(Address v);
        out void listen();
        out Stream accept();
        in void accepted(Stream v);
        out void close();
    }

    /////////////////////////////////////////////////////////
    iface Client
    {
        out void bind(Address v);
        out Stream connect(Address v);
        out void abort();
    }

    /////////////////////////////////////////////////////////
    iface Stream
    {
        out Address ownAddress();
        out Address peerAddress();
        
        out Data read();
        in void readed(Data v);
        
        out void write(Data v);
        
        out void close();
    }
}
