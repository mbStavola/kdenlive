/*
Copyright (C) 2014  Till Theato <root@ttill.de>
This file is part of kdenlive. See www.kdenlive.org.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
*/

#ifndef MLTCONNECTION_H
#define MLTCONNECTION_H

#include <QObject>

/**
 * @class MltConnection
 * @brief Initializes MLT and provides access to its API
 *
 */

class MltConnection
{

public:
    /** @brief Open connection to the MLT framework
        This constructor should be called only once
     */
    MltConnection(const QString &mltPath);

protected:

    /** @brief Locates the MLT environment.
     * @param mltPath (optional) path to MLT environment
     *
     * It tries to set the paths of the MLT profiles and renderer, using
     * mltPath, MLT_PREFIX, searching for the binary `melt`, or asking to the
     * user. It doesn't fill any list of profiles, while its name suggests so. */
    void locateMeltAndProfilesPath(const QString &mltPath = QString());

    static int instanceCounter;
};

#endif
