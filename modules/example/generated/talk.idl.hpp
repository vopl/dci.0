
#include <memory>

//предварительные определения

namespace talk
{
    struct Peer;
    struct Chat;

    struct FilesInfo;
    using FilesInfoPtr = std::shared_ptr<FilesInfo>;

    struct MediaInfo;
    using MediaInfoPtr = std::shared_ptr<MediaInfo>;

    struct Message;
    using MessagePtr = std::shared_ptr<Message>;

    struct FilesReceiver;
    struct VoiceTrain;
    struct VideoTrain;
}
