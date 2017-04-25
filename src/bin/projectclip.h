/*
Copyright (C) 2012  Till Theato <root@ttill.de>
Copyright (C) 2014  Jean-Baptiste Mardelle <jb@kdenlive.org>
This file is part of Kdenlive. See www.kdenlive.org.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROJECTCLIP_H
#define PROJECTCLIP_H

#include "abstractprojectitem.h"
#include "definitions.h"
#include "mltcontroller/clipcontroller.h"
#include "timeline2/model/timelinemodel.hpp"

#include <QFuture>
#include <QMutex>
#include <QUrl>
#include <memory>

class AudioStreamInfo;
class ClipPropertiesController;
class MarkerListModel;
class ProjectFolder;
class ProjectSubClip;
class QDomElement;
class QUndoCommand;

namespace Mlt {
class Producer;
class Properties;
}

/**
 * @class ProjectClip
 * @brief Represents a clip in the project (not timeline).
 *
 */

class ProjectClip : public AbstractProjectItem, public ClipController
{
    Q_OBJECT

public:
    /**
     * @brief Constructor; used when loading a project and the producer is already available.
     */
    ProjectClip(const QString &id, const QIcon &thumb, ProjectItemModel *model, std::shared_ptr<Mlt::Producer> producer, ProjectFolder *parent);
    /**
     * @brief Constructor.
     * @param description element describing the clip; the "id" attribute and "resource" property are used
     */
    ProjectClip(const QDomElement &description, const QIcon &thumb, ProjectItemModel *model, ProjectFolder *parent);
    virtual ~ProjectClip();

    void reloadProducer(bool refreshOnly = false);

    /** @brief Returns a unique hash identifier used to store clip thumbnails. */
    // virtual void hash() = 0;

    /** @brief Returns this if @param id matches the clip's id or nullptr otherwise. */
    ProjectClip *clip(const QString &id) override;

    ProjectFolder *folder(const QString &id) override;

    ProjectSubClip *getSubClip(int in, int out);

    /** @brief Returns this if @param ix matches the clip's index or nullptr otherwise. */
    ProjectClip *clipAt(int ix) override;

    /** @brief Returns the clip type as defined in definitions.h */
    ClipType clipType() const;

    /** @brief Check if clip has a parent folder with id id */
    bool hasParent(const QString &id) const;
    ClipPropertiesController *buildProperties(QWidget *parent);
    QPoint zone() const override;

    /** @brief Returns true if we want to add an affine transition in timeline when dropping this clip. */
    bool isTransparent() const;

    /** @brief Returns whether this clip has a url (=describes a file) or not. */
    bool hasUrl() const;

    /** @brief Returns the clip's url. */
    const QString url() const;

    /** @brief Returns the clip's duration. */
    GenTime duration() const;
    int frameDuration() const;

    /** @brief Returns the original clip's fps. */
    double getOriginalFps() const;

    /** @brief Calls AbstractProjectItem::setCurrent and sets the bin monitor to use the clip's producer. */
    void setCurrent(bool current, bool notify = true) override;

    bool rename(const QString &name, int column) override;

    QDomElement toXml(QDomDocument &document, bool includeMeta = false) override;

    // QVariant getData(DataType type) const override;

    /** @brief Sets thumbnail for this clip. */
    void setThumbnail(const QImage &);
    QPixmap thumbnail(int width, int height);

    /** @brief Sets the MLT producer associated with this clip
     *  @param producer The producer
     *  @param replaceProducer If true, we replace existing producer with this one
     *  @returns true if producer was changed
     * . */
    bool setProducer(std::shared_ptr<Mlt::Producer> producer, bool replaceProducer);

    /** @brief Returns true if this clip already has a producer. */
    bool isReady() const;

    /** @brief Returns this clip's producer. */
    Mlt::Producer *thumbProducer();

    /** @brief Recursively disable/enable bin effects. */
    void setBinEffectsEnabled(bool enabled) override;

    /** @brief Set properties on this clip. TODO: should we store all in MLT or use extra m_properties ?. */
    void setProperties(const QMap<QString, QString> &properties, bool refreshPanel = false);

    /** @brief Get an XML property from MLT produced xml. */
    static QString getXmlProperty(const QDomElement &producer, const QString &propertyName, const QString &defaultValue = QString());

    QString getToolTip() const override;

    /** @brief The clip hash created from the clip's resource. */
    const QString hash();

    /** @brief Returns a list of all markers comments between in ant out frames. */
    QStringList markersText(GenTime in, GenTime out) const;

    /** @brief Returns true if we are using a proxy for this clip. */
    bool hasProxy() const;

