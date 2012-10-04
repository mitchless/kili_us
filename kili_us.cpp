/*
    This file is part of Choqok, the KDE micro-blogging client

    Copyright (C) 2010-2012 Andrey Esin <gmlastik@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or ( at your option ) version 3 or any later version
    accepted by the membership of KDE e.V. ( or its successor approved
    by the membership of KDE e.V. ), which shall act as a proxy
    defined in Section 14 of version 3 of the license.


    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, see http://www.gnu.org/licenses/
*/

#include "kili_us.h"
#include <QtCore/QCoreApplication>
#include <KDebug>
#include <kio/netaccess.h>
#include <KAboutData>
#include <KGenericFactory>
#include <kglobal.h>
#include <kio/job.h>
#include <math.h>
#include <notifymanager.h>
#include <qjson/parser.h>

K_PLUGIN_FACTORY( MyPluginFactory, registerPlugin < Kili_us > (); )
K_EXPORT_PLUGIN( MyPluginFactory( "choqok_kili_us" ) )

Kili_us::Kili_us( QObject* parent, const QVariantList& )
        : Choqok::Shortener( MyPluginFactory::componentData(), parent )
{
   printf("%s\n", "I'm loading...");
}

Kili_us::~Kili_us()
{
}

QString Kili_us::shorten( const QString& url )
{
    kDebug() << "Using kili.us";

    QByteArray req;
    req = "{ 'url' : '" + QUrl::toPercentEncoding( KUrl( url ).url() ) + "' } ";

    QMap<QString, QString> metaData;
    metaData.insert("accept","*/*");
    metaData.insert("content-type", "Content-Type: application/json" );

    KIO::StoredTransferJob *job = KIO::storedHttpPost ( req, KUrl("http://kili.us/+/"), KIO::HideProgressInfo ) ;  
    if (!job){
      Choqok::NotifyManager::error( i18n("Error when creating job"), i18n("Kili.us error") );
      return url;
    }
    job->setMetaData(KIO::MetaData(metaData));

    QByteArray data;
    if ( KIO::NetAccess::synchronousRun( job, 0, &data ) ) {
        QString output( data );
        QJson::Parser parser;
        bool ok;
        QVariantMap map = parser.parse( data , &ok ).toMap();

        if ( ok ) {
            if ( !map[ "error" ].toString().isEmpty() ) {
                Choqok::NotifyManager::error( map[ "error" ].toString(), i18n("Kili.us error") );
                return url;
            }
            return map[ "short_url" ].toString();
        }
        Choqok::NotifyManager::error( i18n("Malformed response\n"), i18n("Kili.us error")  );
    } else {
        Choqok::NotifyManager::error( i18n("Cannot create a short URL.\n%1", job->errorString()), i18n("Kili.us error") );
    }
    return url;
}

#include "kili_us.moc"
