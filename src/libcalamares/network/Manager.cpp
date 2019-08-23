/* === This file is part of Calamares - <https://github.com/calamares> ===
 *
 *   Copyright 2019, Adriaan de Groot <groot@kde.org>
 *
 *   Calamares is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Calamares is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Calamares. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Manager.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace CalamaresUtils
{
namespace Network
{
struct Manager::Private
{
    std::unique_ptr< QNetworkAccessManager > m_nam;
    QUrl m_hasInternetUrl;
    bool m_hasInternet;

    Private();
};

Manager::Private::Private()
    : m_nam( std::make_unique< QNetworkAccessManager >() )
    , m_hasInternet( false )
{
}

Manager::Manager()
    : d( std::make_unique< Private >() )
{
}

Manager::~Manager() {}

Manager&
Manager::instance()
{
    static auto* s_manager = new Manager();
    return *s_manager;
}

bool
Manager::hasInternet()
{
    return d->m_hasInternet;
}

bool
Manager::checkHasInternet()
{
    bool hasInternet = d->m_nam->networkAccessible() == QNetworkAccessManager::Accessible;

    if ( !hasInternet && ( d->m_nam->networkAccessible() == QNetworkAccessManager::UnknownAccessibility ) )
    {
        hasInternet = synchronousPing( d->m_hasInternetUrl );
    }
    d->m_hasInternet = hasInternet;
    return hasInternet;
}

void
Manager::setCheckHasInternetUrl( const QUrl& url )
{
    d->m_hasInternetUrl = url;
}

bool
Manager::synchronousPing( const QUrl& url )
{
    if ( !url.isValid() )
    {
        return false;
    }

    QNetworkRequest req = QNetworkRequest( url );
    QNetworkReply* reply = d->m_nam->get( req );
    QEventLoop loop;
    connect( reply, &QNetworkReply::finished, &loop, &QEventLoop::quit );
    loop.exec();
    reply->deleteLater();
    return reply->bytesAvailable();
}

QByteArray
Manager::synchronousGet( const QUrl& url )
{
    if ( !url.isValid() )
    {
        return QByteArray();
    }

    QNetworkRequest request( url );
    QNetworkReply* reply = d->m_nam->get( request );
    QEventLoop loop;
    connect( reply, &QNetworkReply::finished, &loop, &QEventLoop::quit );
    loop.exec();
    reply->deleteLater();
    return reply->readAll();
}

}  // namespace Network
}  // namespace CalamaresUtils
