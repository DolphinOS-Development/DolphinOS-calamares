/* === This file is part of Calamares - <http://github.com/calamares> ===
 *
 *   Copyright 2014, Teo Mrnjavac <teo@kde.org>
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

#include "CalamaresWindow.h"

#include "ViewManager.h"

#include <QBoxLayout>

CalamaresWindow::CalamaresWindow( QWidget* parent )
    : QWidget( parent )
{
    setupUi( this );
    // Hide close button
    setWindowFlags( Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint );

    //This should create a PageManager or ViewManager or whatever, which
    //should control the sidebar, next/back buttons and QSW.
    Calamares::ViewManager* vm = new Calamares::ViewManager( this );

    layout()->addWidget( vm->centralWidget() );
}
