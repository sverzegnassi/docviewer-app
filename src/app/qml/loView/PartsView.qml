/*
 * Copyright (C) 2015 Stefano Verzegnassi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.3
import Ubuntu.Components 1.1
import QtQuick.Layouts 1.1
import DocumentViewer.LibreOffice 1.0 as LibreOffice

import "../upstreamComponents"

ListView {
    id: view
    objectName: "view"
    clip: true

    property bool expanded: true

    currentIndex: view.model ? view.model.document.currentPart : -1

    delegate: ListItemWithActions {
        id: delegate

        width: parent.width
        height: units.gu(16)

        color: (view.model.document.currentPart === model.index) ? Theme.palette.selected.background
                                                                 : "transparent"

        AbstractButton {
            objectName: "abstractbutton"
            anchors.fill: parent

            onClicked: {
                view.model.document.currentPart = model.index
                pageStack.pop();
            }
        }

        RowLayout {
            anchors.fill: parent
            spacing: units.gu(1)

            Image {
                Layout.fillHeight: true
                Layout.preferredWidth: height
                fillMode: Image.PreserveAspectFit

                source: "image://lok/part/" + model.index
            }

            Label {
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                text: model.name
                color: (view.model.document.currentPart === model.index) ? UbuntuColors.orange
                                                                         : Theme.palette.selected.backgroundText
            }

            Label {
                text: model.index + 1
                color: (view.model.document.currentPart === model.index) ? UbuntuColors.orange
                                                                         : Theme.palette.selected.backgroundText
            }
        }
    }

    Scrollbar { flickableItem: view; parent: view.parent }
}
