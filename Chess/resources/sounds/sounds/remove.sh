# Deletes all unneeded audio files
find . ! -name Capture.mp3 ! -name Check.mp3 ! -name Move.mp3 ! -name Castle.mp3 ! -name Defeat.mp3 ! -name Draw.mp3 ! -name Error.mp3 ! -name GenericNotify.mp3 ! -name Victory.mp3 ! -name LowTime.mp3 ! -name NewChatMessage.mp3 -type f -delete