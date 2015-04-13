#pragma once
#include <dci/couple/runtime/idl.hpp>

namespace dci { namespace couple { namespace runtime { namespace generated
{

    template <int i> struct wire;
    template <int i> struct body;

    template <int i>
    struct wire
    {
        struct talk
        {
            struct Peer
                : public IfaceWire
            {
                idl::Wire< idl::void_(typename body<i>::talk::Peer::Id)> setId;
                idl::Wire< idl::void_(typename body<i>::talk::Peer::Status)> setStatus;
                idl::Wire< idl::bool_(typename body<i>::talk::Chat)> joinChat;

                Peer();
            };

            struct Chat
                : public IfaceWire
            {
                idl::Wire< idl::void_ (idl::list<typename body<i>::talk::Peer::Id>)> setMembers;

                idl::Wire< idl::void_ (typename body<i>::talk::Peer::Id)> addMember;
                idl::Wire< idl::void_ (typename body<i>::talk::Peer::Id)> delMember;

                idl::Wire< idl::void_ (typename body<i>::talk::VoiceTrain)> exhibitVoice;
                idl::Wire< idl::void_ (typename body<i>::talk::VideoTrain)> exhibitVideo;
                idl::Wire< typename body<i>::talk::FilesReceiver (typename body<i>::talk::FilesInfo::Id)> getFiles;

                idl::Wire< idl::void_ (bool)> typing;
                idl::Wire< idl::void_ (typename body<i>::talk::Message msg)> message;
                idl::Wire< idl::void_ (typename body<i>::talk::Message::Id, typename body<i>::talk::Message::Id)> wantMessageReply;

                Chat();
            };

            struct FilesReceiver
                : public IfaceWire
            {
                idl::Wire< idl::void_ ()> abort;
                idl::Wire< idl::void_ ()> abort2;
                idl::Wire< idl::void_ (idl::list<typename body<i>::talk::FilesReceiver::Entry>)> list;
                idl::Wire< idl::void_ (idl::list<idl::int8>)> content;

                idl::Wire< idl::void_ ()> finish;

                FilesReceiver();
            };

            struct VoiceTrain
                : public IfaceWire
            {
                VoiceTrain();
            };

            struct VideoTrain
                : public IfaceWire
            {
                VideoTrain();
            };
        };

    };




    template <int i>
    struct body
    {
        struct talk
        {
            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct Peer
                : private Iface
            {
                using Id = idl::array< idl::int8, 32>;

                enum class Status
                {
                    available=0,
                    busy=1,
                };



                Peer();
                Peer(Peer &&from);
                Peer(Iface &&from);
                Peer(typename body<i>::talk::PeerOpposite &from);
                ~Peer();

                Peer &operator=(Peer &&from);
                Peer &operator=(Iface &&from);

                typename wire<i>::talk::Peer *wire();




                idl::Future<> setId(Id &&id);
                idl::Future<> setId(const Id &id);
                idl::Future<> setId(idl::ValuePorter<Id> &&id);

                idl::Future<> setStatus(Status &&status);
                idl::Future<> setStatus(const Status &status);
                idl::Future<> setStatus(idl::ValuePorter<Status> &&status);

                idl::Future<bool> joinChat(typename body<i>::talk::Chat &&chat);
                idl::Future<bool> joinChat(idl::ValuePorter<typename body<i>::talk::Chat> &&chat);
            };


            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct PeerOpposite
                : private Iface
            {
                using Id = typename body<i>::talk::Peer::Id;
                using Status = typename body<i>::talk::Peer::Status;

                PeerOpposite();
                PeerOpposite(PeerOpposite &&from);
                PeerOpposite(Iface &&from);
                PeerOpposite(typename body<i>::talk::Peer &from);
                ~PeerOpposite();

                PeerOpposite &operator=(PeerOpposite &&from);
                PeerOpposite &operator=(Iface &&from);

                typename wire<i>::talk::Peer *wire();




                idl::Signal<void(Id)> &setId();
                idl::Signal<void(Status)> &setStatus();

                idl::Signal<bool(typename body<i>::talk::Chat)> &joinChat();

            };


            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct Chat
                : private Iface
            {
                //incomplete
            };

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct ChatOpposite
                : private Iface
            {
                //incomplete
            };


            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct FilesInfo
            {
                using Id = idl::int64;
                Id id;

                idl::list< idl::string> topNamesPreview;
                idl::uint64 filesAmount;
                idl::uint64 directoriesAmount;
                idl::uint64 totalFilesVolume;
            };

            struct MediaInfo
                : FilesInfo
            {
                idl::string mimeType;
                idl::list<idl::int8> preview;
            };


            struct Message
            {
                using Id=idl::int64;

                //variant Data
                struct Data
                    : idl::variant<idl::string, FilesInfo, MediaInfo>
                {
                };


                Id id;
                idl::list<Data> data;
            };

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct FilesReceiver
                : private Iface
            {
                struct Entry
                {
                    struct SizeOrVoid
                        : idl::variant<idl::uint64, void>
                    {
                    };

                    idl::string name;
                    SizeOrVoid sizeOrVoid;
                };

                //incomplete
            };

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct FilesReceiverOpposite
                : private Iface
            {
                using Entry = typename body<i>::talk::FilesReceiver::Entry;

                //incomplete
            };


            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct VoiceTrain
                : private Iface
            {

            };

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct VoiceTrainOpposite
                : private Iface
            {

            };

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct VideoTrain
                : private Iface
            {

            };

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            struct VideoTrainOpposite
                : private Iface
            {

            };


        };




    };

}}}}

namespace talk
{
    using Peer = typename ::dci::couple::runtime::generated::body<0>::talk::Peer;
    using PeerOpposite = typename ::dci::couple::runtime::generated::body<0>::talk::PeerOpposite;

    using Chat = typename ::dci::couple::runtime::generated::body<0>::talk::Chat;
    using ChatOpposite = typename ::dci::couple::runtime::generated::body<0>::talk::ChatOpposite;

    using FilesInfo = typename ::dci::couple::runtime::generated::body<0>::talk::FilesInfo;

    using MediaInfo = typename ::dci::couple::runtime::generated::body<0>::talk::MediaInfo;

    using Message = typename ::dci::couple::runtime::generated::body<0>::talk::Message;

    using FilesReceiver = typename ::dci::couple::runtime::generated::body<0>::talk::FilesReceiver;
    using FilesReceiverOpposite = typename ::dci::couple::runtime::generated::body<0>::talk::FilesReceiverOpposite;

    using VoiceTrain = typename ::dci::couple::runtime::generated::body<0>::talk::VoiceTrain;
    using VoiceTrainOpposite = typename ::dci::couple::runtime::generated::body<0>::talk::VoiceTrainOpposite;

    using VideoTrain = typename ::dci::couple::runtime::generated::body<0>::talk::VideoTrain;
    using VideoTrainOpposite = typename ::dci::couple::runtime::generated::body<0>::talk::VideoTrainOpposite;
}