    /** Cache for every audio Frame with 10 Bytes */
    /** format is frame -> channel ->bytes */
    QVariantList audioFrameCache;
    bool audioThumbCreated() const;

    void updateParentInfo(const QString &folderid, const QString &foldername);
    void setWaitingStatus(const QString &id);
    /** @brief Returns true if the clip matched a condition, for example vcodec=mpeg1video. */
    bool matches(const QString &condition);
    /** @brief Returns true if the clip's video codec is equal to @param codec.
     *  @param audioCodec set to true if you want to check audio codec. When false, this will check the video codec
     */
    const QString codec(bool audioCodec) const;

    void addClipMarker(QList<CommentedTime> newMarkers, QUndoCommand *groupCommand);
    bool deleteClipMarkers(QUndoCommand *groupCommand);
    void addMarkers(QList<CommentedTime> &markers);
    /** @brief Create audio thumbnail for this clip. */
    void createAudioThumbs();
    /** @brief Returns the number of audio channels. */
    int audioChannels() const;
    /** @brief get data analysis value. */
    QStringList updatedAnalysisData(const QString &name, const QString &data, int offset);
    QMap<QString, QString> analysisData(bool withPrefix = false);
    /** @brief Abort running audio thumb process if any. */
    void abortAudioThumbs();
    /** @brief Returns the list of this clip's subclip's ids. */
    QStringList subClipIds() const;
    /** @brief Delete cached audio thumb - needs to be recreated */
    void discardAudioThumb();
    /** @brief Get path for this clip's audio thumbnail */
    const QString getAudioThumbPath(AudioStreamInfo *audioInfo);
    /** @brief Returns a cached pixmap for a frame of this clip */
    QImage findCachedThumb(int pos);
    void slotQueryIntraThumbs(const QList<int> &frames);
    /** @brief Returns true if this producer has audio and can be splitted on timeline*/
    bool isSplittable() const;

    /** @brief Returns a marker data at given pos */
    CommentedTime getMarker(const GenTime &pos) const;

protected:
    friend class ClipModel;
    /** @brief This is a call-back called by a ClipModel when it is created
        @param timeline ptr to the pointer in which this ClipModel is inserted
        @param clipId id of the inserted clip
     */
    void registerTimelineClip(std::weak_ptr<TimelineModel> timeline, int clipId);

    /** @brief This is a call-back called by a ClipModel when it is deleted
        @param clipId id of the deleted clip
    */
    void deregisterTimelineClip(int clipId);

public slots:
    void updateAudioThumbnail(const QVariantList &audioLevels);
    /** @brief Extract image thumbnails for timeline. */
    void slotExtractImage(const QList<int> &frames);
    void slotCreateAudioThumbs();
    /** @brief Set the Job status on a clip.
     * @param jobType The job type
     * @param status The job status (see definitions.h)
     * @param progress The job progress (in percents)
     * @param statusMessage The job info message */
    void setJobStatus(int jobType, int status, int progress = 0, const QString &statusMessage = QString());

    /* @brief Returns the marker model associated with this clip */
    std::shared_ptr<MarkerListModel> getMarkerModel() const;

private:
    bool m_abortAudioThumb;
    /** @brief Generate and store file hash if not available. */
    const QString getFileHash();
    /** @brief Store clip url temporarily while the clip controller has not been created. */
    QString m_temporaryUrl;
    Mlt::Producer *m_thumbsProducer;
    QMutex m_producerMutex;
    QMutex m_thumbMutex;
    QMutex m_intraThumbMutex;
    QMutex m_audioMutex;
    QFuture<void> m_thumbThread;
    QList<int> m_requestedThumbs;
    QFuture<void> m_intraThread;
    QList<int> m_intraThumbs;
    const QString geometryWithOffset(const QString &data, int offset);
    void doExtractImage();
    void doExtractIntra();

    std::map<int, std::weak_ptr<TimelineModel>> m_registeredClips;

    std::shared_ptr<MarkerListModel> m_markerModel;

private slots:
    void updateFfmpegProgress();

signals:
    void gotAudioData();
    void refreshPropertiesPanel();
    void refreshAnalysisPanel();
    void refreshClipDisplay();
    void thumbReady(int, const QImage &);
    void thumbUpdated(const QImage &);
    void updateJobStatus(int jobType, int status, int progress = 0, const QString &statusMessage = QString());
    /** @brief Clip is ready, load properties. */
    void loadPropertiesPanel();
    /** @brief Terminate running audio proxy job. */
    void doAbortAudioThumbs();
    void updateThumbProgress(long);
};

#endif
