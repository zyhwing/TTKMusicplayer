#include "musicsongchecktoolscore.h"
#include "musicsongtag.h"

MusicSongCheckToolsRenameCore::MusicSongCheckToolsRenameCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsRenameCore::~MusicSongCheckToolsRenameCore()
{
    stopAndQuitThread();
}

QString MusicSongCheckToolsRenameCore::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsRenameCore::setRenameSongs(MusicSongItems *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsRenameCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsRenameCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsRenameCore::run()
{
    if(m_songItems && m_songItems->count() >= 4)
    {
        MusicSongs *musicSongs = &m_songItems->first().m_songs;
        SongCheckToolsRenames items;
        MusicSongTag tag;
        foreach(const MusicSong &song, *musicSongs)
        {
            if(!m_run)
            {
                emit finished(SongCheckToolsRenames());
                return;
            }

            if(!tag.readFile(song.getMusicPath()))
            {
                continue;
            }

            if(tag.getArtist() != song.getMusicArtistFront() ||
               tag.getTitle() != song.getMusicArtistBack())
            {
                items << SongCheckToolsRename(song.getMusicName(), tag.getArtist() + " - " + tag.getTitle());
            }
        }
        emit finished(items);
    }
}



MusicSongCheckToolsDuplicateCore::MusicSongCheckToolsDuplicateCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsDuplicateCore::~MusicSongCheckToolsDuplicateCore()
{
    stopAndQuitThread();
}

QString MusicSongCheckToolsDuplicateCore::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsDuplicateCore::setDuplicateSongs(MusicSongItems *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsDuplicateCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsDuplicateCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsDuplicateCore::run()
{
    if(m_songItems && m_songItems->count() >= 4)
    {
        MusicSongs *musicSongs = &m_songItems->first().m_songs;
        SongCheckToolsDuplicates items;
        MusicSongTag tag;
        foreach(const MusicSong &song, *musicSongs)
        {
            if(!m_run)
            {
                emit finished(SongCheckToolsDuplicates());
                return;
            }

            if(!tag.readFile(song.getMusicPath()))
            {
                continue;
            }

            items << SongCheckToolsDuplicate(song, tag.getBitrate());
        }
        emit finished(items);
    }
}



MusicSongCheckToolsQualityCore::MusicSongCheckToolsQualityCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsQualityCore::~MusicSongCheckToolsQualityCore()
{
    stopAndQuitThread();
}

QString MusicSongCheckToolsQualityCore::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsQualityCore::setQualitySongs(MusicSongItems *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsQualityCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsQualityCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsQualityCore::run()
{
    if(m_songItems && m_songItems->count() >= 4)
    {
        MusicSongs *musicSongs = &m_songItems->first().m_songs;
        SongCheckToolsQualitys items;
        MusicSongTag tag;
        foreach(const MusicSong &song, *musicSongs)
        {
            if(!m_run)
            {
                emit finished(SongCheckToolsQualitys());
                return;
            }

            if(!tag.readFile(song.getMusicPath()))
            {
                continue;
            }

            items << SongCheckToolsQuality(song, tag.getBitrate(), transfromBitrateToQuality(tag.getBitrate()));
        }
        emit finished(items);
    }
}

int MusicSongCheckToolsQualityCore::transfromBitrateToQuality(const QString &bitrate) const
{
    if(bitrate.isEmpty())
    {
        return -1;
    }

    QStringList data(bitrate.split(" "));
    if(data.count() >= 2)
    {
        int bit = data.front().trimmed().toInt();
        if(bit <= 0)
        {
            return -1;
        }
        else if(bit > 0 && bit <= 96)
        {
            return 0;
        }
        else if(bit > 96 && bit <= 196)
        {
            return 1;
        }
        else if(bit > 196 && bit <= 320)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }
    return -1;
}